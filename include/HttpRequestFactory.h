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

#ifndef HTTP_REQUEST_FACTORY_H_
#define HTTP_REQUEST_FACTORY_H_

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <string>

#include "FfmpegProcessRunner.h"
#include "PixyCamera.h"

namespace pixy_cam
{
    class HttpRequestFactory : public Poco::Net::HTTPRequestHandlerFactory
    {
        public:
            HttpRequestFactory( FfmpegProcessRunner& ffmpeg, PixyCamera& camera );
            ~HttpRequestFactory();

            Poco::Net::HTTPRequestHandler *createRequestHandler( const Poco::Net::HTTPServerRequest &request ) override;

        private:
            HttpRequestFactory() = delete;

            FfmpegProcessRunner& ffmpeg;
            PixyCamera& camera;
    };
}

#endif // HTTP_REQUEST_FACTORY_H_