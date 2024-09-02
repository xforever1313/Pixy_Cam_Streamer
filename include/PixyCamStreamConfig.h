#ifndef CONFIG_H_
#define CONFIG_H_

#include <libconfig.h++>
#include <string>

namespace pixy_cam
{
    class PixyCamStreamConfig
    {
        public:
            PixyCamStreamConfig( const std::string& configFileLocation );
            ~PixyCamStreamConfig();

            uint16_t Web_ListenPort() const;
            
            bool Camera_AutoWhiteBalance() const;
            bool Camera_AutoExposureCompensation() const;
            uint8_t Camera_Brightness() const;

            const std::string Stream_FfmpegPath() const;
            uint8_t Stream_Fps() const;
            const std::string Stream_RtmpServer() const;

        private:
            PixyCamStreamConfig() = delete;
            const libconfig::Setting& GetSettingCategory( const std::string& categoryName ) const;

            libconfig::Config config;
    };
}

#endif // CONFIG_H_