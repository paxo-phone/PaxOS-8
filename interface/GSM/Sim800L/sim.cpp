#include "sim.hpp"

/*#ifndef localtime_r
// Can be undefined, so we use "localtime_s" in replacement
#define localtime_r(timer, buf) localtime_s((tm *const) timer, (const time_t *const) buf)
#endif*/

void GSM::init()
{
    #ifdef BUILD_PAXO
        pinMode(32, OUTPUT);
        if(SIM800Serial)
            SIM800Serial.end();
        else
            addEventListener(new CallbackMethod<GSM>(this, &GSM::initRequests), new ConditionMethod<GSM>(this, &GSM::moduleCheck), true);
        SIM800Serial.begin(4800);
        delay(100);
    #else
        addEventListener(new CallbackMethod<GSM>(this, &GSM::initRequests), new ConditionMethod<GSM>(this, &GSM::moduleCheck), true);
    #endif

    timer = 0;
}

bool GSM::moduleCheck()
{
    #ifdef BUILD_PAXO
    while(SIM800Serial.available())
    {
        char c = SIM800Serial.read();
        data.push_back(c);
        Serial.write(c);
    }

        if(timer + 1000 < millis())
        {
            
            if(data.find("SMS Ready") != std::string::npos)
            {
                data="";
                print ("module ok");
                return true;
            }
            else
                print ("not responding...:" + data);
            timer = millis();
            
            //SIM800Serial.end();
            //SIM800Serial.begin(4800);
            SIM800Serial.print("AT\r\n");
        }
        else
            return false;
    #endif
    #ifdef BUILD_EMU
    return true;
    #endif

    return false; // in emulator, sim800 is not available
}

void GSM::initRequests()
{
    #ifdef BUILD_PAXO
    delay(100);
    SIM800Serial.print(F("AT+CMGF=1\r\n"));
    delay(100);
    SIM800Serial.print(F("AT+CNMI=1,1,0,0,0\r\n"));
    delay(100);
    SIM800Serial.print(F("AT+CLTS=1\r\n"));
    delay(100);
    SIM800Serial.print(F("AT&W\r\n"));
    delay(100);
    #endif

    setInterval(new CallbackMethod<GSM>(this, &GSM::update), 2);
    setInterval(new CallbackMethod<GSM>(this, &GSM::getHour), 5000);
    setInterval(new CallbackMethod<GSM>(this, &GSM::askForMessages), 6000);
    setInterval(new CallbackMethod<GSM>(this, &GSM::getNetworkQuality), 10000);
    setInterval(new CallbackMethod<GSM>(this, &GSM::getBatteryLevel), 10000);
    
    getHour();
    askForMessages();
    getNetworkQuality();
    getBatteryLevel();

    struct Key key1 = {&GSM::askForMessagesPrio, "+CMTI", false}; add_key(key1); // message
    struct Key key2 = {&GSM::askedForCall, "RING", false}; add_key(key2); // call
}

void GSM::update()
{
    get_data();
    
    for (int i = 0; i < keys.size(); i++) // need to be fixed, if someone send a key in a sms, it will be interpreted as a key...
    {
        if(data.find(keys[i].key) != std::string::npos)
        {
            keys[i].isDetected = true;
        }
    }

    if(requests.size()!=0)
    {
        if (actual_cmd_count==0 || data.find("OK")!=std::string::npos || data.find("ERROR")!=std::string::npos || timeout+10000 < millis())
        {
            // print("Request: 1/" + to_string(requests.size()));
            if(((data.find("ERROR")==std::string::npos && data.find("OK")!=std::string::npos) && timeout+10000 > millis() && requests[0].size()!=actual_cmd_count && !break_) || actual_cmd_count==0)
            {
                (this->*requests[0][actual_cmd_count])();
                timeout=millis();
                data="";
                actual_cmd_count++;
                print("   func " + to_string(actual_cmd_count) + "/" + to_string(requests[0].size()));
            }else
            {
                print("Request ended");

                requests.erase(requests.begin());
                actual_cmd_count = 0;

                data = data.substr(0,data.find_first_of("OK\n")+3);

                for (int i = 0; i < keys.size(); i++) // need to be fixed, if someone send a key in a sms, it will be interpreted as a key...
                {
                    if(keys[i].isDetected)
                    {
                        keys[i].isDetected = false;
                        (this->*keys[i].func)();
                        print("key detected: " + keys[i].key);
                    }
                }

                break_ = false;
            }
        }
    }
}

