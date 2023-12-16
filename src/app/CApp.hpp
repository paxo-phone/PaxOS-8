//
// Created by Kevin on 16/12/2023.
//

#ifndef PAXOS_CAPP_HPP
#define PAXOS_CAPP_HPP

#include <string>
#include <sstream>

class CApp {

public:
    [[nodiscard]] virtual const char* getAppName() const = 0;

    virtual void main() = 0;
    virtual ~CApp() = default;

    [[nodiscard]] virtual std::string getAppBasePath() const
    {
        std::stringstream ss;
        ss << "apps/";
        ss << getAppName();
        ss << "/";
        return ss.str();
    }

    [[nodiscard]] std::string getAppIconPath() const
    {
        std::stringstream ss;
        ss << getAppBasePath();
        ss << "/logo.png";
        return ss.str();
    }

    [[nodiscard]] std::string getAppConfPath() const
    {
        std::stringstream ss;
        ss << getAppName();
        ss << "/conf.txt";
        return ss.str();
    }
};


#endif //PAXOS_CAPP_HPP
