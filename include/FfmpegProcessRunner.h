#ifndef FFMPEG_PROCESS_RUNNER_H_
#define FFMPEG_PROCESS_RUNNER_H_

#include <mutex>
#include <thread>

#include "PixyCamera.h"

namespace pixy_cam
{
    class FfmpegProcessRunner
    {
        public:
            FfmpegProcessRunner( PixyCamera& camera, const std::string& url );
            ~FfmpegProcessRunner();

            void Init();
            void StartLoop();
            void StopLoop();

            std::string GetStatus() const;

        private:
            FfmpegProcessRunner() = delete;

            void StdOutThreadEntry();
            void StdErrThreadEntry();
            void StdInThreadEntry();
            void ReadLoop( int file, std::ostream& outFile );
            void SetStatus( const std::string& str );

            const uint8_t fps;

            PixyCamera& camera;
            const std::string& url;
            mutable std::mutex startStopLock;

            pid_t ffmpegProcessId;
            int stdinFile;
            int stdoutFile;
            int stderrFile;

            std::thread* stdoutThread;
            std::thread* stderrThread;
            std::thread* stdinThread;
            std::mutex printLock;

            bool isRunning;
            volatile bool keepProcessing;
            
            std::string statusString;
            mutable std::mutex statusLock;
    };
}

#endif // FFMPEG_PROCESS_RUNNER_H_