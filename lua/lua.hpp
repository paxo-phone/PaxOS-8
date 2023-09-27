#ifndef LUA_HPP
#define LUA_HPP

#include "../src/lua.hpp"
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
#define METATABLES_GUI {METATABLE_WIN_GUI, METATABLE_BOX_GUI, METATABLE_LABEL_GUI, METATABLE_BTN_GUI}

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

class LuaInterpreter {
    private:
        static Window* current_root;
        std::string data = "";
        static std::string dir;
        static vector<LuaEvent> events;
        static void fill_gui_metatable(lua_State* L, const char* table_name, lua_CFunction f, const luaL_Reg *l);

    public:
        LuaInterpreter(string dir);
        void loadScript(std::string filename);
        void runApp();

        // Lua provided to lua    
        static int setWindow(lua_State* L);
        static int getWindow(lua_State* L);
        
        static int window(lua_State* L);
        static int box(lua_State* L);
        static int label(lua_State* L);
        static int button(lua_State* L);
        static int canvas(lua_State* L);

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
        static int fillRect(lua_State* L);
        static int push(lua_State* L);
        static int onClick(lua_State* L);

        static int readFile(lua_State* L);
        static int writeFile(lua_State* L);
        static int special_sleep(lua_State* L);
};
Window* LuaInterpreter::current_root = nullptr;
vector<LuaEvent> LuaInterpreter::events;
std::string LuaInterpreter::dir;

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
    {NULL, NULL}
};

// Library bindings
static const luaL_Reg paxolib[] = {
    {"setWindow",  LuaInterpreter::setWindow},
    {"getWindow",  LuaInterpreter::getWindow},
    {"window",     LuaInterpreter::window},
    {"box",        LuaInterpreter::box},
    {"label",      LuaInterpreter::label},
    {"button",     LuaInterpreter::button},
    {"canvas",     LuaInterpreter::canvas},
    {"sleep",      LuaInterpreter::special_sleep},
    {"readFile",   LuaInterpreter::readFile},
    {"writeFile",  LuaInterpreter::writeFile},
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
    {"COLOR_WHITE",     COLOR_BLUE}
};

// Making our own primitives available to lua by adding it as a library.
LUAMOD_API int luaopen_paxolib(lua_State* L){
    // Our datatypes!
    for (const char* name : METATABLES_GUI) luaL_newmetatable(L, name);
    luaL_newlib(L, paxolib);
    // Saving colors
    for (auto const &x : color_bindings) {
        lua_pushinteger(L, x.second);
        lua_setfield(L, -2, x.first.c_str());
    }
    return 1;
}

#endif