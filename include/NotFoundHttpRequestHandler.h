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

#ifndef NOT_FOUND_HTTP_REQUEST_HANDLER_H_
#define NOT_FOUND_HTTP_REQUEST_HANDLER_H_

#include "BaseHttpRequestHandler.h"

namespace pixy_cam
{
    class NotFoundHttpRequestHandler : public BaseHttpRequestHandler
    {
        public:
            NotFoundHttpRequestHandler();
            ~NotFoundHttpRequestHandler();

            void handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;
            void handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

        private:
            void handleHttpRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response );
    };
}

#endif // NOT_FOUND_HTTP_REQUEST_HANDLER_H_