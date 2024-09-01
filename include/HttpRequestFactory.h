#ifndef HTTP_REQUEST_FACTORY_H_
#define HTTP_REQUEST_FACTORY_H_

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <string>

#include "FfmpegProcessRunner.h"
#include "PixyCamera.h"

namespace pixy_cam
{
    class HttpRequestFactory : public Poco::Net::HTTPRequestHandlerFactory
    {
        public:
            HttpRequestFactory( FfmpegProcessRunner& ffmpeg, PixyCamera& camera );
            ~HttpRequestFactory();

            Poco::Net::HTTPRequestHandler *createRequestHandler( const Poco::Net::HTTPServerRequest &request ) override;

        private:
            HttpRequestFactory() = delete;

            FfmpegProcessRunner& ffmpeg;
            PixyCamera& camera;
    };
}

#endif // HTTP_REQUEST_FACTORY_H_