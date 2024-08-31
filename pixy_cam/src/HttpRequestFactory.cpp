#include <iostream>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "CameraStartRequest.h"
#include "CameraStopRequest.h"
#include "FfmpegProcessRunner.h"
#include "HttpRequestFactory.h"
#include "NotFoundHttpRequestHandler.h"
#include "SetBrightnessHttpRequestHandler.h"
#include "StreamStatusRequestHandler.h"
#include "TakePictureHttpRequestHandler.h"

namespace pixy_cam
{
    HttpRequestFactory::HttpRequestFactory( FfmpegProcessRunner& ffmpeg, PixyCamera& camera ) :
        ffmpeg( ffmpeg ),
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
        else if( "/capture" == request.getURI() )
        {
            return new TakePictureHttpRequestHandler( this->camera );
        }
        else if( "/start_stream" == request.getURI() )
        {
            return new CameraStartRequest( this->ffmpeg );
        }
        else if( "/stop_stream" == request.getURI() )
        {
            return new CameraStopRequest( this->ffmpeg );
        }
        else if( "/stream_status" == request.getURI() )
        {
            return new StreamStatusRequestHandler( this->ffmpeg );
        }
        else
        {
            return new NotFoundHttpRequestHandler();
        }
    }
}