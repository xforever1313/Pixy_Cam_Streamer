#include <string>

extern "C" {
#include <libavutil/avutil.h>
}

#include "FfmpegException.h"

namespace pixy_cam
{
    FfmpegException::FfmpegException( int errorCode ) :
        FfmpegException::FfmpegException( GetErrorCodeMessage( errorCode ) )
    {
        
    }

    FfmpegException::FfmpegException( const std::string& message ) :
        std::runtime_error( message )
    {
    }

    FfmpegException::~FfmpegException()
    {
    }

    std::string FfmpegException::GetErrorCodeMessage( int errorCode )
    {
        const size_t bufferSize = 255;
        char errorBuffer[bufferSize];
        int return_code = av_strerror( errorCode, errorBuffer, bufferSize );
        if( return_code < 0 )
        {
            return std::string( "Unknown Error: " ) + std::to_string( errorCode );
        }

        return std::string( errorBuffer );
    }
}
