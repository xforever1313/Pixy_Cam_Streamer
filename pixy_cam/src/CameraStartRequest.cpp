#include "CameraStartRequest.h"
#include "FfmpegProcessRunner.h"

namespace pixy_cam
{
    CameraStartRequest::CameraStartRequest( FfmpegProcessRunner& ffmpeg ) :
        ffmpeg( ffmpeg )
    {
    }

    CameraStartRequest::~CameraStartRequest()
    {
    }

    void CameraStartRequest::handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response )
    {
        this->ffmpeg.StartLoop();
        sendSuccessResponse( response, "ACK" );
    }

    void CameraStartRequest::handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response )
    {
        sendBadRequestResponse( response, "GET request not valid for this method." );
    }
}