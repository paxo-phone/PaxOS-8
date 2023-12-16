#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "../app.hpp"
#include "../../interface/interface.hpp"

class Message : public CApp
{
    
public:
    [[nodiscard]] const char* getAppName() const override { return "Message"; }
    void main() override;
    
    static void saveMessage(std::vector<GSM::Message> message);
    static bool notif;
};

#endif
