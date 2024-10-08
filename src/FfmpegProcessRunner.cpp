//
// Pixy Cam Streamer - Stream a Pixy Camera to a RTMP server
// Copyright (C) 2024 Seth Hendrick
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// 
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>

#include "FfmpegProcessRunner.h"
#include "FfmpegException.h"
#include "PixyCameraException.h"
#include "PixyCamStreamConfig.h"

namespace pixy_cam
{
    FfmpegProcessRunner::FfmpegProcessRunner( PixyCamera& camera, const PixyCamStreamConfig& config ) :
        camera( camera ),
        config( config ),
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
        keepProcessing( false ),

        statusString( "STOPPED" ),
        statusLock()
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

    std::string FfmpegProcessRunner::GetStatus() const
    {
        {
            std::lock_guard<std::mutex>( this->startStopLock );
            if( this->isRunning )
            {
                return "STOPPED";
            }
        }

        std::lock_guard<std::mutex>( this->statusLock );
        return this->statusString;
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
            throw FfmpegException(
                "Could not make stdin pipe"
            );
        }

        if( pipe( stdoutPipe ) == -1 )
        {
            throw FfmpegException(
                "Could not make stdout pipe"
            );
        }

        if( pipe( stderrPipe ) == -1 )
        {
            throw FfmpegException(
                "Could not make stderr pipe"
            );
        }

        pid_t pid = fork();
        if( pid == -1 )
        {
            throw FfmpegException(
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

            std::string sizeArg = std::to_string( this->camera.GetWidth() ) + "x" + std::to_string( this->camera.GetHeight() );

            execl(
                this->config.Stream_FfmpegPath().c_str(),
                "-hide_banner",
                "-loglevel",
                "info",

                // Get null audio
                "-ar",
                "44100",
                "-ac",
                "2",
                "-acodec",
                "pcm_s16le",
                "-f",
                "s16le",
                "-ac",
                "2",
                "-i",
                "/dev/zero",

                // Get Video From stdin.
                "-f",
                "rawvideo",
                "-pixel_format",
                "bayer_bggr8",
                "-video_size",
                sizeArg.c_str(),
                "-framerate",
                std::to_string( this->config.Stream_Fps() ).c_str(),
                "-i",
                "pipe:",  // <- From stdin.

                // Set audio codec
                "-acodec",
                "aac",
                "-ab",
                "128k",
                "-g",
                "50",
                "-strict",
                "experimental",

                // Set video codec
                "-vcodec",
                "h264",
                "-g",
                "2",
                "-preset",
                "ultrafast",
                "-crf",
                "1",
                "-minrate",
                "400k",
                "-maxrate",
                "1000k",
                "-bufsize",
                "1000k",

                // Output to the stream.
                "-f",
                "flv",
                this->config.Stream_RtmpServer().c_str(),
                nullptr
            );
            exit( 0 );
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

        this->isRunning = true;
    }

    void FfmpegProcessRunner::StopLoop()
    {
        std::lock_guard<std::mutex>( this->startStopLock );
        if( this->isRunning == false )
        {
            return;
        }

        this->keepProcessing = false;

        this->stdinThread->join();
        close( this->stdinFile );

        kill( this->ffmpegProcessId, SIGINT );
        close( this->ffmpegProcessId );
        int exitCode;
        waitpid( this->ffmpegProcessId, &exitCode, 0 );

        this->stdoutThread->join();
        this->stderrThread->join();

        delete this->stdinThread;
        this->stdinThread = nullptr;

        delete this->stderrThread;
        this->stderrThread = nullptr;

        delete this->stdoutThread;
        this->stdoutThread = nullptr;

        close( this->stderrFile );
        close( this->stdoutFile );

        this->isRunning = false;
    }

    void FfmpegProcessRunner::StdOutThreadEntry()
    {
        ReadLoop( this->stdoutFile, std::cout );

        std::lock_guard<std::mutex>( this->printLock );
        std::cout << "stdout thread exiting" << std::endl;
    }

    void FfmpegProcessRunner::StdErrThreadEntry()
    {
        ReadLoop( this->stderrFile, std::cerr );

        std::lock_guard<std::mutex>( this->printLock );
        std::cerr << "stderr thread exiting" << std::endl;
    }

    void FfmpegProcessRunner::StdInThreadEntry()
    {
        uint16_t actualWidth;
        uint16_t actualHeight;
        std::vector<uint8_t> rawBytes;

        try
        {
            SetStatus( "OK" );
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
                    this->stdinFile,
                    rawBytes.data(),
                    rawBytes.size()
                );

                if( bytesWritten == -1 )
                {
                    throw FfmpegException( "Could not write bytes to ffmpeg" );
                }

                float timeForFps = 1.0f / this->config.Stream_Fps() * 1000.0;
                timeForFps -=20; // Need to go slightly faster than the FPS so Ffmpeg doesn't break.

                std::this_thread::sleep_for( std::chrono::milliseconds( static_cast<int64_t>( timeForFps ) ) );
            }
            SetStatus( "EXITING" );
        }
        catch( const std::exception& e )
        {
            std::lock_guard<std::mutex>( this->printLock );
            std::cerr << e.what() << std::endl;
            SetStatus( e.what() );
        }

        std::lock_guard<std::mutex>( this->printLock );
        std::cout << "stdin thread exiting" << std::endl;
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
                outFile << output << std::endl;
            }
            else if( bytesRead == 0 )
            {
                // End-of-file, exit loop.
                break;
            }
            else
            {
                std::lock_guard<std::mutex>( this->printLock );
                outFile << "Failed to read output from sub-process: " << bytesRead << std::endl;
                break;
            }
        }

        std::lock_guard<std::mutex>( this->printLock );
        outFile << std::endl;
    }

    void FfmpegProcessRunner::SetStatus( const std::string& str )
    {
        std::lock_guard<std::mutex>( this->statusLock );
        this->statusString = str;
    }
}
