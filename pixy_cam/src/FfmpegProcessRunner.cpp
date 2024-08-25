#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>

#include "FfmpegProcessRunner.h"
#include "FfmpegException.h"
#include "PixyCameraException.h"

namespace pixy_cam
{
    FfmpegProcessRunner::FfmpegProcessRunner( PixyCamera& camera, const std::string& url ) :
        camera( camera ),
        url( url ),
        startStopLock(),

        ffmpegProcessId( -1 ),
        stdinFile( 0 ),
        stdoutFile( 0 ),
        stderrFile( 0 ),

        stdoutThread( nullptr ),
        stderrThread( nullptr ),
        stdinThread( nullptr ),
        printLock(),

        isRunning( false ),
        keepProcessing( false )
    {
    }

    FfmpegProcessRunner::~FfmpegProcessRunner()
    {
        StopLoop();
    }

    void FfmpegProcessRunner::Init()
    {
        // Nothing to do.
    }

    void FfmpegProcessRunner::StartLoop()
    {
        std::lock_guard<std::mutex>( this->startStopLock );
        if( this->isRunning )
        {
            return;
        }
        
        int stdinPipe[2] = { 0, 0 };
        int stdoutPipe[2]= { 0, 0 };
        int stderrPipe[2]= { 0, 0 };

        if( pipe( stdinPipe ) == -1 )
        {
            throw new FfmpegException(
                "Could not make stdin pipe"
            );
        }

        if( pipe( stdoutPipe ) == -1 )
        {
            throw new FfmpegException(
                "Could not make stdout pipe"
            );
        }

        if( pipe( stderrPipe ) == -1 )
        {
            throw new FfmpegException(
                "Could not make stderr pipe"
            );
        }

        pid_t pid = fork();
        if( pid == -1 )
        {
            throw new FfmpegException(
                "Forking ffmpeg process failed: " + std::to_string( pid )
            );
        }
        else if( pid == 0 )
        {
            // Child Process
            // 0 in the array appears to be the parent process
            // 1 appears to be the child process.
            dup2( stdinPipe[0], STDIN_FILENO );
            dup2( stdoutPipe[1], STDOUT_FILENO );
            dup2( stderrPipe[1], STDERR_FILENO );

            // Child process is not expecting to read
            // stdout from this process, or write
            // to stdin. Close these directions.
            close( stdinPipe[1] );
            close( stdoutPipe[0] );
            close( stderrPipe[0] );

            execl(
                "ffmpeg",
                // Get Video From stdin.
                "-f",
                "rawvideo",
                "-pixel_format",
                "bayer_bggr8",
                "-video_size",
                std::to_string( this->camera.GetWidth() ) + "x" + std::to_string( this->camera.GetHeight() ),
                "-framerate",
                "10",
                "-i",
                "-" // <- From stdin.

                // Get null audio.
                "-f",
                "lavfi",
                "-i",
                "anullsrc=channel_layout-stereo:sample_rate=44100",

                // Set video encoding using H.264
                // with fast preset encoding and
                // bitrate of 3000k.
                "-c:v",
                "libx264",
                "-preset",
                "verfast",
                "-b:v",
                "3000k",
                "-maxrate",
                "3000k",
                "-bufsize",
                "6000k",

                // Set pixel format to YUV420P
                // and GOP size to 60.
                "-pix_fmt",
                "yuv420p",
                "-g",
                "60",

                // Sets up audio using AAC
                // with a bitrate of 128k.
                "-c:a",
                "aac",
                "b:a",
                "128k",

                // Maps the video to the first input.
                "-map",
                "0:v",

                // Maps the audio to the second input.
                "-map",
                "1:a",

                // Output to the stream.
                "-f",
                "flv",
                this->url.c_str(),
                nullptr
            );
            exit( 1 );
        }
        else
        {
            // Parent Process

            // Parent process is not expected to get data
            // from stdin from the child process nor
            // read from its stdout.  Close these pipes.
            close( stdinPipe[0] );
            close( stdoutPipe[1] );
            close( stderrPipe[1] );

            this->stdinFile = stdinPipe[1];
            this->stdoutFile = stdoutPipe[0];
            this->stderrFile = stderrPipe[0];
            this->ffmpegProcessId = pid;

            this->keepProcessing = true;

            this->stdoutThread = new std::thread(
                std::bind( &FfmpegProcessRunner::StdOutThreadEntry, this )
            );
            this->stderrThread = new std::thread(
                std::bind( &FfmpegProcessRunner::StdErrThreadEntry, this )
            );
            this->stdinThread = new std::thread(
                std::bind( &FfmpegProcessRunner::StdInThreadEntry, this )
            );
        }
    }

    void FfmpegProcessRunner::StopLoop()
    {
        std::lock_guard<std::mutex>( this->startStopLock );
        if( this->isRunning == false )
        {
            return;
        }

        this->keepProcessing = false;

        close( this->ffmpegProcessId );
        int exitCode;
        waitpid( this->ffmpegProcessId, &exitCode, 0 );

        this->stdinThread->join();
        this->stdoutThread->join();
        this->stderrThread->join();

        delete this->stdinThread;
        this->stdinThread = nullptr;

        delete this->stderrThread;
        this->stderrThread = nullptr;

        delete this->stdoutThread;
        this->stdoutThread = nullptr;

        close( this->stdinFile );
        close( this->stderrFile );
        close( this->stdoutFile );

        this->isRunning = false;
    }

    void FfmpegProcessRunner::StdOutThreadEntry()
    {
        ReadLoop( this->stdoutFile, std::cout );
    }
    
    void FfmpegProcessRunner::StdErrThreadEntry()
    {
        ReadLoop( this->stdoutFile, std::cerr );
    }
    
    void FfmpegProcessRunner::StdInThreadEntry()
    {
        uint16_t actualWidth;
        uint16_t actualHeight;
        std::vector<uint8_t> rawBytes;

        try
        {
            while( this->keepProcessing )
            {
                int returnValue = this->camera.GetFrame(
                    0x21, // <- Unsure what this does, but only 0x21 works.
                    &actualWidth,
                    &actualHeight,
                    rawBytes
                );

                if( returnValue < 0 )
                {
                    throw PixyCameraException( returnValue );
                }

                ssize_t bytesWritten = ::write(
                    this->ffmpegProcessId,
                    rawBytes.data(),
                    rawBytes.size()
                );

                if( bytesWritten == -1 )
                {
                    throw new FfmpegException( "Could not write bytes to ffmpeg" );
                }
            }
        }
        catch( const std::exception& e )
        {
            std::lock_guard<std::mutex>( this->printLock );
            std::cerr << e.what() << std::endl;
        }
    }

    void FfmpegProcessRunner::ReadLoop( int file, std::ostream& outFile )
    {
        const size_t bufferSize = 4096;
        std::vector<uint8_t> readBuffer( bufferSize );

        while( this->keepProcessing )
        {
            ssize_t bytesRead = ::read(
                file,
                readBuffer.data(),
                readBuffer.size()
            );
            if( bytesRead > 0 )
            {
                std::string output = std::string(
                    &readBuffer[0],
                    &readBuffer[bytesRead - 1]
                );
                std::lock_guard<std::mutex>( this->printLock );
                outFile << output;
            }
        }

        std::lock_guard<std::mutex>( this->printLock );
        outFile << std::endl;
    }
}
