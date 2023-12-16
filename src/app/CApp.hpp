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

    [[nodiscard]] std::string getIconPath() const
    {
        std::stringstream ss;
        ss << "apps/";
        ss << getAppName();
        ss << "/logo.png";
        return ss.str();
    }

    [[nodiscard]] std::string getConfPath() const
    {
        std::stringstream ss;
        ss << getAppName();
        ss << "/conf.txt";
        return ss.str();
    }
};


#endif //PAXOS_CAPP_HPP
