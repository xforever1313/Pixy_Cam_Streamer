#include "CameraStopRequest.h"
#include "FfmpegProcessRunner.h"

namespace pixy_cam
{
    CameraStopRequest::CameraStopRequest( FfmpegProcessRunner& ffmpeg ) :
        ffmpeg( ffmpeg )
    {
    }

    CameraStopRequest::~CameraStopRequest()
    {
    }

    void CameraStopRequest::handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response )
    {
        this->ffmpeg.StopLoop();
        sendSuccessResponse( response, "ACK" );
    }

    void CameraStopRequest::handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response )
    {
        sendBadRequestResponse( response, "GET request not valid for this method." );
    }
}