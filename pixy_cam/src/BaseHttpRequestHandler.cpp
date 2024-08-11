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

    void BaseHttpRequestHandler::sendSuccessResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "ACK"*/)
    {
        sendResponse(response, Poco::Net::HTTPResponse::HTTP_OK, message);
    }

    void BaseHttpRequestHandler::sendNotFoundResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Not Found"*/)
    {
        sendResponse(response, Poco::Net::HTTPResponse::HTTP_NOT_FOUND, message);
    }

    void BaseHttpRequestHandler::sendBadRequestResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Bad Request"*/)
    {
        sendResponse(response, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, message);
    }

    void BaseHttpRequestHandler::sendServerErrorResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Internal Server Error"*/)
    {
        sendResponse(response, Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, message);
    }

    void BaseHttpRequestHandler::sendForbiddenResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Forbidden"*/)
    {
        sendResponse(response, Poco::Net::HTTPResponse::HTTP_FORBIDDEN, message);
    }

    void BaseHttpRequestHandler::sendResponse(
        Poco::Net::HTTPServerResponse &response,
        Poco::Net::HTTPResponse::HTTPStatus status,
        const std::string &message
    )
    {
        response.setContentType("text/html");
        response.setStatus(status);
        std::ostream &ostr = response.send();
        ostr << message;
    }
}
