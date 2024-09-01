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

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>

#include "NotFoundHttpRequestHandler.h"

namespace pixy_cam
{

    NotFoundHttpRequestHandler::NotFoundHttpRequestHandler()
    {
    }

    NotFoundHttpRequestHandler::~NotFoundHttpRequestHandler()
    {
    }

    void NotFoundHttpRequestHandler::handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response )
    {
        handleHttpRequest( request, response );
    }

    void NotFoundHttpRequestHandler::handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response )
    {
        handleHttpRequest(request, response);
    }

    void NotFoundHttpRequestHandler::handleHttpRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response )
    {
        sendNotFoundResponse( response, "404 - Not Found" );
    }
}
