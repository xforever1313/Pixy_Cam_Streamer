#ifdef FAKE_CAMERA // <- This is defined if we're on a system that won't have a Pixy Camera connected.
#include <string>
#include <mutex>

#include "PixyCamera.h"

namespace pixy_cam
{
    PixyCamera::PixyCamera() :
        isInitialized( false )
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
}
#endif
