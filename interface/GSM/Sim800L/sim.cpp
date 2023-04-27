#include "../../shell.hpp"
#include "../../../tasks/tasks.hpp"
#include "sim.hpp"

#ifdef BUILD_PAXO
#include <SoftwareSerial.h>
SoftwareSerial SIM800Serial(26, 27);
#endif

void GSM::init()
{
    #ifdef BUILD_PAXO
    SIM800Serial.begin(9600);
    SIM800Serial.print(F("AT+CMGF=1\r\n"));
    SIM800Serial.print(F("AT+CNMI=2,1,0,0,0\r\n"));
    #endif

    setInterval(new CallbackMethod<GSM>(this, &GSM::downloadSerial), 10);
    setInterval(new CallbackMethod<GSM>(this, &GSM::askForHour), 10000);
    addEventListener(new CallbackMethod<GSM>(this, &GSM::getNewMessagesMODE), new ConditionMethod<GSM>(this, &GSM::messageAvailable));
    addEventListener(new CallbackMethod<GSM>(this, &GSM::getNumberWhoCall), new ConditionMethod<GSM>(this, &GSM::callAvailable));
    addEventListener(new CallbackMethod<GSM>(this, &GSM::getHour), new ConditionMethod<GSM>(this, &GSM::hourAvailable));
}

void GSM::write(char c)
{
    #ifdef BUILD_PAXO
    SIM800Serial.write(c);
    #endif
}

void GSM::gsm_print(std::string s)
{
    for (int i = 0; i < s.length(); i++)
        write(s[i]);
}

char GSM::read()
{
    #ifdef BUILD_PAXO
    return SIM800Serial.read();
    #endif

    return '.';
}

bool GSM::available()
{
    #ifdef BUILD_PAXO
    return SIM800Serial.available();
    #endif

    return false;
}

void GSM::downloadSerial()
{
    while (available())
    {
        data.push_back(read());
        //print(data);
    }    
    getError();
}

bool GSM::getError()
{
    bool s = data.find("ERROR", 0) != -1;
    if (s)
    {
        gsm_print("ERROR from sim800");
    }
    return s;
}

bool GSM::getOK()
{
    return data.find("OK", 0) != -1;
}

bool GSM::messageAvailable()
{
    return data.find("+CMTI") != -1 && !locked;
}

bool GSM::callAvailable()
{
    return data.find("RING") != -1 && !locked;
}

bool GSM::hourAvailable()
{
    return data.find("+CCLK") != -1 && data.find("OK", data.find("+CCLK")) != -1 && !locked;
}

void GSM::getNewMessagesMODE()
{
    locked = true;
    clear();
    this->gsm_print("AT+CMGF=1\r\n");
    addEventListener(new CallbackMethod<GSM>(this, &GSM::getNewMessagesGET), new ConditionMethod<GSM>(this, &GSM::getOK), true);
}

void GSM::getNewMessagesGET()
{
    clear();
    this->gsm_print("AT+CMGL=\"REC UNREAD\"\r\n");
    addEventListener(new CallbackMethod<GSM>(this, &GSM::getNewMessagesPARSE), new ConditionMethod<GSM>(this, &GSM::getOK), true);
}

void GSM::getNewMessagesPARSE()
{
    std::vector<Message> messages;

    for (uint i = 0; i < data.size();) {
        std::string number, message, date;
        uint j = data.find("+CMGL:", i);

        if (j == -1)
        {
            break;
        }

        uint k = data.find("\"", j);
        k = data.find("\"", k+1);
        k = data.find("\"", k+1);

        number = data.substr(k+1, data.find("\"", k+1)-k-1);

        k = data.find("\"", k+1);
        k = data.find("\"", k+1);
        k = data.find("\"", k+1);
        k = data.find("\"", k+1);

        date = data.substr(k+1, data.find("\"", k+1)-k-4);

        k = data.find("\"", k+1);

        message = data.substr(k+2, data.find("\n\n", k+1)-k-2);

        messages.push_back({number, message, date});
        
        i = j+1;
    }

    clear();

    if(saveMessages != nullptr)
    {
        saveMessages(messages);
        print("[GSM] I: " + to_string(messages.size()) + " messages saved");
    }
    else
    {
        print("[GSM] E: Can't save messages because no function was provided");
        print("[GSM] I: " + to_string(messages.size()) + " messages received");
    }
    locked = false;
}

void GSM::getNumberWhoCall()
{
    locked = true;
    data="";
    this->gsm_print("AT+CLCC\r\n");
    addEventListener(new CallbackMethod<GSM>(this, &GSM::showCall), new ConditionMethod<GSM>(this, &GSM::getOK), true);
}

void GSM::showCall()
{
    std::string number = "";

    if(data.find("+CLCC:") != -1)
    {
        uint k = data.find("+CLCC:");
        k = data.find("\"", k);

        number = data.substr(
            k+1,
            data.find("\"", k+1) - k-1
        );
        data="";
    }

    print("number " + number + "is calling...");
    
    if (getCall != nullptr)
    {
        getCall(number);
    }

    locked = false;
}

void GSM::answerCall()
{
    this->gsm_print("ATA");
}

void GSM::askForHour()
{
    if(locked)
        return;
    
    data="";
    this->gsm_print("AT+CCLK?\r\n");
}

void GSM::getHour()
{
    print("getHour");
    if(data.find("\"") == -1)
    {
        data = "";
        return;
    }
    data = data.substr(data.find("\"") + 1, data.find_last_of("\"") - 1 - data.find("\"") - 1);

    print("data:" + data);
    years = stoi(data.substr(0, 2));
    months = stoi(data.substr(3, 5-3));
    days = stoi(data.substr(6, 8-6));
    hours = stoi(data.substr(9, 11-9));
    minutes = stoi(data.substr(12, 14-12));
    seconds = stoi(data.substr(15, 17-15));
    print(to_string(years) + " " + to_string(months) + " " + to_string(days) + " " + to_string(hours) + " " + to_string(minutes) + " " + to_string(seconds));
    locked = false;
    data = "";
}