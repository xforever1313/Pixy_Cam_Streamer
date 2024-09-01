#ifndef STREAM_STATUS_REQUEST_HANDLER_H_
#define STREAM_STATUS_REQUEST_HANDLER_H_

#include "BaseHttpRequestHandler.h"
#include "FfmpegProcessRunner.h"

namespace pixy_cam
{
    class StreamStatusRequestHandler : public BaseHttpRequestHandler
    {
        public: 
            StreamStatusRequestHandler( FfmpegProcessRunner& ffmpeg );
            ~StreamStatusRequestHandler();

            void handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

            void handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

        private:
            StreamStatusRequestHandler() = delete;

            FfmpegProcessRunner& ffmpeg;
    };
}

#endif // STREAM_STATUS_REQUEST_HANDLER_H_
