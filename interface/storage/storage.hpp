#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <stdlib.h>
#include <vector>
using namespace std;

#ifdef ESP32
    #include <SPIFFS.h>
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <windows.h>
#elif defined(__APPLE__) || defined(__linux__)
    #include <iostream>
    #include <fstream>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

namespace Storage
{
    class LFile
    {
        public:
        enum Mode
        {
            R, // read only
            W  // read and write
        };

        LFile(std::string filename, Mode mode = R);
        ~LFile() { close(); }

        bool open();
        void close();

        std::string read(int from = 0, int to = -1);
        
        void setCursor(int pos = -1);
        void write(std::string data);

        private:
        std::string filename;
        std::string data;

#ifdef ESP32
        File file;
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        HANDLE fileHandle;
#elif defined(__APPLE__) || defined(__linux__)
        std::fstream fileStream;
#endif
    };

    std::vector<std::string> listFiles(std::string dir);
    bool mkdir(std::string dir);

    bool isDir(std::string dir);
    bool isFile(std::string dir);

    bool mv(std::string from, std::string to); // move or rename a directory or a file
    bool rm(std::string path); // remove a directory or a file;
};

#endif
