#include <mutex>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>
#include <vector>

#include "pixy.h"
#include "PixyCamera.h"
#include "PixyCameraException.h"
#include "TakePictureHttpRequestHandler.h"

namespace pixy_cam
{
    TakePictureHttpRequestHandler::TakePictureHttpRequestHandler( PixyCamera& camera ) :
        camera( camera )
    {
    }

    TakePictureHttpRequestHandler::~TakePictureHttpRequestHandler()
    {
    }

    void TakePictureHttpRequestHandler::handleGetRequest(
        Poco::Net::HTTPServerRequest &request,
        Poco::Net::HTTPServerResponse &response
    )
    {
        uint16_t actualWidth = 0;
        uint16_t actualHeight = 0;
        std::vector<unsigned char> pixels;  //returned pointer to video frame buffer

        int return_value = this->camera.GetFrame(
            0x21, // <- Unsure what this does, but only 0x21 works.
            &actualWidth,
            &actualHeight,
            pixels
        );

        // quit now if not successful:
        if( return_value < 0 )
        {
            PixyCameraException ex( return_value );

            sendServerErrorResponse(
                response,
                "Error taking picture: " + std::string( ex.what() ) +
                "  Returned width: " + std::to_string( actualWidth ) + ", height: " + std::to_string( actualHeight ) + ", num pixels: " + std::to_string( pixels.size() )
            );
            return;
        }

        // Taken from https://stackoverflow.com/a/59734450.
        using namespace boost::archive::iterators;
        using It = base64_from_binary<transform_width<std::vector<unsigned char>::const_iterator, 6, 8>>;
        std::string base64 = std::string( It( pixels.begin() ), It( pixels.end()) );

        // Add padding.
        base64 = base64.append( (3 - pixels.size() % 3 ) % 3, '=' );

        std::string jsonResponse = 
            std::string( "{" ) + 
                "\"base64\": \"" + base64 + "\"," + 
                "\"width\":" + std::to_string( actualWidth ) + "," +
                "\"height\":" + std::to_string( actualHeight ) + "" +
            "}";

        sendSuccessResponseAsJson( response, jsonResponse );
    }

    void TakePictureHttpRequestHandler::handlePostRequest(
        Poco::Net::HTTPServerRequest &request,
        Poco::Net::HTTPServerResponse &response
    )
    {
        sendBadRequestResponse( response, "POST request not valid for this method." );
    }
}