#include <string>

#include "FfmpegException.h"

namespace pixy_cam
{
    FfmpegException::FfmpegException( const std::string& message ) :
        std::runtime_error( message )
    {
    }

    FfmpegException::~FfmpegException()
    {
    }
}
