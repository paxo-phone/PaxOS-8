#ifndef C_APP_HPP
#define C_APP_HPP

#include <string>
#include <sstream>
#include <cstdint>

class CApp 
{
    public:
    
        virtual ~CApp() = default;

        [[nodiscard]] virtual const char* getAppName() const = 0;

        virtual void main() = 0;

        [[nodiscard]] virtual std::string getAppBasePath() const
        {
            std::string o = "";
            o += "apps/";
            o += getAppName();
            o += "/";
            return o;
        }

        [[nodiscard]] std::string getAppIconPath() const
        {
            std::string o = "";
            o += getAppBasePath();
            o += "logo.png";
            return o;
        }

        [[nodiscard]] std::string getAppConfPath() const
        {
            std::string o = "";
            o += getAppName();
            o += "conf.txt";
            return o;
        }
};


#endif /* C_APP_HPP */
