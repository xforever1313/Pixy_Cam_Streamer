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

#ifndef CAMERA_START_REQUEST_H_
#define CAMERA_START_REQUEST_H_

#include "BaseHttpRequestHandler.h"
#include "FfmpegProcessRunner.h"

namespace pixy_cam
{
    class CameraStartRequest : public BaseHttpRequestHandler
    {
        public: 
            CameraStartRequest( FfmpegProcessRunner& ffmpeg );
            ~CameraStartRequest();

            void handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

            void handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

        private:
            CameraStartRequest() = delete;

            FfmpegProcessRunner& ffmpeg;
    };
}

#endif // CAMERA_START_REQUEST_H_