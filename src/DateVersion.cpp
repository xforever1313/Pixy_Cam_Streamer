//
// Pixy Cam Streamer - Stream a Pixy Camera to a RTMP server
// Copyright (C) 2024 Seth Hendrick
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// 
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

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