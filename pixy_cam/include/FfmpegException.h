#ifndef FFMPEG_EXCEPTION_H_
#define FFMPEG_EXCEPTION_H_

#include <stdexcept>

namespace pixy_cam
{
    class FfmpegException : public std::runtime_error
    {
        public:
            FfmpegException( const std::string& message );
            ~FfmpegException();
    };
}

#endif // FFMPEG_EXCEPTION_H_