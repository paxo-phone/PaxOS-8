#ifndef HTTP_HPP
#define HTTP_HPP

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

namespace network
{
    class HttpClient 
    {
        public:

            HttpClient();
            ~HttpClient();

            std::string get(const std::string& url);
            // std::string post(const std::string& url);

        private:

            #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
                CURL* curl;
                static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
                {
                    size_t total_size = size * nmemb;
                    output->append(static_cast<char*>(contents), total_size);
                    return total_size;
                }
            #endif

    };
}

#endif /* HTTP_HPP */