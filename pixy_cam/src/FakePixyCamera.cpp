#ifdef FAKE_CAMERA // <- This is defined if we're on a system that won't have a Pixy Camera connected.
#include <string>
#include <mutex>

#include "PixyCamera.h"

namespace pixy_cam
{
    PixyCamera::PixyCamera() :
        isInitialized( false ),
        buffer( nullptr )
    {
        this->buffer = new unsigned char[4]{ 'a', 'b', 'c', 'd' };
    }

    PixyCamera::~PixyCamera()
    {
        delete[] this->buffer;
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

    int PixyCamera::GetFrame(
        uint8_t mode,
        uint16_t width,
        uint16_t height,
        uint16_t* outputtedWidth,
        uint16_t* outputtedHeight,
        unsigned char*& pixels,
        uint32_t* numPixels
    )
    {
        *outputtedWidth = 2;
        *outputtedHeight = 2;
        pixels = this->buffer;
        *numPixels = 4;

        return 0;
    }
}
#endif
