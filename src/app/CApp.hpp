#ifndef C_APP_HPP
#define C_APP_HPP

#include <string>

class CApp 
{
    public:
    
        virtual ~CApp() = default;

        [[nodiscard]] virtual const char* getAppName() const = 0;

        virtual void execute() = 0;

        [[nodiscard]] virtual std::string getAppBasePath() const
        {
            std::string appBasePath;
            appBasePath += "apps/";
            appBasePath += getAppName();
            appBasePath += "/";
            return appBasePath;
        }

        [[nodiscard]] std::string getAppIconPath() const
        {
            std::string appIconPath;
            appIconPath += getAppBasePath();
            appIconPath += "logo.png";
            return appIconPath;
        }

        [[nodiscard]] std::string getAppConfPath() const
        {
            std::string appConfPath;
            appConfPath += getAppName();
            appConfPath += "conf.txt";
            return appConfPath;
        }
};


#endif /* C_APP_HPP */
