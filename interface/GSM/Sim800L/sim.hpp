#ifndef ASYNC_SIM
#define ASYNC_SIM

#include <string>
#include <vector>

#include "../../shell.hpp"
#include "../../../tasks/tasks.hpp"
#include "sim.hpp"

const char *daysOfWeek[7] = { "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi" };
const char *daysOfMonth[12] = { "Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Novembre", "Decembre"};

#include "encoder.hpp"

#ifdef BUILD_PAXO
    #include <SoftwareSerial.h>
    SoftwareSerial SIM800Serial(26, 27);
#endif

class GSM;

struct Key
{
    void (GSM::*func)(void);
    std::string key;
    bool isDetected;
};

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
    bool moduleReady() { get_data(); return data.find("SMS Ready")!=-1; }
    void initRequests();
    void write(char c);
    void gsm_print(std::string data);
    void get_data();

    void update();
    void add_request(std::vector<void (GSM::*)(void)> request);
    void add_key(Key key);

    // specific methods

    bool hourAvailable();
    bool numberFromRing();
    bool listMessageAvailable();

    void askForMessages();
    void getNewMessagesMODE();
    void getNewMessagesGET();
    void getNewMessagesPARSE();

    void sendNewMessageMODE(std::string number, std::string message);
    void sendNewMessageNUMBER();
    void sendNewMessageSEND();

    std::string number_buffer = "";
    std::string message_buffer = "";

    void askedForCall();
    void getNumberWhoCall();
    void showCall();
    void answerCall(bool answer);
    bool callEnded();
    void makeCall(std::string number);

    void getHour();
    void askForHour();
    void parseHour();

    void (*saveMessages)(std::vector<Message>) = nullptr;
    void (*getCall)(std::string number) = nullptr;
    bool locked = false;
    bool call = false;
    std::string numberiscalling = "";

    uint seconds, minutes, hours, days, months, years = 0;
    NetworkState network_state = NetworkState::UNAVAILABLE;

    // end

    private:
    uint timeout = 0;
    std::string data;
    std::vector<std::vector<void (GSM::*)(void)>> requests;
    std::vector<Key> keys;
    uint actual_cmd_count = 0;
};

GSM gsm;

#endif