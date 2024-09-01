//
// Pixy Cam Streamer - Stream a Pixy Camera to a RTMP server
// Copyright (C) 2024 Seth Hendrick
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// 
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef BASEHTTPREQUESTHANDLER_H
#define BASEHTTPREQUESTHANDLER_H

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

namespace pixy_cam
{
    ///
    /// \brief All http request handlers inherit from this.  They must implement
    ///        handlePostRequest and handleGetRequest.
    ///
    class BaseHttpRequestHandler : public Poco::Net::HTTPRequestHandler
    {
        public:
            ///
            /// \brief overrides the required method from parent class.  This will call handleGetRequest or handlePostRequest respectfully
            ///
            void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

        protected:
            BaseHttpRequestHandler();
            virtual ~BaseHttpRequestHandler();

            ///
            /// \brief Called when a post request comes through
            ///
            virtual void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) = 0;

            ///
            /// \brief Called when a get request comes through
            ///
            virtual void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) = 0;

            ///
            /// \brief Sends back message with status 200 with content type of application/json.
            ///
            void sendSuccessResponseAsJson(Poco::Net::HTTPServerResponse &response, const std::string &message);

            ///
            /// \brief Sends back message with status 200.
            ///
            void sendSuccessResponse(Poco::Net::HTTPServerResponse &response, const std::string &message = "ACK");

            ///
            /// \brief Sends back not found message with status 404
            ///
            void sendNotFoundResponse(Poco::Net::HTTPServerResponse &response, const std::string &message = "Not Found");

            ///
            /// \brief Sends back a bad request response, status 400
            ///
            void sendBadRequestResponse(Poco::Net::HTTPServerResponse &response, const std::string &message = "Bad Request");

            ///
            /// \brief Sends back a bad server response, status 500
            ///
            void sendServerErrorResponse( Poco::Net::HTTPServerResponse &response, const std::string &message = "Internal Server Error" );

            ///
            /// \brief Sends back a forbidden request response, status 403
            ///
            void sendForbiddenResponse(Poco::Net::HTTPServerResponse &response, const std::string &message = "Forbidden");

        private:
             void sendResponse(
                Poco::Net::HTTPServerResponse &response,
                Poco::Net::HTTPResponse::HTTPStatus status,
                const std::string &message,
                const std::string &contentType
            );
    };
}

#endif // BASEHTTPREQUESTHANDLER_H