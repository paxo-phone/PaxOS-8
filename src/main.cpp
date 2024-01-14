#include <iterator>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <iostream>

#include "app/launcher.hpp"

/*
    HEADERS OF THE PAXOS_8
*/

#include "interface/interface.hpp"
#include "widgets/gui.hpp"
#include "tasks/tasks.hpp"
#include "lua/lua.hpp"
#include "app/CApp.hpp"
#include "network/network.hpp"
#include "app/message/message.hpp"

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
// Note that SDL will only redraw the screen if pShouldUpdateScreen is true, SDL set it to true if the screen has been touched.
bool *pShouldUpdateScreen;

void flushScreen()
{
    *pShouldUpdateScreen = true;
}
#endif

#ifdef ESP32
void setup()
{
    esp_task_wdt_init(30, true);

    esp_task_wdt_deinit();
#endif
    
#if defined(__linux__) || defined(__APPLE__) || defined(_WIN32) || defined(_WIN64)
void setup(bool *shouldUS)
{
    pShouldUpdateScreen = shouldUS;
#endif

    screen::init();
    
    storage::init();
    light::init();
    home_button::init();

    std::string test_ssid = "Fairphone Gab";
    std::string test_passwd = "bonjourbonjour";
    network::connect(test_ssid, test_passwd);
    
    gsm.init();
    gsm.saveMessages = Message::saveMessage;
    gsm.getHour();

    launcher();

}

void loop()
{
}
