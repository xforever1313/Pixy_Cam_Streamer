#ifndef PIXY_CAMERA_H_
#define PIXY_CAMERA_H_

#include <string>

namespace pixy_cam
{
    class PixyCamera
    {
        public:
            PixyCamera();
            ~PixyCamera();

            void Init();

            const std::string GetVersion();

            void SetBrightness( const uint8_t brightness );
            int GetBrightness() const;

            bool IsInitialized() const noexcept;

            int GetFrame(
                uint8_t mode,
                uint16_t width,
                uint16_t height,
                uint16_t* outputtedWidth,
                uint16_t* outputtedHeight,
                unsigned char*& pixels,
                uint32_t* numPixels
            );

        private:
            bool isInitialized;

            void ThrowIfNotInitialized() const;

            #ifdef FAKE_CAMERA
            unsigned char* buffer;
            #endif
    };
}

#endif // PIXY_CAMERA_H_