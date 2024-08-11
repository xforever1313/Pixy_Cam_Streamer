#include <mutex>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

#include "pixy.h"
#include "TakePictureHttpRequestHandler.h"

namespace pixy_cam
{
    TakePictureHttpRequestHandler::TakePictureHttpRequestHandler()
    {
        // TODO: Should probably make static so we don't have a billion heap allocations.pixy
        this->buffer = new unsigned char[78000];
    }

    TakePictureHttpRequestHandler::~TakePictureHttpRequestHandler()
    {
        delete[] this->buffer;
        this->buffer = nullptr;
    }

    void TakePictureHttpRequestHandler::handleGetRequest(
        Poco::Net::HTTPServerRequest &request,
        Poco::Net::HTTPServerResponse &response
    )
    {
        sendBadRequestResponse( response, "GET request not valid for this method." );
    }

    void TakePictureHttpRequestHandler::handlePostRequest(
        Poco::Net::HTTPServerRequest &request,
        Poco::Net::HTTPServerResponse &response
    )
    {
        // Only allow one resource in at the camera at a time.
        std::lock_guard<std::mutex>( this->camLock );

        unsigned char *pixels;  //returned pointer to video frame buffer
        int32_t camResponse, fourcc;
        int8_t renderflags;
        int return_value, res;
        uint16_t width, height;
        uint32_t  numPixels;

        camResponse = 0;
        return_value = pixy_command(
            "cam_getFrame",  // String id for remote procedure
            0x01, 0x21,      // mode 0 = 1280x800 25 fps.  1 byte
            0x02,   0,       // xoffset - 2 bytes
            0x02,   0,       // yoffset - 2 bytes
            0x02, 320,       // width   - 2 bytes
            0x02, 200,       // height  - 2 bytes
            0,               // separator
            &camResponse,    // pointer to mem address for return value
            &fourcc,         //contrary to docs, the next 5 args are needed
            &renderflags,
            &width,
            &height,
            &numPixels,
            &pixels,        // pointer to mem address for returned frame
            0
        );

        // quit now if not successful:
        if( return_value < 0 )
        {
            sendServerErrorResponse(
                response,
                "Error taking picture; error code: " + std::to_string( return_value ) +
                ".  Returned width: " + std::to_string( width ) + ", height: " + std::to_string( height ) + ", num pixels: " + std::to_string( numPixels )
            );
            return;
        }

        sendSuccessResponse( response, "TODO: Create Base64 here" );
    }
}