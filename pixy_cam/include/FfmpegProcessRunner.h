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

        private:
            FfmpegProcessRunner() = delete;

            void StdOutThreadEntry();
            void StdErrThreadEntry();
            void StdInThreadEntry();
            void ReadLoop( int file, std::ostream& outFile );

            PixyCamera& camera;
            const std::string& url;
            std::mutex startStopLock;

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
    };
}

#endif // FFMPEG_PROCESS_RUNNER_H_