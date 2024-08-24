#ifndef FFMPEG_RUNNER_H_
#define FFMPEG_RUNNER_H_

#include <mutex>
#include <thread>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include "PixyCamera.h"

namespace pixy_cam
{
    class FfmpegRunner
    {
        public:
            FfmpegRunner( PixyCamera& camera, const std::string& url );
            ~FfmpegRunner();

            void StartLoop();
            void StopLoop();

            bool IsHealthy() const;

        private:
            FfmpegRunner() = delete;

            void CreateInputStream();
            void CreateOutputStream();
            void ThreadEntry();

            bool isRunning;
            volatile bool keepRunning;
            std::string threadErrorMessage;

            PixyCamera& camera;
            const std::string& url;
            std::thread* thread;
            std::mutex startStopLock;

            AVFrame* cameraFrame;
            SwsContext* swsContext;

            AVStream* outputStream;
            AVFormatContext* outputFormatContext;
            AVCodecContext* outputCodecContext;
            AVFrame* outputStreamFrame;
    };
}

#endif // FFMPEG_RUNNER_H_