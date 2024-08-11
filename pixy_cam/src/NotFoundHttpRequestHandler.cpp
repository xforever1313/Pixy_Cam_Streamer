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
