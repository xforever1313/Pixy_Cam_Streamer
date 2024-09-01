#ifndef PIXY_CAMERA_H_
#define PIXY_CAMERA_H_

#include <mutex>
#include <string>
#include <vector>

namespace pixy_cam
{
    class PixyCamera
    {
        public:
            PixyCamera( uint16_t frameWidth, uint16_t frameHeight );
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
            inline uint16_t GetWidth() const noexcept
            {
                return this->frameWidth;
            }
            inline uint16_t GetHeight() const noexcept
            {
                return this->frameHeight;
            }

            int StopProgram( int32_t* response );

            int GetFrame(
                uint8_t mode,
                uint16_t* outputtedWidth,
                uint16_t* outputtedHeight,
                std::vector<uint8_t>& pixels
            );

        private:
            PixyCamera() = delete;

            bool isInitialized;
            const uint16_t frameWidth;
            const uint16_t frameHeight;
            std::mutex takePictureLock;

            void ThrowIfNotInitialized() const;
    };
}

#endif // PIXY_CAMERA_H_