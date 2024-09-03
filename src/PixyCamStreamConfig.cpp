#include <cstdint>
#include <libconfig.h++>
#include <limits>

#include "PixyCamStreamConfig.h"

using namespace libconfig;

namespace pixy_cam
{
    PixyCamStreamConfig::PixyCamStreamConfig( const std::string& fileLocation ) :
        config()
    {
        this->config.readFile( fileLocation.c_str() );
    }

    PixyCamStreamConfig::~PixyCamStreamConfig()
    {
    }

    uint16_t PixyCamStreamConfig::Web_ListenPort() const
    {
        const Setting& webController = GetSettingCategory( "web_controller" );
        uint port = webController.lookup( "port" );
        if( port > std::numeric_limits<uint16_t>::max() )
        {
            throw SettingTypeException( webController, "port" );
        }
        return static_cast<uint16_t>( port );
    }

    bool PixyCamStreamConfig::Camera_AutoWhiteBalance() const
    {
        const Setting& cameraSettings = GetSettingCategory( "pixy_cam_settings" );
        return cameraSettings.lookup( "auto_white_balance" );
    }

    bool PixyCamStreamConfig::Camera_AutoExposureCompensation() const
    {
        const Setting& cameraSettings = GetSettingCategory( "pixy_cam_settings" );
        return cameraSettings.lookup( "auto_exposure_compensation" );
    }

    uint8_t PixyCamStreamConfig::Camera_Brightness() const
    {
        const Setting& cameraSettings = GetSettingCategory( "pixy_cam_settings" );
        uint value = cameraSettings.lookup( "brightness" );
        if( value > std::numeric_limits<uint8_t>::max() )
        {
            throw SettingTypeException( cameraSettings, "brightness" );
        }

        return static_cast<uint8_t>( value );
    }

    const std::string PixyCamStreamConfig::Stream_FfmpegPath() const
    {
        const Setting& streamSettings = GetSettingCategory( "stream_settings" );
        return streamSettings.lookup( "ffmpeg_path" );
    }

    uint8_t PixyCamStreamConfig::Stream_Fps() const
    {
        const Setting& streamSettings = GetSettingCategory( "stream_settings" );
        uint value = streamSettings.lookup( "fps" );
        if( value > std::numeric_limits<uint8_t>::max() )
        {
            throw SettingTypeException( streamSettings, "fps" );
        }

        return static_cast<uint8_t>( value );
    }

    const std::string PixyCamStreamConfig::Stream_RtmpServer() const
    {
        const Setting& streamSettings = GetSettingCategory( "stream_settings" );
        return streamSettings.lookup( "rtmp_server" );
    }

    const Setting& PixyCamStreamConfig::GetSettingCategory( const std::string& categoryName ) const
    {
        return this->config.getRoot()["pixy_cam_streamer"][categoryName.c_str()];
    }
}