#ifndef CAMERA_START_REQUEST_H_
#define CAMERA_START_REQUEST_H_

#include "BaseHttpRequestHandler.h"
#include "FfmpegProcessRunner.h"

namespace pixy_cam
{
    class CameraStartRequest : public BaseHttpRequestHandler
    {
        public: 
            CameraStartRequest( FfmpegProcessRunner& ffmpeg );
            ~CameraStartRequest();

            void handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

            void handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

        private:
            CameraStartRequest() = delete;

            FfmpegProcessRunner& ffmpeg;
    };
}

#endif // CAMERA_START_REQUEST_H_