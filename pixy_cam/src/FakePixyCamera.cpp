#ifdef FAKE_CAMERA // <- This is defined if we're on a system that won't have a Pixy Camera connected.
#include <string>
#include <mutex>

#include "PixyCamera.h"

namespace pixy_cam
{
    PixyCamera::PixyCamera( uint16_t frameWidth, uint16_t frameHeight ) :
        isInitialized( false ),
        frameWidth( frameWidth ),
        frameHeight( frameHeight )
    {
    }

    PixyCamera::~PixyCamera()
    {
        this->isInitialized = false;
    }

    void PixyCamera::Init()
    {
        this->isInitialized = true;
    }

    const std::string PixyCamera::GetVersion()
    {
        return "0.0.0";
    }

    void PixyCamera::SetAutoExposureCompensation( bool enable )
    {

    }
    
    bool PixyCamera::GetAutoExposureCompensation()
    {
        return true;
    }

    void PixyCamera::SetAutoWhiteBalance( bool enable )
    {
    }
    
    bool PixyCamera::GetAutoWhiteBalance()
    {
        return true;
    }

    void PixyCamera::SetBrightness( const uint8_t newValue )
    {
    }

    int PixyCamera::GetBrightness() const
    {
        return 0;
    }

    bool PixyCamera::IsInitialized() const noexcept
    {
        return this->isInitialized;
    }

    int PixyCamera::StopProgram( int32_t* response )
    {
        *response = 0;
        return 0;
    }

    int PixyCamera::GetFrame(
        uint8_t mode,
        uint16_t* outputtedWidth,
        uint16_t* outputtedHeight,
        std::vector<unsigned char>& vec
    )
    {
        vec.clear();
        vec.push_back( 'a' );
        vec.push_back( 'b' );
        vec.push_back( 'c' );
        vec.push_back( 'd' );

        *outputtedWidth = 2;
        *outputtedHeight = 2;

        return 0;
    }
}
#endif
