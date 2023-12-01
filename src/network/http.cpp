#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
    #include <HTTPClient.h>
    #include <WiFi.h>
#endif

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    #include <iostream>
    #include <curl/curl.h>
#endif

#include <string>
#include <stdint.h>

#include "http.hpp"

namespace network
{
    HttpClient::HttpClient() 
    {
        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            curl_global_init(CURL_GLOBAL_ALL);
            curl = curl_easy_init();
        #endif
    }

    HttpClient::~HttpClient() 
    {
        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            if (curl) 
            {
                curl_easy_cleanup(curl);
                curl_global_cleanup();
            }
        #endif
    }

    std::string HttpClient::get(const std::string& url) 
    {
        #ifdef ESP32
        
            if (network::isConnected() == false) // non connecté
                return ""; 

            HTTPClient http;
            http.begin(url.c_str());

            int httpCode = http.GET();

            if (httpCode == HTTP_CODE_OK) 
            {
                std::string payload = http.getString().c_str();
                http.end();
                return payload;
            } 

            http.end();
            return "";

        #endif

        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

            if (!curl)
                return "";

            std::string response;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);

            if (res != CURLE_OK) 
            {
                std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
                return "";
            }

            return response;

        #endif

        return "";
    }

};