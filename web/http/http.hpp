#include <HTTPClient.h>
#include <WiFi.h>
#include <string>

class HttpClient {
public:
    std::string get(const std::string& url) {
        if (!webdriver.isConnected()) {
            print("Not connected to Internet");
            return "";
        }

        HTTPClient http;
        http.begin(url.c_str());

        int httpCode = http.GET();

        if (httpCode == HTTP_CODE_OK) {
            std::string payload = http.getString().c_str();
            http.end();
            return payload;
        } else {
            print("HTTP request failed with error code " + to_string(httpCode));
            http.end();
            return "";
        }
    }
};