#ifndef SHELL_HPP
#define SHELL_HPP

#include "../includes.h"

#include <iostream>
#include <string>
#include <vector>

#include "filestream.hpp"

class SerialIO
{
    virtual void print(string str, bool newLine = true) = 0;
    virtual void print(char str) = 0;
    virtual string input() = 0;
};

class CommandShell : public SerialIO
{
    public:
    void print(string str, bool newLine = true)
    {
        #ifdef BUILD_EMU
            std::cout << str;
            if (newLine)
            {
                std::cout<<std::endl;
            }
        #endif
        #ifdef BUILD_PAXO
            for (int i = 0; i < str.size(); i++)
                Serial.print(str[i]);
            if(newLine)
                Serial.println();
        #endif
    }

    void print(char str)
    {
        #ifdef BUILD_EMU
            std::cout << str;
        #endif
        #ifdef BUILD_PAXO
            Serial.print(str);
        #endif
    }

    string input()
    {
        string line;

        #ifdef BUILD_EMU
            cout << ">> ";
            getline(cin, line);
        #endif
        #ifdef BUILD_PAXO
            while (!Serial.available())
                delay(10);
            String str = Serial.readString();
            line = std::string(str.c_str());
        #endif

        return line;
    }
};

extern CommandShell command_shell;

void print(string str);
void print(char str);
string input();

using namespace std;

#undef ERROR

namespace shell
{
    typedef vector<string> ArgList;

    enum
    {
        SUCCESS = 0,
        ERROR   = 1,
    };

    void init();

    void execute(const string& line);


    /* commands */

    int cmd_echo(const ArgList& args);

    int cmd_ls(const ArgList& args);
    int cmd_cd(const ArgList& args);
    int cmd_cat(const ArgList& args);
    int cmd_event(const ArgList& args);
    int cmd_help(const ArgList& args);
    int cmd_reboot(const ArgList& args);
    int cmd_getMessages(const ArgList& args);
    int cmd_pressHomeButton(const ArgList& args);
    int addMessage(const ArgList& args);

    extern std::string cmd_str[];

    extern int (*cmd_func[])(const ArgList&);

    extern std::string currentDirectory;
                                        
    /* end commands */

    ArgList tokenize(const string& line, char delimiter = ' ');

    int call(string command, const shell::ArgList& arguments);
};

#include "../tasks/tasks.hpp"

extern uint64_t heap;
extern uint64_t nh;

extern uint64_t pheap;
extern uint64_t pnh;

/*#include <esp_system.h>
#include <esp_spi_flash.h>
#include <esp_heap_caps.h>
*/

void thread_shell(void* data);
#endif
