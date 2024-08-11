#ifndef NOT_FOUND_HTTP_REQUEST_HANDLER_H_
#define NOT_FOUND_HTTP_REQUEST_HANDLER_H_

#include "BaseHttpRequestHandler.h"

namespace pixy_cam
{
    class NotFoundHttpRequestHandler : public BaseHttpRequestHandler
    {
        public:
            NotFoundHttpRequestHandler();
            ~NotFoundHttpRequestHandler();

            void handlePostRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;
            void handleGetRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response ) override;

        private:
            void handleHttpRequest( Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response );
    };
}

#endif // NOT_FOUND_HTTP_REQUEST_HANDLER_H_