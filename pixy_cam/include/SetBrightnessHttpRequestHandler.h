#ifndef SET_BRIGHTNESS_HTTP_REQUEST_HANDLER_H_
#define SET_BRIGHTNESS_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "BaseHttpRequestHandler.h"

namespace pixy_cam
{
    class SetBrightnessHttpRequestHandler : public pixy_cam::BaseHttpRequestHandler
    {
        public: 
            SetBrightnessHttpRequestHandler();
            ~SetBrightnessHttpRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
    };
}

#endif // SET_BRIGHTNESS_HTTP_REQUEST_HANDLER_H_
