#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "../app.hpp"
#include "../../interface/interface.hpp"

class Message : public CppAppContainer
{
    public:
    void main();
    
    static void saveMessage(std::vector<GSM::Message> message);
    static bool notif;
};

#endif
