#ifndef MESSAGE_HPP
#define MESSAGE_HPP

class Message : public CppAppContainer
{
    public:
    void main();
    
    static void saveMessage(std::vector<GSM::Message> message);
    static bool notif;
};

bool Message::notif = false;

#endif
