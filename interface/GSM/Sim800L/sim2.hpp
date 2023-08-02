// OLD MODULE NOT USED

#ifndef SIM_HPP
#define SIM_HPP

const char *daysOfWeek[7] = { "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi" };
const char *daysOfMonth[12] = { "Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Novembre", "Decembre"};

#include "encoder.hpp"

enum NetworkState
{
    UNAVAILABLE,
    AVAILABLE,
    CONNECTED
};

class GSM
{
    public:

    struct Message
    {
        std::string number;
        std::string message;
        std::string hour;
    };

    void init();
    void write(char c);
    void gsm_print(std::string s);
    char read();
    bool available();
    void downloadSerial();
    bool getError();
    bool getOK();
    void clear() { data = ""; };
    bool isLocked() { return locked; };

    std::string data = "";

    bool messageAvailable();
    bool callAvailable();
    bool hourAvailable();
    bool numberFromRing();
    bool listMessageAvailable();

    void getNewMessagesMODE();
    void getNewMessagesGET();
    void getNewMessagesPARSE();

    void sendNewMessageMODE(std::string number, std::string message);
    void sendNewMessageNUMBER();
    void sendNewMessageSEND();

    std::string number_buffer = "";
    std::string message_buffer = "";

    void getNumberWhoCall();
    void showCall();
    void answerCall(bool answer);
    bool callEnded();
    void makeCall(string number);

    void askForHour();
    void getHour();

    void (*saveMessages)(std::vector<Message>) = nullptr;
    void (*getCall)(string number) = nullptr;
    bool locked = false;
    bool call = false;
    std::string numberiscalling = "";

    uint seconds, minutes, hours, days, months, years = 0;
    NetworkState network_state = NetworkState::UNAVAILABLE; // 
};

GSM gsm;

#endif