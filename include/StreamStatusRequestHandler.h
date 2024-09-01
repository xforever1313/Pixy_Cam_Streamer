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

#ifndef STREAM_STATUS_REQUEST_HANDLER_H_
#define STREAM_STATUS_REQUEST_HANDLER_H_

#include "BaseHttpRequestHandler.h"
#include "FfmpegProcessRunner.h"

namespace pixy_cam
{
    class StreamStatusRequestHandler : public BaseHttpRequestHandler
    {
        public: 
            StreamStatusRequestHandler( FfmpegProcessRunner& ffmpeg );
            ~StreamStatusRequestHandler();

            void handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

            void handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

        private:
            StreamStatusRequestHandler() = delete;

            FfmpegProcessRunner& ffmpeg;
    };
}

#endif // STREAM_STATUS_REQUEST_HANDLER_H_
