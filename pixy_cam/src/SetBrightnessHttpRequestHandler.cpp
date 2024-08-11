#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <stdexcept>
#include <string>

#include "pixy.h"
#include "SetBrightnessHttpRequestHandler.h"

namespace pixy_cam
{
    SetBrightnessHttpRequestHandler::SetBrightnessHttpRequestHandler()
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
        int brightness = pixy_cam_get_brightness();
        if( brightness < 0 )
        {
            sendServerErrorResponse( response, "Could not get brightness, got error message " + std::to_string( brightness ) );
        }

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

            int error = pixy_cam_set_brightness( brightness );
            if( error < 0 )
            {
                sendServerErrorResponse( response, "Could not set brightness, got error " + std::to_string( error ) );
            }
            else
            {
                sendSuccessResponse( response );
            }
        }
        catch(const Poco::NotFoundException &e)
        {
            sendBadRequestResponse( response, "Missing Field" );
        }
    }
}
