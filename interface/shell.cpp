#include "shell.hpp"

#include "GSM/GSM.hpp"

int shell::cmd_ls(const ArgList& args)
{
    vector<string> ls = storage::listdir(args[0]);
    for (int i = 0; i < ls.size(); i++)
    {
        print(ls[i]);
    }
    return SUCCESS;
}

int shell::cmd_cd(const ArgList& args)
{
    // to fill
    return SUCCESS;
}

int shell::cmd_cat(const ArgList& args)
{
    if(args.size() != 1)
    {
        print("Usage: cat filename");
        return ERROR;
    }
    storage::LFile file(args[0], storage::OPEN_MODE::READ, false);
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