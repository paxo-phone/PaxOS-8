#include <stdint.h>
#include <string>

#include "app/launcher.hpp"

#include "interface/interface.hpp"
#include "widgets/gui.hpp"
#include "network/network.hpp"
#include "app/message/message.hpp"

using namespace std;

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    #include <atomic>

    struct Rectangle {
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
    };

    bool *shouldUS;
    struct Rectangle* *screenUZ;

    void flushScreen()
    {
        *shouldUS = true;
    }
#endif

/*
    HEADERS OF THE PAXOS_8
*/

#ifdef ESP32
void setup()
{
    esp_task_wdt_init(30, true);

    esp_task_wdt_deinit();
#endif
    
#if defined(__linux__) || defined(__APPLE__)
void setup(bool *shouldUpdateScreen, Rectangle* *screenUpdateZones)
    {
    shouldUS = shouldUpdateScreen;
    screenUZ = screenUpdateZones;
#elif defined(_WIN32) || defined(_WIN64)

void setup(bool *shouldUpdateScreen, struct Rectangle* *screenUpdateZones)
{
    shouldUS = shouldUpdateScreen;
    screenUZ = screenUpdateZones;
#endif

    screen::init();
    
    storage::init();
    light::init();
    home_button::init();

    const std::string test_ssid = "Fairphone Gab";
    const std::string test_passwd = "bonjourbonjour";
    network::connect(test_ssid, test_passwd);
    
    gsm.init();
    gsm.saveMessages = Message::saveMessage;

    launcher();

}

void loop()
{
}
