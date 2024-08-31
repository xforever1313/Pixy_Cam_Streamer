#ifndef CAMERA_STOP_REQUEST_H_
#define CAMERA_STOP_REQUEST_H_

#include "BaseHttpRequestHandler.h"
#include "FfmpegProcessRunner.h"

namespace pixy_cam
{
    class CameraStopRequest : public BaseHttpRequestHandler
    {
        public: 
            CameraStopRequest( FfmpegProcessRunner& ffmpeg );
            ~CameraStopRequest();

            void handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

            void handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

        private:
            CameraStopRequest() = delete;

            FfmpegProcessRunner& ffmpeg;
    };
}

#endif // CAMERA_STOP_REQUEST_H_