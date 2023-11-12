#include "shell.hpp"

#include "../interface.hpp"
#include "GSM/GSM.hpp"

void print(string str) { command_shell.print(str + "\n" + shell::currentDirectory + "$ ", false); }
void print(char str) { command_shell.print(str); }
string input() { return command_shell.input(); }

CommandShell command_shell;

void shell::init()
{
    #ifdef BUILD_PAXO
        Serial.begin(115200);
    #endif
}

int (*shell::cmd_func[])(const ArgList &) = { &shell::cmd_echo,
    &shell::cmd_ls,
    &shell::cmd_cd,
    &shell::cmd_cat,
    &shell::cmd_event,
    &shell::cmd_help,
    &shell::cmd_reboot,
    &shell::cmd_getMessages,
    &shell::cmd_pressHomeButton,
    &shell::addMessage };
string shell::currentDirectory = "/";

std::string shell::cmd_str[] = { "echo",
    "ls",
    "cd",
    "cat",
    "event",
    "help",
    "reboot",
    "messages",
    "phb",
    "newmsg" };

shell::ArgList shell::tokenize(const string& line, char delimiter)
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

int shell::call(string command, const shell::ArgList& arguments)
{
    for(int i = 0; i < sizeof(shell::cmd_str)/sizeof(string); i++)
    {
        if(command == shell::cmd_str[i])
        {
            return (*shell::cmd_func[i])(arguments);
        }
    }

    // unknown command
    command_shell.print(string(command + ": command not found\n"));
    print("");
    return shell::ERROR;
}

void shell::execute(const string& line)
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

int shell::cmd_echo(const ArgList& args)
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

int shell::cmd_ls(const ArgList& args)
{
    vector<string> ls = storage::listdir(shell::currentDirectory+"/"+args[0]);
    for (int i = 0; i < ls.size(); i++)
    {
        print(ls[i]);
    }
    return SUCCESS;
}

int shell::cmd_cd(const ArgList& args) // does not work properly
{
    if(args.size()!=1)
        return ERROR;
    
    if(args[0] == "/")
    {
        currentDirectory = "/";
    }else
    {
        if(storage::isdir(currentDirectory+"/"+args[0]))
            currentDirectory += "/"+args[0];
        else
            return ERROR;
    }

    return SUCCESS;
}

int shell::cmd_cat(const ArgList& args)
{
    if(args.size() != 1)
    {
        print("Usage: cat filename");
        return ERROR;
    }
    storage::FileStream file(shell::currentDirectory+"/"+args[0], storage::OPEN_MODE::READ, false);
    if(file.is_open() == false)
    {
        print("Error opening file: " + args[0]);
        return ERROR;
    }
    print(file.read());
    file.close();
    return SUCCESS;
}

int shell::cmd_event(const ArgList& args)
{
    // to fill
    return SUCCESS;

}

int shell::cmd_help(const ArgList& args)
{
    // to fill
    return SUCCESS;
}

int shell::cmd_reboot(const ArgList& args)
{
    // to fill
    #ifdef BUILD_EMU
    abort();
    #endif
    #ifdef BUILD_PAXO
    ESP.restart();
    #endif
    
    return SUCCESS;
}

int shell::cmd_getMessages(const ArgList& args)
{
    gsm.getNewMessagesMODE();
    return SUCCESS;
}

int shell::cmd_pressHomeButton(const ArgList& args)
{
    home_button.state = 2;
    return SUCCESS;
}


int shell::addMessage(const ArgList& args)
{
    if(args.size()!=2 && gsm.saveMessages!=nullptr)
        return ERROR;

    std::vector<GSM::Message> msgs;
    GSM::Message msg = {args[0], encoder.HexStringToString(args[1]), "10/11/2006"};
    msgs.push_back(msg);
    gsm.saveMessages(msgs);

    return SUCCESS;
}

void thread_shell(void* data)
{
    while(true)
    {
        std::string data = input();
        #ifdef BUILD_PAXO
            shell::execute(data.substr(0, data.length()-1));
        #else
            shell::execute(data);
        #endif
        /*nh = esp_get_free_heap_size();
        if(heap != nh)
        {
            heap = nh;
            Serial.println((String)"ram:" + nh);
        }

        pnh = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        if(pheap != pnh)
        {
            pheap = pnh;
            Serial.println((String)"psram:" + pnh);
        }*/
    }
}

uint64_t heap = 0;
uint64_t nh = 0;

uint64_t pheap = 0;
uint64_t pnh = 0;
