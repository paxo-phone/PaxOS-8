#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>
#include "../web.hpp"

#ifdef ESP32

#include <HTTPClient.h>
#include <WiFi.h>
#include <string>

class HttpClient {
public:
    std::string get(const std::string& url) {
        if (!webdriver.isConnected()) {
            //print("Not connected to Internet");
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
            //print("HTTP request failed with error code " + to_string(httpCode));
            http.end();
            return "";
        }
    }
};

#endif
#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

#include <iostream>
#include <string>
#include <curl/curl.h>

class HttpClient {
public:
    HttpClient() {
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
    }

    ~HttpClient() {
        if (curl) {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
        }
    }

    std::string get(const std::string& url) {
        if (!curl) {
            return "";
        }

        std::string response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
            return "";
        }

        return response;
    }

private:
    CURL* curl;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t total_size = size * nmemb;
        output->append(static_cast<char*>(contents), total_size);
        return total_size;
    }
};

#endif