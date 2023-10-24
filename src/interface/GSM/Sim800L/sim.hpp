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
    bool moduleCheck();
    uint32_t timer = 0;
    bool moduleReady() { get_data(); return data.find("SMS Ready")!=-1; }
    void initRequests();
    void write(char c);
    void gsm_print(std::string data);
    void get_data();

    void update();
    void add_request(std::vector<void (GSM::*)(void)> request, bool priority = false);
    void add_key(Key key);

    // specific methods

    bool hourAvailable();
    bool numberFromRing();
    bool listMessageAvailable();

    void askForMessages();
    void askForMessagesPrio();
    void getNewMessagesMODE();
    void getNewMessagesGET();
    void getNewMessagesPARSE();
    void getNewMessagesClear();

    void sendNewMessageMODE(std::string number, std::string message);
    void sendNewMessageRequest();

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
    #ifdef BUILD_EMU
    void parseHourFromComputer(time_t* time);
    #endif

    void getNetworkQuality();
    void askNetworkQuality();
    void parseNetworkQuality();
    uint16_t quality = 0;

    void getBatteryLevel();
    void askBatteryLevel();
    void parseBatteryLevel();
    uint16_t batteryLevel = 4;

    void (*saveMessages)(std::vector<Message>) = nullptr;
    void (*getCall)(std::string number) = nullptr;
    bool locked = false;
    bool call = false;
    std::string numberiscalling = "";

    uint16_t seconds, minutes, hours, days, months, years = 0;
    NetworkState network_state = NetworkState::UNAVAILABLE;

    // end

    private:
    uint64_t timeout = 0;
    std::string data;
    std::vector<std::vector<void (GSM::*)(void)>> requests;
    std::vector<Key> keys;
    uint16_t actual_cmd_count = 0;
    bool break_ = false;
};

GSM gsm;

#endif
