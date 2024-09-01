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