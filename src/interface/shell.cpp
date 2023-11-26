#include <string>
#include <vector>

#include "console.hpp"

#include "shell.hpp"
#include "filestream.hpp"
#include "GSM/GSM.hpp"

namespace shell
{
    enum
    {
        STATUS_SUCCESS = 0,
        STATUS_ERROR   = 1
    };

    typedef std::string Arg;
    typedef std::vector<Arg> ArgList;

    std::string currentDirectory = "/";

    std::string cmd_str[] = { 
        "echo",
        "ls",
        "cd",
        "cat",
        "event",
        "help",
        "reboot",
        "messages",
        "newmsg" };


    int cmd_echo       (const ArgList& _);
    int cmd_ls         (const ArgList& _);
    int cmd_cd         (const ArgList& _);
    int cmd_cat        (const ArgList& _);
    int cmd_event      (const ArgList& _);
    int cmd_help       (const ArgList& _);
    int cmd_reboot     (const ArgList& _);
    int cmd_getMessages(const ArgList& _);
    int cmd_addMessage (const ArgList& _);

    int (*cmd_func[])(const ArgList &) = { 
        &cmd_echo,
        &cmd_ls,
        &cmd_cd,
        &cmd_cat,
        &cmd_event,
        &cmd_help,
        &cmd_reboot,
        &cmd_getMessages,
        &cmd_addMessage };

    ArgList tokenize(const std::string& line, char delimiter = ' ')
    {
        ArgList toklist;

        Arg tok;
        for(int i = 0; i < line.size(); i++)
        {
            if(line[i] == delimiter)
            {
                if(tok.empty() == false)
                {
                    toklist.push_back(tok);
                    tok.clear();
                }

                continue;
            }

            if(line[i] == ';')
            {
                if(tok.empty() == false)
                {
                    toklist.push_back(tok);
                    tok.clear();
                }

                continue;
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

    int call(std::string command, const ArgList& arguments)
    {
        for(int i = 0; i < sizeof(cmd_str)/sizeof(std::string); i++)
        {
            if(command == cmd_str[i])
            {
                return (*cmd_func[i])(arguments);
            }
        }

        // unknown command
        console.error( command + ": command not found" );
        return STATUS_ERROR;
    }

    void eat(const std::string& line)
    {
        if(line.empty())
            return;

        ArgList tokens = tokenize(line);
        tokens.push_back(";");
        
        std::string command;
        ArgList arguments;

        int status = STATUS_SUCCESS;
        
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
                    if(status == STATUS_SUCCESS)
                        break;
                }

                if(tokens[i] == "&&")
                {
                    if(status == STATUS_ERROR)
                        break;
                }

                continue;
            }

            if(command.empty())
                command = tokens[i];
            else
                arguments.push_back(tokens[i]);
        }
    }

    /***** COMMANDS *****/


    int cmd_echo(const ArgList& args)
    {
        const char delimiter = ' ';

        for(int i = 0; i < args.size(); i++)
        {
            console.log(args[i]);
            
            // if (i != args.size() - 1)
            //     console.log(delimiter);
            // else
            //     console.log("");
        }

        return STATUS_SUCCESS;
    }

    int cmd_ls(const ArgList& args)
    {
        std::vector<std::string> list;

        if(args.size() == 0)
            list = storage::listdir(currentDirectory);
        else
        {
            list = storage::listdir(currentDirectory + "/" + args[0]);
            if(args.size() != 1)
                console.log("ls: warning: only the first argument is read");
        }

        for (int i = 0; i < list.size(); i++)
            console.log(list[i]);

        return STATUS_SUCCESS;
    }

    int cmd_cd(const ArgList& args) // does not work properly
    {
        if(args.size() != 1)
            return STATUS_ERROR;
        
        if(args[0] == "/")
        {
            currentDirectory = "/";
        }else
        {
            if(storage::isdir(currentDirectory+"/"+args[0]))
                currentDirectory += "/" + args[0];
            else
                return STATUS_ERROR;
        }

        return STATUS_SUCCESS;
    }

    int cmd_cat(const ArgList& args)
    {
        if(args.size() != 1)
        {
            console.log("usage: cat <filename>");
            return STATUS_ERROR;
        }

        storage::FileStream file(currentDirectory + "/" + args[0], storage::OPEN_MODE::READ, false);
        if(file.is_open() == false)
        {
            console.log("cat: " + args[0] + "not found");
            return STATUS_ERROR;
        }
        console.log(file.read());
        file.close();
        return STATUS_SUCCESS;
    }

    int cmd_event(const ArgList& args)
    {
        // à remplir
        return STATUS_SUCCESS;
    }

    int cmd_help(const ArgList& args)
    {
        for(int i = 0; i < sizeof(cmd_str)/sizeof(std::string); i++)
            console.log(cmd_str[i]);

        return STATUS_SUCCESS;
    }

    int cmd_reboot(const ArgList& args)
    {
        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            abort();
        #endif

        #ifdef ESP32
            ESP.restart();
        #endif
        
        return STATUS_SUCCESS;
    }

    int cmd_getMessages(const ArgList& args)
    {
        gsm.getNewMessagesMODE();
        return STATUS_SUCCESS;
    }

    int cmd_addMessage(const ArgList& args)
    {
        if(args.size()!=2 && gsm.saveMessages!=nullptr)
            return STATUS_ERROR;

        std::vector<GSM::Message> msgs;
        GSM::Message msg = {args[0], encoder.HexStringToString(args[1]), "10/11/2006"};
        msgs.push_back(msg);
        gsm.saveMessages(msgs);

        return STATUS_SUCCESS;
    }

    void thread_shell(void* data)
    {
        while(1)
        {
            std::string line = console.input();
            
            #ifdef ESP32
                eat(line.substr(0, line.length()-1));
            #else
                eat(line);
            #endif
        }

        console.log("exit...");
    }

}; // namespace shell

