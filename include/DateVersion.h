#ifndef DATE_VERSION_H_
#define DATE_VERSION_H_

#include <string>

namespace pixy_cam
{
    class DateVersion
    {
        public:
            //Both these methods have a static string in the backend,
            // making returning a reference safe.
            static const std::string &getBuildTime();
            static const std::string &getVersionNumber();

        private:
            DateVersion();
            ~DateVersion();
    };
}

#endif // DATE_VERSION_H_
