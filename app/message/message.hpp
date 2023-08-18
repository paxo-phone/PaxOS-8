#ifndef MESSAGE_HPP
#define MESSAGE_HPP

class Message : public App
{
    public:
    void main();
    
    static std::shared_ptr<App> createInstance() {
        return std::make_shared<Message>();
    }
    
    static void saveMessage(std::vector<GSM::Message> message);
    static bool notif;
};

bool Message::notif = false;

#endif
