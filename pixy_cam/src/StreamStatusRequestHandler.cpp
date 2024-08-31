#include "StreamStatusRequestHandler.h"

namespace pixy_cam
{
    StreamStatusRequestHandler::StreamStatusRequestHandler( FfmpegProcessRunner& ffmpeg ) :
        ffmpeg( ffmpeg )
    {
    }

    StreamStatusRequestHandler::~StreamStatusRequestHandler()
    {
    }

    void StreamStatusRequestHandler::handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response )
    {
        sendBadRequestResponse( response, "POST request not valid for this method." );
    }

    void StreamStatusRequestHandler::handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response )
    {
        std::string status = this->ffmpeg.GetStatus();
        if( status != "OK" )
        {
            sendServerErrorResponse( response, status );
        }
        else
        {
            sendSuccessResponse( response, status );
        }
    }
}