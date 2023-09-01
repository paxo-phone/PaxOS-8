#ifndef LUA_HPP
#define LUA_HPP

#include "../src/lua.hpp"
#include <stdlib.h>
#include <string>
using namespace std;

#include "../widgets/gui.hpp"


int customPrint(lua_State* L) {
    int nargs = lua_gettop(L);

    for (int i = 1; i <= nargs; ++i) {
        const char* str = lua_tostring(L, i);
        if (str) {
            //print(str);
        }
    }
    return 0;
}

bool runLuaFunction(lua_State* L, const std::string& functionName) {
    lua_getglobal(L, functionName.c_str());
    
    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, LUA_MULTRET, 0) == 0) {
            // Lua function executed successfully, and output is captured in luaOutput.
            return true;
        } else {
            //std::cerr << "Error executing Lua function '" << functionName << "': " << lua_tostring(L, -1) << std::endl;
        }
    } else {
        //std::cerr << "Lua function '" << functionName << "' not found." << std::endl;
    }

    return false;
}

void addFunction(lua_State* L, std::string name, int (*function)(lua_State* L))
{
    lua_pushcfunction(L, function);
    lua_setglobal(L, name.c_str());
}

void addVariable(lua_State* L, std::string name, int value)
{
    lua_pushinteger(L, value);
    lua_setglobal(L, name.c_str());
}

void* custom_allocator(void *ud, void *ptr, size_t osize, size_t nsize) {
    if (nsize == 0) {
        // Free the block
        if (ptr != NULL) {
            free(ptr);
        }
        return NULL;
    } else {
        // Allocate or resize the block
        #ifdef BUILD_PAXO
            return ps_realloc(ptr, nsize);
        #else
            return realloc(ptr, nsize);
        #endif
    }
}

struct LuaEvent
{
    string callback;
    Gui* obj;
    bool (Gui::*func)(void);
};

class LuaInterpreter
{
    public:

    std::string conf = "";
    std::string data = "";
    static uint idcounter;
    static vector<Gui*> gui;
    static vector<LuaEvent> events;

    void loadScript(std::string filename);
    void runApp();

    static int luaNewObject(lua_State* L);
    static void setColorInit(lua_State* L);
    static int setX(lua_State* L);
    static int setY(lua_State* L);
    static int setWidth(lua_State* L);
    static int setHeight(lua_State* L);
    static int getX(lua_State* L);
    static int getY(lua_State* L);
    static int getWidth(lua_State* L);
    static int getHeight(lua_State* L);
    static int setColor(lua_State* L);
    static int setText(lua_State* L);
    static int onClick(lua_State* L);
};

uint LuaInterpreter::idcounter;
vector<Gui*> LuaInterpreter::gui;
vector<LuaEvent> LuaInterpreter::events;


#endif