#ifndef LUA_HPP
#define LUA_HPP

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#include "../lib/lua/lua.hpp"
#include <stdlib.h>
#include <string>
#include <map>
#include <iostream>

#define LUA_MAIN_FUNCTION "run"

#define GARBAGE_COLLECT "__gc"
#define INDEX_METATABLE "__index"

#define LUA_WINDOW_ERR "Currently no instances of window!"
#define LUA_FUNC_ERR "Bad arguments!"
#define METATABLE_WIN_GUI    "wingui"
#define METATABLE_BOX_GUI    "boxgui"
#define METATABLE_LABEL_GUI  "labelgui"
#define METATABLE_BTN_GUI    "btngui"
#define METATABLE_IMG_GUI    "imggui"
#define METATABLES_GUI {METATABLE_WIN_GUI, METATABLE_BOX_GUI, METATABLE_LABEL_GUI, METATABLE_BTN_GUI, METATABLE_IMG_GUI}

using namespace std;

#include "../widgets/gui.hpp"

// Helpers
Gui *get_checked_gui(lua_State* L, int idx);
void* custom_allocator(void *ud, void *ptr, size_t osize, size_t nsize);
void execute_lua(lua_State* L, const std::string& functionName);

// Represent a LuaEvent, mainly used to set callbacks in lua world
typedef struct LuaEvent {
    Gui* ref;
    int callback_ref;
} LuaEvent;

class LuaEventInterval
{
    public:
    LuaEventInterval(lua_State *L, int callback_ref, int interval);
    ~LuaEventInterval();
    int id;
    int callback_ref;
    lua_State *L;
    void call(void);
};

class LuaEventTimeOut
{
    public:
    LuaEventTimeOut(lua_State *L, int callback_ref, int timer);
    ~LuaEventTimeOut();
    int id;
    int callback_ref;
    lua_State *L;
    void call(void);
};

class LuaInterpreter {
    private:
        static Window* current_root;
        std::string data = "";
        static std::string dir;
        static vector<LuaEvent> events;
        static vector<LuaEventInterval*> intervals;
        static vector<LuaEventTimeOut*> timeOuts;
        static uint64_t timerFromStart;
        static void fill_gui_metatable(lua_State* L, const char* table_name, lua_CFunction f, const luaL_Reg *l);

    public:
        LuaInterpreter(string dir);
        void loadScript(std::string filename);
        void runApp();

        // Lua provided to lua    
        static int setWindow(lua_State* L);
        static int delWindow(lua_State* L); // pas encore au point -> ne supprime pas les events associés pour le moment
        static int getWindow(lua_State* L);
        
        static int window(lua_State* L);
        static int box(lua_State* L);
        static int label(lua_State* L);
        static int button(lua_State* L);
        static int canvas(lua_State* L);
        static int image(lua_State* L);

        static int setText(lua_State* L);   // Label
        static int getText(lua_State* L);
        static int setEditable(lua_State* L);
        static int setFontSize(lua_State* L);
        /*static int setFontSize(lua_State* L);*/

        static int fillRect(lua_State* L); // Canvas
        static int push(lua_State* L);

        static int setX(lua_State* L);      // style
        static int setY(lua_State* L);
        static int setWidth(lua_State* L);
        static int setHeight(lua_State* L);
        static int getX(lua_State* L);
        static int getY(lua_State* L);
        static int getWidth(lua_State* L);
        static int getHeight(lua_State* L);
        static int setColor(lua_State* L);
        static int isFocused(lua_State* L);

        static int onClick(lua_State* L); // Gui event

        static int readFile(lua_State* L);
        static int writeFile(lua_State* L);
        static int renameFile(lua_State* L);
        static int deleteFile(lua_State* L);
        static int newDir(lua_State* L);
        static int renameDir(lua_State* L);
        static int deleteDir(lua_State* L);
        static int isDir(lua_State* L);
        static int isFile(lua_State* L);
        static int listDir(lua_State* L);
        static int specialSleep(lua_State* L);
        static int setInterval(lua_State* L);
        static int setTimeOut(lua_State* L);
        static int monotonic(lua_State* L);
        static int getTime(lua_State* L);
        static int getWeb(lua_State* L);

        static LuaEventTimeOut* timeOutToRemove;
};

// Gui commands bindings
static const luaL_Reg gui_common_binds[] = {
    {"setX",       LuaInterpreter::setX,},
    {"getX",       LuaInterpreter::getX},
    {"setY",       LuaInterpreter::setY},
    {"getY",       LuaInterpreter::getY},
    {"setWidth",   LuaInterpreter::setWidth},
    {"getWidth",   LuaInterpreter::getWidth},
    {"setHeight",  LuaInterpreter::setHeight},
    {"getHeight",  LuaInterpreter::getHeight},
    {"setColor",   LuaInterpreter::setColor},
    {"setText",    LuaInterpreter::setText},
    {"fillRect",   LuaInterpreter::fillRect},
    {"push",       LuaInterpreter::push},
    {"onClick",    LuaInterpreter::onClick},
    {"setEditable",LuaInterpreter::setEditable},
    {"getText",    LuaInterpreter::getText},
    {"setFontSize",LuaInterpreter::setFontSize},
    {"isFocused",  LuaInterpreter::isFocused},
    {NULL, NULL}
};

// Library bindings
static const luaL_Reg paxolib[] = {
    {"setWindow",   LuaInterpreter::setWindow},
    {"delWindow",   LuaInterpreter::delWindow},
    {"getWindow",   LuaInterpreter::getWindow},
    {"window",      LuaInterpreter::window},
    {"box",         LuaInterpreter::box},
    {"label",       LuaInterpreter::label},
    {"button",      LuaInterpreter::button},
    {"canvas",      LuaInterpreter::canvas},
    {"image",       LuaInterpreter::image},
    {"sleep",       LuaInterpreter::specialSleep},
    {"readFile",    LuaInterpreter::readFile},
    {"writeFile",   LuaInterpreter::writeFile},
    {"newDir",      LuaInterpreter::newDir},
    {"renameDir",   LuaInterpreter::renameDir},
    {"deleteDir",   LuaInterpreter::deleteDir},
    {"isDir",       LuaInterpreter::isDir},
    {"isFile",      LuaInterpreter::isFile},
    {"renameFile",  LuaInterpreter::renameFile},
    {"deleteFile",  LuaInterpreter::deleteFile},
    {"listDir",     LuaInterpreter::listDir},
    {"setInterval", LuaInterpreter::setInterval},
    {"setTimeOut",  LuaInterpreter::setTimeOut},
    {"monotonic",   LuaInterpreter::monotonic},
    {"getTime",     LuaInterpreter::getTime},
    {"getWeb",      LuaInterpreter::getWeb},
    
    /* placeholders */
    {"COLOR_LIGHT",     NULL},
    {"COLOR_BLACK",     NULL},
    {"COLOR_PRIMARY",   NULL},
    {"COLOR_SUCCESS",   NULL},
    {"COLOR_WHITE",     NULL},
    {NULL, NULL}
};

static const std::map<std::string, int> color_bindings = {
    {"COLOR_LIGHT",     COLOR_LIGHT},
    {"COLOR_BLACK",     COLOR_BLACK},
    {"COLOR_PRIMARY",   COLOR_PRIMARY},
    {"COLOR_SUCCESS",   COLOR_SUCCESS},
    {"COLOR_WHITE",     COLOR_LIGHT},
    {"AUTO",            AUTO}
};

// Making our own primitives available to lua by adding it as a library.
LUAMOD_API int luaopen_paxolib(lua_State* L);

#endif
