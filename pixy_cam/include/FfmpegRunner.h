#ifndef FFMPEG_RUNNER_H_
#define FFMPEG_RUNNER_H_

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include "PixyCamera.h"

namespace pixy_cam
{
    class FfmpegRunner
    {
        public:
            FfmpegRunner( PixyCamera& camera );
            ~FfmpegRunner();

            void StartLoop();

        private:
            FfmpegRunner() = delete;

            void CreateOutputStream();

            PixyCamera& camera;
            AVStream* outputStream;
            AVFormatContext* outputFormatContext;
            AVCodecContext* outputCodecContext;
    };
}

#endif // FFMPEG_RUNNER_H_