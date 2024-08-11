#ifndef PIXY_CAMERA_H_
#define PIXY_CAMERA_H_

#include <string>
#include <vector>

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

            void SetAutoExposureCompensation( bool enable );
            bool GetAutoExposureCompensation();

            void SetAutoWhiteBalance( bool enable );
            bool GetAutoWhiteBalance();

            bool IsInitialized() const noexcept;

            int StopProgram( int32_t* response );

            int GetFrame(
                uint8_t mode,
                uint16_t width,
                uint16_t height,
                uint16_t* outputtedWidth,
                uint16_t* outputtedHeight,
                std::vector<unsigned char>& pixels
            );

        private:
            bool isInitialized;

            void ThrowIfNotInitialized() const;
    };
}

#endif // PIXY_CAMERA_H_