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