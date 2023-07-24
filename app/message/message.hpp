#ifndef MESSAGE_HPP
#define MESSAGE_HPP

class Message : public App
{
    public:
    void main();
    static void saveMessage(std::vector<GSM::Message> message);
    static bool notif;
};

bool Message::notif = false;

#endif