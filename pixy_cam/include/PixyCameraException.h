#ifndef PIXY_CAMERA_EXCEPTION_H_
#define PIXY_CAMERA_EXCEPTION_H_

#include <stdexcept>

namespace pixy_cam
{
    class PixyCameraException : public std::runtime_error
    {
        public:
            PixyCameraException( int errorCode );
            ~PixyCameraException();

            int GetErrorCode() const noexcept;

        private:
            const int errorCode;
    };
}

#endif // PIXY_CAMERA_EXCEPTION_H_