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
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

#include "BaseHttpRequestHandler.h"

namespace pixy_cam
{
    BaseHttpRequestHandler::BaseHttpRequestHandler()
    {

    }

    BaseHttpRequestHandler::~BaseHttpRequestHandler()
    {
    }

    void BaseHttpRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
    {
        try
        {
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET)
            {
                handleGetRequest(request, response);
            }
            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
            {
                handlePostRequest(request, response);
            }
            else
            {
                sendBadRequestResponse(response);
            }
        }
        catch(const std::exception& e)
        {
            sendServerErrorResponse( response, e.what() );
        }
        
    }

    void BaseHttpRequestHandler::sendSuccessResponseAsJson(Poco::Net::HTTPServerResponse &response, const std::string &message )
    {
        sendResponse( response, Poco::Net::HTTPResponse::HTTP_OK, message, "application/json" );
    }

    void BaseHttpRequestHandler::sendSuccessResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "ACK"*/)
    {
        sendResponse( response, Poco::Net::HTTPResponse::HTTP_OK, message, "text/plain" );
    }

    void BaseHttpRequestHandler::sendNotFoundResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Not Found"*/)
    {
        sendResponse( response, Poco::Net::HTTPResponse::HTTP_NOT_FOUND, message, "text/plain" );
    }

    void BaseHttpRequestHandler::sendBadRequestResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Bad Request"*/)
    {
        sendResponse( response, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, message, "text/plain" );
    }

    void BaseHttpRequestHandler::sendServerErrorResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Internal Server Error"*/)
    {
        sendResponse( response, Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, message, "text/plain" );
    }

    void BaseHttpRequestHandler::sendForbiddenResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Forbidden"*/)
    {
        sendResponse( response, Poco::Net::HTTPResponse::HTTP_FORBIDDEN, message, "text/plain" );
    }

    void BaseHttpRequestHandler::sendResponse(
        Poco::Net::HTTPServerResponse &response,
        Poco::Net::HTTPResponse::HTTPStatus status,
        const std::string &message,
        const std::string &contentType
    )
    {
        response.setContentType( contentType );
        response.setStatus( status );
        std::ostream &ostr = response.send();
        ostr << message;
    }
}
