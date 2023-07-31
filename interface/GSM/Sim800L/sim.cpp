#include "sim.hpp"

void GSM::init()
{
    #ifdef BUILD_PAXO
    pinMode(32, OUTPUT);
    SIM800Serial.begin(9600);


    addEventListener(new CallbackMethod<GSM>(this, &GSM::initRequests), new ConditionMethod<GSM>(this, &GSM::moduleCheck), true);
    #endif
}

bool GSM::moduleCheck()
{
    if(timer+1000 < millis())
    {
        SIM800Serial.print(F("AT\r\n"));

        uint32_t t = millis();
        
        while(!SIM800Serial.available())
        {
            if(t + 200 <= millis())
            {
                print("not responding");
                timer = millis();
                return false;
            }
        }
        return true;        
    }
    else
        return false;
}

void GSM::initRequests()
{
    #ifdef BUILD_PAXO
    delay(10);
    SIM800Serial.print(F("AT+CNMI=2,1,0,0,0\r\n"));
    delay(10);
    SIM800Serial.print(F("AT+CLTS=1\r\n"));
    delay(10);
    SIM800Serial.print(F("AT+CMGF=1\r\n"));
    delay(10);
    SIM800Serial.print(F("AT&W\r\n"));
    delay(10);
    #endif

    setInterval(new CallbackMethod<GSM>(this, &GSM::update), 2);
    setInterval(new CallbackMethod<GSM>(this, &GSM::getHour), 5000);
    setInterval(new CallbackMethod<GSM>(this, &GSM::askForMessages), 6000);
    setInterval(new CallbackMethod<GSM>(this, &GSM::getNetworkQuality), 10000);
    setInterval(new CallbackMethod<GSM>(this, &GSM::getBatteryLevel), 10000);

    struct Key key1 = {&GSM::askForMessages, "+CMTI\n", false}; add_key(key1); // message
    struct Key key2 = {&GSM::getNumberWhoCall, "RING\n", false}; add_key(key2); // call
}

void GSM::update()
{
    get_data();

    for (int i = 0; i < keys.size(); i++)
    {
        if(data.find(keys[i].key) != -1)
            keys[i].isDetected = true;
    }

    if(requests.size()!=0)
    {
        if (actual_cmd_count==0 || data.find("OK")!=-1 || data.find("ERROR")!=-1 || timeout+10000 < millis())
        {
            print("Request: 1/" + to_string(requests.size()));
            if(((data.find("ERROR")==-1 && data.find("OK")!=-1) && timeout+10000 > millis() && requests[0].size()!=actual_cmd_count) || actual_cmd_count==0)
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

                for (int i = 0; i < keys.size(); i++)
                {
                    if(keys[i].isDetected)
                    {
                        keys[i].isDetected = false;
                        (this->*keys[i].func)();
                    }
                }
            }
        }
    }
}

void GSM::add_request(std::vector<void (GSM::*)(void)> requests)
{
    for (uint i = 0; i < this->requests.size(); i++)
        if(this->requests[i][0] == requests[0])
            return;
    this->requests.push_back(requests);
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
    add_request({&GSM::getNewMessagesMODE, &GSM::getNewMessagesGET, &GSM::getNewMessagesPARSE});
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
}

void GSM::sendNewMessageMODE(std::string number, std::string message)
{
    print("[GSM] I: message to " + number);
    locked = true;
    this->number_buffer = number;
    this->message_buffer = message;

    add_request({&GSM::sendNewMessageRequest});
}

void GSM::sendNewMessageRequest()
{
    data="";

    this->gsm_print("AT+CMGF=1\r\n");
    
    while(data.find("\nOK")==-1 && data.find("\nERROR")==-1)
        get_data();

    if(data.find("\nERROR")!=-1)
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

    while(data.find("\nOK")==-1 && data.find("\nERROR")==-1)
        get_data();
    

    if(data.find("\nOK")!=-1)
        print("[GSM] I: Message sent!");
    else
        print("[GSM] I: Message not sent...");
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
    if(data.find("NO CARRIER") != -1)
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
    add_request({&GSM::askForHour, &GSM::parseHour});
    print("getHour");
}

void GSM::askForHour()
{
    this->gsm_print("AT+CCLK?\r\n");
}

void GSM::parseHour()
{
    print("parseHour");
    if(data.find("\"") == -1)
    {
        data = "";
        return;
    }
    std::string data2 = data.substr(data.find("\"") + 1, data.find_last_of("\"") - 1 - data.find("\"") - 1);

    print("data:" + data2);
    years = stoi(data2.substr(0, 2));
    months = stoi(data2.substr(3, 5-3));
    days = stoi(data2.substr(6, 8-6));
    hours = stoi(data2.substr(9, 11-9));
    minutes = stoi(data2.substr(12, 14-12));
    seconds = stoi(data2.substr(15, 17-15));
    print(to_string(years) + " " + to_string(months) + " " + to_string(days) + " " + to_string(hours) + " " + to_string(minutes) + " " + to_string(seconds));
}

void GSM::getNetworkQuality()
{
    add_request({&GSM::askNetworkQuality, &GSM::parseNetworkQuality});
}

void GSM::askNetworkQuality()
{
    this->gsm_print("AT+CSQ\r\n");
}

void GSM::parseNetworkQuality()
{
    quality = stoi(data.substr(data.find("+CSQ: ") + 5, data.find(",") - data.find("+CSQ: ") - 5));

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
    add_request({&GSM::askBatteryLevel, &GSM::parseBatteryLevel});
}

void GSM::askBatteryLevel()
{
    this->gsm_print("AT+CBC\r\n");
}

void GSM::parseBatteryLevel()
{
    std::string data2 = data.substr(data.find("+CBC:"), data.find_last_of(",") - data.find("+CBC:"));
    batteryLevel = stoi(data2.substr(data2.find(",") + 1, data2.find_last_of(",") - data2.find(",") - 1));

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