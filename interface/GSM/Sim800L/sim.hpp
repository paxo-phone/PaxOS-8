#ifndef SIM_HPP
#define SIM_HPP

const char *daysOfWeek[7] = { "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi" };
const char *daysOfMonth[12] = { "Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Novembre", "Decembre"};

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

    void getNewMessagesMODE();
    void getNewMessagesGET();
    void getNewMessagesPARSE();

    void getNumberWhoCall();
    void showCall();
    void answerCall();

    void askForHour();
    void getHour();

    void (*saveMessages)(std::vector<Message>) = nullptr;
    void (*getCall)(string number) = nullptr;
    bool locked = false;

    uint seconds, minutes, hours, days, months, years = 0;
};

GSM gsm;

#endif