void GSM::add_request(std::vector<void (GSM::*)(void)> requests, bool priority)
{
    if (priority) {
        // Si la priorité est vraie, nous insérons la requête à ID=1
        this->requests.insert(this->requests.begin()+1, requests);
    } else {
        // Si la priorité est fausse, nous vérifions d'abord si la requête existe déjà
        for (uint16_t i = 0; i < this->requests.size(); i++) {
            if (this->requests[i][0] == requests[0])
                return; // La requête existe déjà, nous ne l'ajoutons pas à nouveau
        }
        // Si la requête n'existe pas déjà, nous l'ajoutons à la fin
        this->requests.push_back(requests);
    }
}

void GSM::add_key(Key key)
{
    keys.push_back(key);
}

void GSM::write(char c)
{
    #ifdef BUILD_PAXO
    SIM800Serial.write(c);
    Serial.write(c);
    #endif
}

void GSM::gsm_print(std::string s)
{
    for (int i = 0; i < s.length(); i++)
        write(s[i]);
}

void GSM::get_data()
{
    #ifdef BUILD_PAXO
    while(SIM800Serial.available())
    {
        char c = SIM800Serial.read();
        data.push_back(c);
        Serial.write(c);
    }    
    #endif
}

void GSM::askForMessages()
{
    #ifdef BUILD_PAXO
    add_request({&GSM::getNewMessagesMODE, &GSM::getNewMessagesGET, &GSM::getNewMessagesPARSE, &GSM::getNewMessagesClear});
    #endif
}

void GSM::askForMessagesPrio()
{
    #ifdef BUILD_PAXO
    add_request({&GSM::getNewMessagesMODE, &GSM::getNewMessagesGET, &GSM::getNewMessagesPARSE, &GSM::getNewMessagesClear}, 1);
    #endif
}

void GSM::getNewMessagesMODE()
{
    this->gsm_print("AT+CMGF=1\r\n");
    print("getNewMessagesMODE");
    
}

void GSM::getNewMessagesGET()
{
    this->gsm_print("AT+CMGL=\"REC UNREAD\"\r\n");
    print("getNewMessagesGET");
}

void GSM::getNewMessagesPARSE()
{
    std::vector<Message> messages;

    print("==={\n"+data+"}===");

    for (int64_t i = 0; i < data.size();) {
        std::string number, message, date;
        int64_t j = data.find("+CMGL:", i);

        print("id: " + to_string(j));

        if (j == std::string::npos)
        {
            break;
        }

        int64_t k = data.find("\"", j);
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
        this->gsm_print("AT+CMGDA=\"DEL ALL\"\r\n");
    }
    else
    {
        print("[GSM] I: No message received");
        break_ = true;
    }
}

void GSM::getNewMessagesClear()
{

}

void GSM::sendNewMessageMODE(std::string number, std::string message)
{
    #ifdef BUILD_PAXO
        print("[GSM] I: message to " + number);
        locked = true;
        this->number_buffer = number;
        this->message_buffer = message;
        add_request({&GSM::sendNewMessageRequest}, true);
    #else
        print("[GSM EMU] I: message to " + number);
        Message messageToSend = {number, message, std::to_string(days) + "/" + std::to_string(months) + "/" + std::to_string(years)};
        gsm.saveMessages({messageToSend});
    #endif
}

void GSM::sendNewMessageRequest()
{
    #ifdef BUILD_PAXO
        data="";

        this->gsm_print("AT+CMGF=1\r\n");
        
        while(data.find("\nOK")==std::string::npos && data.find("\nERROR")==std::string::npos)
            get_data();

        if(data.find("\nERROR")!=std::string::npos)
        {
            print("[GSM] E: Can't send message");
            data="";
            return;
        }
        
        print("[GSM] I: sending number to module...");
        data="";
        this->gsm_print("AT+CMGS=\"" + this->number_buffer + "\"\r");

        delay(10);

        print("[GSM] I: sending message to module...");
        
        data="";

        this->gsm_print(this->message_buffer + (char)26);

        while(data.find("\nOK")==std::string::npos && data.find("\nERROR")==std::string::npos)
            get_data();
        

        if(data.find("\nOK")!=std::string::npos)
            print("[GSM] I: Message sent!");
        else
            print("[GSM] I: Message not sent...");
    #endif
}

void GSM::askedForCall()
{
    add_request({&GSM::getNumberWhoCall, &GSM::showCall});
}

void GSM::getNumberWhoCall()
{
    if(call==true) return
    this->gsm_print("AT+CLCC\r\n");
}

