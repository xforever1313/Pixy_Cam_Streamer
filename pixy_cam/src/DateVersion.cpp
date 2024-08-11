#include <string>

#include "DateVersion.h"

namespace pixy_cam
{
    const std::string &DateVersion::getBuildTime(){
        static const std::string buildTime(std::string(__DATE__)  + std::string(" ") + std::string(__TIME__));
        return buildTime;
    }

    const std::string &DateVersion::getVersionNumber(){
        #ifndef VERSION
        #define VERSION "0.0.0"
        #endif

        static const std::string versionString(VERSION);
        return versionString;
    }
}