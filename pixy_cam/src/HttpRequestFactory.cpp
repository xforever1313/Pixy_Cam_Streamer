#include <iostream>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "HttpRequestFactory.h"
#include "NotFoundHttpRequestHandler.h"
#include "SetBrightnessHttpRequestHandler.h"
#include "TakePictureHttpRequestHandler.h"

namespace pixy_cam
{
    HttpRequestFactory::HttpRequestFactory( PixyCamera& camera ) :
        camera( camera )
    {
    }

    HttpRequestFactory::~HttpRequestFactory()
    {
    }

    Poco::Net::HTTPRequestHandler* HttpRequestFactory::createRequestHandler( const Poco::Net::HTTPServerRequest &request )
    {
        std::cout << request.getMethod() << " Request made to " << request.getURI() << " from " << request.clientAddress() << std::endl;

        // Poco handles deleting the created classes.
        if( "/brightness" == request.getURI() )
        {
            return new SetBrightnessHttpRequestHandler( this->camera );
        }
        else
        {
            return new NotFoundHttpRequestHandler();
        }
    }
}