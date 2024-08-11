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

            const std::string GetFrame();

        private:
            bool isInitialized;

            void ThrowIfNotInitialized() const;
    };
}

#endif // PIXY_CAMERA_H_