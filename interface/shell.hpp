#ifndef SHELL_HPP
#define SHELL_HPP

#include <iostream>
#include <string>
#include <vector>

#include "memory.hpp"

void print(string str)
{
    #ifdef BUILD_EMU
        std::cout << str << std::endl;
    #endif
    #ifdef BUILD_PAXO
        for (int i = 0; i < str.size(); i++)
            Serial.print(str[i]);
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
        String str = Serial.readString();
        line = std::string(str.c_str());
    #endif

    return line;
}

using namespace std;

namespace shell
{
    typedef vector<string> ArgList;

    enum
    {
        SUCCESS = 0,
        ERROR   = 1,
    };

    void init()
    {
        #ifdef BUILD_PAXO
            Serial.begin(115200);
        #endif
    }

    /* commands */

    int cmd_echo(const ArgList& args)
    {
        const char delimiter = ' ';
        for(int i = 0; i < args.size(); i++)
        {
            cout << args[i];
            if (i != args.size() - 1)
                print(delimiter);
            else
                print('\n');
        }

        return SUCCESS;
    }

    int cmd_ls(const ArgList& args);
    int cmd_cd(const ArgList& args);
    int cmd_cat(const ArgList& args);
    int cmd_event(const ArgList& args);
    int cmd_help(const ArgList& args);
    int cmd_reboot(const ArgList& args);

    string cmd_str[] = { "echo", 
                         "ls", 
                         "cd", 
                         "cat", 
                         "event",
                         "help",
                         "reboot" };

    int (*cmd_func[])(const ArgList&) = { &cmd_echo, 
                                          &cmd_ls, 
                                          &cmd_cd, 
                                          &cmd_cat, 
                                          &cmd_event,
                                          &cmd_help,
                                          &cmd_reboot };
    /* end commands */

    ArgList tokenize(const string& line, char delimiter = ' ')
    {
        ArgList toklist;

        string tok;
        for(int i = 0; i < line.size(); i++)
        {
            
            if(line[i] == delimiter)
            {
                if(!tok.empty())
                {
                    toklist.push_back(tok);
                    tok.clear();
                }

                continue;
            }

            if(line[i] == ';')
            {
                if(!tok.empty())
                {
                    toklist.push_back(tok);
                    tok.clear();
                }
            }

            if(line[i] == '"')
            {
                i++;
                while(i < line.size() && line[i] != '"')
                    tok += line[i++];

                continue;
            }

            if(line[i] == '\'')
            {
                i++;
                while(i < line.size() && line[i] != '\'')
                    tok += line[i++];

                continue;
            }

            tok += line[i];
        }
    
        if(!tok.empty())
            toklist.push_back(tok);
    
        return toklist;
    }

    int call(string command, const ArgList& arguments)
    {
        for(int i = 0; i < sizeof(cmd_str)/sizeof(string); i++)
        {
            if(command == cmd_str[i])
            {
                return (*cmd_func[i])(arguments);
            }
        }

        // unknown command
        print(string(command + ": command not found\n"));
        return ERROR;
    }

    void execute(const string& line)
    {
        if(line.empty())
            return;

        ArgList tokens = tokenize(line);
        tokens.push_back(";");
        
        string command;
        ArgList arguments;

        int status = SUCCESS;
        
        for(int i = 0; i < tokens.size(); i++)
        {
            if(
                tokens[i] == ";"  ||
                tokens[i] == "&&" ||
                tokens[i] == "||"
               )
            {
                if(!command.empty())
                {
                    status = call(command, arguments);

                    command.clear();
                    arguments.clear();
                }

                if(tokens[i] == "||")
                {
                    if(status == SUCCESS)
                        break;
                }

                if(tokens[i] == "&&")
                {
                    if(status == ERROR)
                        break;
                }

                continue;
            }

            if(command.empty())
            {
                command = tokens[i];
            }
            else
            {
                arguments.push_back(tokens[i]);
            }
        }
    }
};

void thread_shell(void* data)
{
    while(true)
    {
        shell::execute(input());
    }
}

#endif