void GSM::showCall()
{
    if(call)
        return;
    
    call=true;

    std::string number = "";

    if(data.find("+CLCC:") != std::string::npos)
    {
        int64_t k = data.find("+CLCC:");
        k = data.find("\"", k);

        number = data.substr(
            k+1,
            data.find("\"", k+1) - k-1
        );
        data="";
    }

    print("number " + number + "is calling...");

    numberiscalling=number;
    
    Phone::get_a_call();

    call=false;

    if (getCall != nullptr)
    {
        getCall(number);
    }
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
    if(data.find("NO CARRIER") != std::string::npos)
    {
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

void GSM::getHour()
{
    #ifdef BUILD_PAXO
    add_request({&GSM::askForHour, &GSM::parseHour});
    #endif
    #ifdef BUILD_EMU
    time_t nowTime = time(0);
    parseHourFromComputer(&nowTime);
    #endif
    print("getHour");
}

#ifdef BUILD_EMU
void GSM::parseHourFromComputer(time_t* time) {
    struct tm* formattedTime;
    formattedTime = gmtime(time);

    localtime_r(time, formattedTime);

    // https://cplusplus.com/reference/ctime/tm/
    years = formattedTime->tm_year + 1900;
    months = formattedTime->tm_mon + 1; // 0-11
    days = formattedTime->tm_mday + 1; // 0-6
    hours = formattedTime->tm_hour; // 0-23
    minutes = formattedTime->tm_min; // 0-59
    seconds = formattedTime->tm_sec; // 0-60 "tm_sec is generally 0-59. The extra range is to accommodate for leap seconds in certain systems."
}
#endif

void GSM::askForHour()
{
    this->gsm_print("AT+CCLK?\r\n");
}

void GSM::parseHour()
{
    print("parseHour");
    if(data.find("\"") == std::string::npos)
    {
        data = "";
        return;
    }
    std::string data2 = data.substr(data.find("\"") + 1, data.find_last_of("\"") - 1 - data.find("\"") - 1);

    print("data:" + data2);
    years = atoi(data2.substr(0, 2).c_str());
    months = atoi(data2.substr(3, 5-3).c_str());
    days = atoi(data2.substr(6, 8-6).c_str());
    hours = atoi(data2.substr(9, 11-9).c_str());
    minutes = atoi(data2.substr(12, 14-12).c_str());
    seconds = atoi(data2.substr(15, 17-15).c_str());
    print(to_string(years) + " " + to_string(months) + " " + to_string(days) + " " + to_string(hours) + " " + to_string(minutes) + " " + to_string(seconds));
}

void GSM::getNetworkQuality()
{
    #ifdef BUILD_PAXO
    add_request({&GSM::askNetworkQuality, &GSM::parseNetworkQuality});
    #endif
    #ifdef BUILD_EMU
    quality = 4;
    #endif
}

void GSM::askNetworkQuality()
{
    this->gsm_print("AT+CSQ\r\n");
}

void GSM::parseNetworkQuality()
{
    quality = atoi(data.substr(data.find("+CSQ: ") + 5, data.find(",") - data.find("+CSQ: ") - 5).c_str());

    if (quality > 19) // excellent quality
        quality = 4;
    else if (quality > 14) // Good quality
        quality = 3;
    else if (quality > 9) // ok quality
        quality = 2;
    else if (quality > 0) // Marginal quality
        quality = 1;
}

void GSM::getBatteryLevel()
{
    #ifdef BUILD_PAXO
    add_request({&GSM::askBatteryLevel, &GSM::parseBatteryLevel});
    #endif
    #ifdef BUILD_EMU
    batteryLevel = 4;
    #endif
}

void GSM::askBatteryLevel()
{
    this->gsm_print("AT+CBC\r\n");
}

void GSM::parseBatteryLevel()
{
    std::string data2 = data.substr(data.find("+CBC:"), data.find_last_of(",") - data.find("+CBC:"));
    batteryLevel = atoi(data2.substr(data2.find(",") + 1, data2.find_last_of(",") - data2.find(",") - 1).c_str());

    if (batteryLevel > 90) // excellent quality
        batteryLevel = 4;
    else if (batteryLevel > 75) // Good quality
        batteryLevel = 3;
    else if (batteryLevel > 40) // ok quality
        batteryLevel = 2;
    else if (batteryLevel > 20) // Marginal quality
        batteryLevel = 1;
    else
        batteryLevel = 0;
}
