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
    pinMode(32, OUTPUT);
    digitalWrite(32, 1);
    delay(1000);
    digitalWrite(32, 0);

    SIM800Serial.begin(9600);
    SIM800Serial.print(F("AT\r\n"));
    uint32_t t = millis();

    while (t+500>millis())
    {
        if(SIM800Serial.available())
        {
            network_state = NetworkState::AVAILABLE;
            print("sim800 ok");
            break;
        }else
            print("connecting...");
        delay(50);
    }

    SIM800Serial.print(F("AT+CMGF=1\r\n"));
    SIM800Serial.print(F("AT+CNMI=2,1,0,0,0\r\n"));
    SIM800Serial.print(F("AT+CLTS=1\r\n"));
    #endif

    setInterval(new CallbackMethod<GSM>(this, &GSM::downloadSerial), 10);
    //setInterval(new CallbackMethod<GSM>(this, &GSM::askForHour), 5000);
    //setInterval(new CallbackMethod<GSM>(this, &GSM::getNewMessagesMODE), 6000);
    addEventListener(new CallbackMethod<GSM>(this, &GSM::getNewMessagesMODE), new ConditionMethod<GSM>(this, &GSM::messageAvailable));
    addEventListener(new CallbackMethod<GSM>(this, &GSM::getNumberWhoCall), new ConditionMethod<GSM>(this, &GSM::callAvailable));
    addEventListener(new CallbackMethod<GSM>(this, &GSM::getHour), new ConditionMethod<GSM>(this, &GSM::hourAvailable));
    addEventListener(new CallbackMethod<GSM>(this, &GSM::showCall), new ConditionMethod<GSM>(this, &GSM::numberFromRing));
    addEventListener(new CallbackMethod<GSM>(this, &GSM::getNewMessagesPARSE), new ConditionMethod<GSM>(this, &GSM::listMessageAvailable));
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
        char c = read();
        data.push_back(c);
        //print(data);
        Serial.print(c);
    }    
    getError();
}

bool GSM::getError()
{
    #ifdef BUILD_EMU
        return true;
    #endif
    bool s = data.find("ERROR", 0) != -1;
    if (s)
    {
        gsm_print("ERROR from sim800");
        locked = false;
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
    return data.find("+CCLK") != -1 && data.find("OK", data.find("+CCLK")) != -1;
}

bool GSM::numberFromRing()
{
    return data.find("+CLCC") != -1 && data.find("OK", data.find("+CLCC")) != -1;
}

bool GSM::listMessageAvailable()
{
    return data.find("AT+CMGL=\"REC UNREAD\"") != -1 && data.find("OK", data.find("AT+CMGL=\"REC UNREAD\"")) != -1;
}

void GSM::getNewMessagesMODE()
{
    if(locked) return;
    locked = true;
    clear();
    this->gsm_print("AT+CMGF=1\r\n");
    print("getNewMessagesMODE");
    setTimeout(new CallbackMethod<GSM>(this, &GSM::getNewMessagesGET), 500);
}

void GSM::getNewMessagesGET()
{
    clear();
    this->gsm_print("AT+CMGL=\"REC UNREAD\"\r\n");
    print("getNewMessagesGET");
}

void GSM::getNewMessagesPARSE()
{
    std::vector<Message> messages;

    print("==={\n"+data+"}===");

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

        message = data.substr(k+2+1, data.find("\n\nOK", k+1)-k-2-1-1);

        messages.push_back({number, encoder.HexStringToString(message), date});
        
        i = j+1;
    }

    clear();

    if(messages.size() != 0)
    {
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
    }
    else
    {
        print("[GSM] I: No message received");
    }

    locked = false;
}

void GSM::sendNewMessageMODE(std::string number, std::string message)
{
    print("[GSM] I: message to " + number);
    locked = true;
    this->number_buffer = number;
    this->message_buffer = message;

    clear();
    this->gsm_print("AT+CMGF=1\r\n");
    
    while(!getOK() && !getError())
        downloadSerial();

    if(getError())
    {
        print("[GSM] E: Can't send message");
        clear();
        return;
    }
    
    print("[GSM] I: sending number to module...");
    clear();
    this->gsm_print("AT+CMGS=\"" + this->number_buffer + "\"\r");

    delay(10);

    print("[GSM] I: sending message to module...");
    clear();
    this->gsm_print(this->message_buffer + (char)26);

    while(!getOK() && !getError())
        downloadSerial();
    
    if(getError())
    {
        print("[GSM] E: Can't send message");
        clear();
        return;
    }

    sendNewMessageSEND();
}

void GSM::sendNewMessageSEND()
{
    clear();
    print("[GSM] I: Message sent!");
    locked = false;
}

void GSM::getNumberWhoCall()
{
    locked = true;
    data="";
    this->gsm_print("AT+CLCC\r\n");
}

void GSM::showCall()
{
    if(call || getError())
    {
        data="";
        return;
    }
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

    numberiscalling=number;
    call=true;
    Phone::get_a_call();
    call=false;

    if (getCall != nullptr)
    {
        getCall(number);
    }

    locked = false;
}

void GSM::answerCall(bool answer)
{
    if(answer)
    {
        this->gsm_print("ATA\r\n");
        call=true;
    }
    else
    {
        this->gsm_print("ATH\r\n");
        call=false;
    }
}

bool GSM::callEnded()
{
    if(data.find("NO CARRIER") != -1)
    {
        data = "";
        call=false;
        return true;
    }
    return false;
}

void GSM::makeCall(string number)
{
    this->gsm_print("ATD" + number + ";\r\n");
    print("ATD" + number + ";");
    call=true;
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