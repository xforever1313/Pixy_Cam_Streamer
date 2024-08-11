#ifndef TAKE_PICTURE_HTTP_REQUEST_HANDLER_H_
#define TAKE_PICTURE_HTTP_REQUEST_HANDLER_H_

#include <mutex>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

#include "BaseHttpRequestHandler.h"

namespace pixy_cam
{
    class TakePictureHttpRequestHandler : public pixy_cam::BaseHttpRequestHandler
    {
        public: 
            TakePictureHttpRequestHandler();
            ~TakePictureHttpRequestHandler();

            void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

            void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

        private:
            unsigned char* buffer;
            std::mutex camLock;
    };
}

#endif
