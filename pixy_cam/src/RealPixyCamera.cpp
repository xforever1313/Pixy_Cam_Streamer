#ifndef FAKE_CAMERA // <- This is defined if we're on a system that won't have a Pixy Camera connected.
#include <string>
#include <mutex>
#include <vector>

#include "pixy.h"
#include "PixyCamera.h"
#include "PixyCameraException.h"

namespace pixy_cam
{
    PixyCamera::PixyCamera() :
        isInitialized( false )
    {
    }

    PixyCamera::~PixyCamera()
    {
        pixy_close();
        this->isInitialized = false;
    }

    void PixyCamera::Init()
    {
        int returnCode = pixy_init();
        if( returnCode < 0 )
        {
            throw PixyCameraException( returnCode );
        }

        this->isInitialized = true;
    }

    const std::string PixyCamera::GetVersion()
    {
        this->ThrowIfNotInitialized();

        uint16_t major;
        uint16_t minor;
        uint16_t build;
        int returnCode = pixy_get_firmware_version( &major, &minor, &build );
        if( returnCode < 0 )
        {
            throw PixyCameraException( returnCode );
        }

        return std::to_string( major ) + "." + std::to_string( minor ) + "." + std::to_string( build );
    }

    void PixyCamera::SetBrightness( const uint8_t newValue )
    {
        this->ThrowIfNotInitialized();

        int returnCode = pixy_cam_set_brightness( newValue );
        if( returnCode < 0 )
        {
            throw PixyCameraException( returnCode );
        }
    }

    int PixyCamera::GetBrightness() const
    {
        this->ThrowIfNotInitialized();

        int returnCode  = pixy_cam_get_brightness();
        if( returnCode < 0 )
        {
            throw PixyCameraException( returnCode );
        }

        return returnCode;
    }

    bool PixyCamera::IsInitialized() const noexcept
    {
        return this->isInitialized;
    }

    int PixyCamera::StopProgram( int32_t* response )
    {
        this->ThrowIfNotInitialized();

        return pixy_command( "stop", END_OUT_ARGS, response, END_IN_ARGS );
    }

    int PixyCamera::GetFrame(
        uint8_t mode,
        uint16_t width,
        uint16_t height,
        uint16_t* outputtedWidth,
        uint16_t* outputtedHeight,
        std::vector<unsigned char>& pixels
    )
    {
        this->ThrowIfNotInitialized();

        pixels.clear();

        unsigned char *rawPixels;  //returned pointer to video frame buffer

        int32_t camResponse = 0;
        int32_t fourcc = 0;
        int8_t renderflags = 0;
        uint32_t numPixels = 0;

        int return_value = pixy_command(
            "cam_getFrame", // String id for remote procedure
            0x01, mode,
            0x02, 0,        // xoffset - 2 bytes
            0x02, 0,        // yoffset - 2 bytes
            0x02, width,    // width - 2 bytes
            0x02, height,   // height - 2 bytes,
            0,              // separator
            &camResponse,    // pointer to mem address for return value
            &fourcc,        // contrary to docs, the next 5 args are needed
            &renderflags,
            outputtedWidth,
            outputtedHeight,
            &numPixels,
            &rawPixels,         // pointer to mem address for returned frame
            0
        );

        if( return_value == 0 )
        {
            for( uint32_t i = 0; i < numPixels; ++i )
            {
                pixels.push_back( rawPixels[i] );
            }
        }

        return return_value;
    }

    void PixyCamera::ThrowIfNotInitialized() const
    {
        if( this->isInitialized == false )
        {
            throw new std::runtime_error( "Pixy Cam not initialized, please call Init()." );
        }
    }
}
#endif
