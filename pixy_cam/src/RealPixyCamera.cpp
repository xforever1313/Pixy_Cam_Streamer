#ifndef FAKE_CAMERA // <- This is defined if we're on a system that won't have a Pixy Camera connected.
#include <string>
#include <mutex>

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

    const std::string GetVersion()
    {
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
        int returnCode = pixy_cam_set_brightness( newValue );
        if( returnCode < 0 )
        {
            throw PixyCameraException( returnCode );
        }
    }

    int PixyCamera::GetBrightness() const
    {
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

    void PixyCamera::ThrowIfNotInitialized() const
    {
        if( this->isInitialized == false )
        {
            throw new std::runtime_error( "Pixy Cam not initialized, please call Init()." );
        }
    }
}
#endif
