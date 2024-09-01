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

#include <stdexcept>

#include "pixy.h"
#include "PixyCameraException.h"

namespace pixy_cam
{
    static std::string GetPixyError( int errorCode )
    {
        if( errorCode == 0 )
        {
            return "Success.";
        }
        else if( errorCode == PIXY_ERROR_USB_IO )
        {
            return "USB Error: I/O.";
        }
        else if( errorCode == PIXY_ERROR_USB_BUSY )
        {
            return "USB Error: Busy.";
        }
        else if( errorCode == PIXY_ERROR_USB_NO_DEVICE )
        {
            return "USB Error: No Device.";
        }
        else if( errorCode == PIXY_ERROR_USB_NOT_FOUND )
        {
            return "USB Error: Target not found.";
        }
        else if( errorCode == PIXY_ERROR_CHIRP )
        {
            return "Chirp Protocol Error.";
        }
        else if( errorCode == PIXY_ERROR_INVALID_COMMAND )
        {
            return "Pixy Error: Invalid Command.";
        }
        else if( errorCode == PIXY_ERROR_INVALID_PARAMETER )
        {
            return "Pixy Error: Invalid Parameter.";
        }
        else
        {
            return "Unknown Error.";
        }
    }

    PixyCameraException::PixyCameraException( int errorCode ) :
        std::runtime_error( std::to_string( errorCode ) + " - " + GetPixyError( errorCode ) ),
        errorCode( errorCode )
    {
    }

    PixyCameraException::~PixyCameraException()
    {
    }

    int PixyCameraException::GetErrorCode() const noexcept
    {
        return this->errorCode;
    }
}
