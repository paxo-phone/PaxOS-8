#ifndef ASYNC_SIM
#define ASYNC_SIM

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#include <string>
#include <vector>

#include "../../console.hpp"
#include "../../../tasks/tasks.hpp"
#include "sim.hpp"

extern const char *daysOfWeek[7];
extern const char *daysOfMonth[12];

#define f_Date(annee, mois) ( ((mois) <= 2) ? ((annee) - 1) : (annee) )
#define g_Date(mois) ( ((mois) <= 2) ? ((mois) + 13) : ((mois) + 1) )

struct Date
{
    int jour;
    int mois;
    long int annee;
};

#define myCalculOfDay(d) (1461 * f_Date(d.annee, d.mois) / 4 + 153 * g_Date(d.mois) / 5 + d.jour)
#define myWhatDay(d_) ( (myCalculOfDay(d_) - 621049) % 7 )

#include "encoder.hpp"

#ifdef ESP32
    #include <SoftwareSerial.h>
    extern SoftwareSerial SIM800Serial;
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
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
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

inline GSM gsm;

#endif
