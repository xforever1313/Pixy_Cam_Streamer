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

#ifndef SET_BRIGHTNESS_HTTP_REQUEST_HANDLER_H_
#define SET_BRIGHTNESS_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "PixyCamera.h"
#include "BaseHttpRequestHandler.h"

namespace pixy_cam
{
    class SetBrightnessHttpRequestHandler : public pixy_cam::BaseHttpRequestHandler
    {
        public: 
            SetBrightnessHttpRequestHandler( PixyCamera& camera );
            ~SetBrightnessHttpRequestHandler();

            void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

            void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

        private:
            PixyCamera& camera;
    };
}

#endif // SET_BRIGHTNESS_HTTP_REQUEST_HANDLER_H_
