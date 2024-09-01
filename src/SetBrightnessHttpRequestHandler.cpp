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

#include <memory>
#include <limits>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <stdexcept>
#include <string>

#include "pixy.h"
#include "PixyCamera.h"
#include "SetBrightnessHttpRequestHandler.h"

namespace pixy_cam
{
    SetBrightnessHttpRequestHandler::SetBrightnessHttpRequestHandler( PixyCamera& camera ) :
        camera( camera )
    {
    }

    SetBrightnessHttpRequestHandler::~SetBrightnessHttpRequestHandler()
    {
    }

    void SetBrightnessHttpRequestHandler::handleGetRequest(
        Poco::Net::HTTPServerRequest &request,
        Poco::Net::HTTPServerResponse &response
    )
    {
        int brightness = this->camera.GetBrightness();
        sendSuccessResponse( response, std::to_string( brightness ) );
    }

    void SetBrightnessHttpRequestHandler::handlePostRequest(
        Poco::Net::HTTPServerRequest &request,
        Poco::Net::HTTPServerResponse &response
    )
    {
        try
        {
            Poco::Net::HTMLForm form(request, request.stream());
            const std::string &brightString = form["BRIGHTNESS"];

            unsigned long value = std::stoul( brightString );
            if( value > std::numeric_limits<uint8_t>::max() )
            {
                sendBadRequestResponse( response, "Brightness is out-of-range." );
                return;
            }

            uint8_t brightness = static_cast<uint8_t>( value );

            this->camera.SetBrightness( brightness );

            sendSuccessResponse( response );
        }
        catch( const Poco::NotFoundException &e )
        {
            sendBadRequestResponse( response, "Missing Field" );
        }
    }
}
