#include "lua.hpp"
#include "../interface/filestream.hpp"
#include "../interface/interface.hpp"

Window* LuaInterpreter::current_root = nullptr;
vector<LuaEvent> LuaInterpreter::events;
std::string LuaInterpreter::dir;

LuaInterpreter::LuaInterpreter(string dir) {
    LuaInterpreter::dir = dir;
}

void LuaInterpreter::loadScript(std::string filename) {
    storage::FileStream file(filename, storage::READ);
    data = file.read();
    file.close();
}

void LuaInterpreter::runApp() {
    // Creating lua state and adding base libraries and our own library
    lua_State* L = luaL_newstate();
    lua_setallocf(L, custom_allocator, NULL);
    luaL_openlibs(L);
    luaL_requiref(L, "paxolib", luaopen_paxolib, 1);
    lua_pop(L, 1); // Remove the library from the stack

    // Load our lua file
    if (!luaL_loadstring(L, data.c_str())) {
        if (lua_pcall(L, 0, LUA_MULTRET, 0)) {
            printf("Error executing Lua script: %s", lua_tostring(L, -1));
            return;
        }
    } else {
        printf("Error loading Lua script");
        return;
    }

    // Execute our lua app
    execute_lua(L, LUA_MAIN_FUNCTION);

    // Also checks that a window has been set!
    while(!home_button.pressed() && current_root != nullptr) {
        current_root->updateAll();
        for (LuaEvent event : events) {
            if(event.ref->isTouched()){
                lua_rawgeti(L, LUA_REGISTRYINDEX, event.callback_ref);
                lua_call(L, 0, 0);
            }
       }
    }

    // LUA_GC will take care of memory!
    lua_close(L);

    // Resets our vars
    current_root = nullptr;
    events.clear();
    data.clear();
}

// Fills metatable for GUI components
// Function also needs to set stack in the same state as the caller gave it!
// Extra is here in order to add extra bindings if needed.
void LuaInterpreter::fill_gui_metatable(lua_State* L, const char* table_name, lua_CFunction f, const luaL_Reg *extra = nullptr){
    // Add all of our GUI functions that can be used for each component
    lua_newtable(L);
    int table = lua_gettop(L);
    if(extra != nullptr) {
        for (const luaL_Reg* ptr = extra; ptr->name != NULL; ptr++) {
            lua_pushcfunction(L, ptr->func);
            lua_setfield(L, -2, ptr->name);
        }
    }
    for (const luaL_Reg* ptr = gui_common_binds; ptr->name != NULL; ptr++) {
        lua_pushcfunction(L, ptr->func);
	    lua_setfield(L, -2, ptr->name);
    }
    // Add garbage collector to delete object
    luaL_getmetatable(L, table_name);
    // GC function provided
    lua_pushstring(L, GARBAGE_COLLECT);
    lua_pushcfunction(L, f);
    lua_settable(L, -3);
    // Adding all of our common gui functions
    lua_pushstring(L, INDEX_METATABLE);
	lua_pushvalue(L, table);
	lua_settable(L, -3);
    // Finally remove both tables from stack
    lua_pop(L, 2);
    return;
}

// Instantiate a window element
int LuaInterpreter::window(lua_State* L) {
    if (lua_gettop(L) != 1 && !lua_isstring(L, 1)) return luaL_error(L, LUA_FUNC_ERR);
    string title = lua_tostring(L, 1);

    // Allocate our userdata and assign our metatable
    Window **w = static_cast<Window **>(lua_newuserdata(L, sizeof *w)); 
    luaL_getmetatable(L, METATABLE_WIN_GUI);
    lua_setmetatable(L, -2);

    // Fill metatable
    fill_gui_metatable(L, METATABLE_WIN_GUI, [](lua_State* L) {
        Window **parent = static_cast<Window **>(lua_touserdata(L, -1));
        delete *parent;
        return 0;
    });

    // Instance of window
    *w = new Window(title);
    return 1;
}

// Set the primary window
int LuaInterpreter::setWindow(lua_State* L) {
    Window **parent = static_cast<Window **>(lua_touserdata(L, 1));
    luaL_argcheck(L, parent != NULL, 1, "Window gui expected!");
    current_root = (*parent);
    return 0;
}

// Gets the current window shown
int LuaInterpreter::getWindow(lua_State* L) {
    if(current_root != nullptr) {
        Window **w = static_cast<Window **>(lua_newuserdata(L, sizeof *w));
        luaL_getmetatable(L, METATABLE_WIN_GUI);
        lua_setmetatable(L, -2);
        *w = current_root;
        return 1;
    }
    return luaL_error(L, LUA_WINDOW_ERR);
}

// TODO: Check userdata later to avoid surprises
// Signature of box: parent -> x -> y -> w -> h -> pgui
int LuaInterpreter::box(lua_State* L) {
    if (lua_gettop(L) != 5 && !lua_isnumber(L, 2)
                           && !lua_isnumber(L, 3)
                           && !lua_isnumber(L, 4)
                           && !lua_isnumber(L, 5)) return luaL_error(L, LUA_FUNC_ERR);
    Window **parent = static_cast<Window **>(lua_touserdata(L, 1));
    luaL_argcheck(L, parent != NULL, 1, "Parent gui expected!");
    uint16_t x         = lua_tonumber(L, 2);
    uint16_t y         = lua_tonumber(L, 3);
    uint16_t w         = lua_tonumber(L, 4);
    uint16_t h         = lua_tonumber(L, 5);

    // Allocate our userdata and assign our metatable
    Box **b = static_cast<Box **>(lua_newuserdata(L, sizeof *b));
    luaL_getmetatable(L, METATABLE_BOX_GUI);
    lua_setmetatable(L, -2);

    // Fill metatable
    fill_gui_metatable(L, METATABLE_BOX_GUI, [](lua_State* L) {
        Box **b = static_cast<Box **>(lua_touserdata(L, -1));
        delete *b;
        return 0;
    });

    // Instance of box
    *b = new Box(x, y, w, h);
    (*b)->enabledBackground = true;
    (*b)->setBackgroundColor(COLOR_EXTRA_LIGHT);
    (*parent)->addChild(*b);
    return 1;
}

int LuaInterpreter::label(lua_State* L) {
    if (lua_gettop(L) != 5 && !lua_isnumber(L, 2)
                           && !lua_isnumber(L, 3)
                           && !lua_isnumber(L, 4)
                           && !lua_isnumber(L, 5)) return luaL_error(L, LUA_FUNC_ERR);
    Window **parent = static_cast<Window **>(lua_touserdata(L, 1));
    luaL_argcheck(L, parent != NULL, 1, "Parent gui expected!");
    uint16_t x         = lua_tonumber(L, 2);
    uint16_t y         = lua_tonumber(L, 3);
    uint16_t w         = lua_tonumber(L, 4);
    uint16_t h         = lua_tonumber(L, 5);

    // Allocate our userdata and assign our metatable
    Label **l = static_cast<Label **>(lua_newuserdata(L, sizeof *l));
    luaL_getmetatable(L, METATABLE_LABEL_GUI);
    lua_setmetatable(L, -2);
    
    // Fill metatable
    fill_gui_metatable(L, METATABLE_LABEL_GUI, [](lua_State* L) {
        Label **l = static_cast<Label **>(lua_touserdata(L, -1));
        delete *l;
        return 0;
    });

    // Instance of label
    *l = new Label(x, y, w, h);
    (*parent)->addChild(*l);
    return 1;
}

int LuaInterpreter::button(lua_State* L) {
    if (lua_gettop(L) != 5 && !lua_isnumber(L, 2)
                           && !lua_isnumber(L, 3)
                           && !lua_isnumber(L, 4)
                           && !lua_isnumber(L, 5)) return luaL_error(L, LUA_FUNC_ERR);
    Window **parent = static_cast<Window **>(lua_touserdata(L, 1));
    luaL_argcheck(L, parent != NULL, 1, "Parent gui expected!");
    uint16_t x         = lua_tonumber(L, 2);
    uint16_t y         = lua_tonumber(L, 3);
    uint16_t w         = lua_tonumber(L, 4);
    uint16_t h         = lua_tonumber(L, 5);

    // Allocate our userdata and assign our metatable
    Button **b = static_cast<Button **>(lua_newuserdata(L, sizeof *b));
    luaL_getmetatable(L, METATABLE_BTN_GUI);
    lua_setmetatable(L, -2);

    // Fill metatable
    fill_gui_metatable(L, METATABLE_BTN_GUI, [](lua_State* L) {
        Button **b = static_cast<Button **>(lua_touserdata(L, -1));
        delete *b;
        return 0;
    });

    // Instance of button
    *b = new Button(x, y, w, h);
    (*parent)->addChild(*b);
    return 1;
}

int LuaInterpreter::canvas(lua_State* L) {
    if (lua_gettop(L) != 5 && !lua_isnumber(L, 2)
                           && !lua_isnumber(L, 3)
                           && !lua_isnumber(L, 4)
                           && !lua_isnumber(L, 5)) return luaL_error(L, LUA_FUNC_ERR);
    Window **parent = static_cast<Window **>(lua_touserdata(L, 1));
    luaL_argcheck(L, parent != NULL, 1, "Parent gui expected!");
    uint16_t x         = lua_tonumber(L, 2);
    uint16_t y         = lua_tonumber(L, 3);
    uint16_t w         = lua_tonumber(L, 4);
    uint16_t h         = lua_tonumber(L, 5);

    // Allocate our userdata and assign our metatable
    Canvas **b = static_cast<Canvas **>(lua_newuserdata(L, sizeof *b));
    luaL_getmetatable(L, METATABLE_BTN_GUI);
    lua_setmetatable(L, -2);

    // Fill metatable
    fill_gui_metatable(L, METATABLE_BTN_GUI, [](lua_State* L) {
        Canvas **b = static_cast<Canvas **>(lua_touserdata(L, -1));
        delete *b;
        return 0;
    });

    // Instance of button
    *b = new Canvas(x, y, w, h);
    (*parent)->addChild(*b);
    return 1;
}

int LuaInterpreter::setX(lua_State* L) {
    if (lua_gettop(L) != 2 || !lua_isnumber(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -2);
    gui->setX(lua_tointeger(L, -1));
    return 0;
}

int LuaInterpreter::setY(lua_State* L) {
    if (lua_gettop(L) != 2 || !lua_isnumber(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -2);
    gui->setY(lua_tointeger(L, -1));
    return 0;
}

int LuaInterpreter::setWidth(lua_State* L) {
    if (lua_gettop(L) != 2 || !lua_isnumber(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -2);
    gui->setWidth(lua_tointeger(L, -1));
    return 0;
}

int LuaInterpreter::setHeight(lua_State* L) {
    if (lua_gettop(L) != 2 || !lua_isnumber(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -2);
    gui->setHeight(lua_tointeger(L, -1));
    return 0;
}

int LuaInterpreter::getX(lua_State* L) {
    if (lua_gettop(L) != 1) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -1);
    lua_pushnumber(L, gui->getX());
    return 1;
}

int LuaInterpreter::getY(lua_State* L) {
    if (lua_gettop(L) != 1) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -1);
    lua_pushnumber(L, gui->getY());
    return 1;
}

int LuaInterpreter::getWidth(lua_State* L) {
    if (lua_gettop(L) != 1) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -1);
    lua_pushnumber(L, gui->getWidth());
    return 1;
}

int LuaInterpreter::getHeight(lua_State* L) {
    if (lua_gettop(L) != 1) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -1);
    lua_pushnumber(L, gui->getHeight());
    return 1;
}

// Maybe introduce later on a userdata for colors?
int LuaInterpreter::setColor(lua_State* L) {
    if (lua_gettop(L) != 2 || !lua_isnumber(L, -1)) return luaL_error(L, LUA_FUNC_ERR);

    Gui* gui       = get_checked_gui(L, -2);
    uint16_t color = lua_tointeger(L, -1);
    gui->setBackgroundColor(color);
    return 0;
}

int LuaInterpreter::setText(lua_State* L) {
    if (lua_gettop(L) != 2 || !lua_isstring(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -2);
    // Cast to derived class before doing anything
    switch (gui->getType()) {
        case GUI_TYPE::LABEL_TYPE:
            reinterpret_cast<Label*>(gui)->setText(lua_tostring(L, -1));
            break;
        case GUI_TYPE::BUTTON_TYPE:
            reinterpret_cast<Button*>(gui)->setText(lua_tostring(L, -1));
            break;
        default:
            break; // Nothing to do here
    }
    return 0;
}

int LuaInterpreter::fillRect(lua_State* L) {
    if (lua_gettop(L) != 6) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, 1);
    // Cast to derived class before doing anything
    switch (gui->getType()) {
        case GUI_TYPE::CANVAS_TYPE:
            reinterpret_cast<Canvas*>(gui)->l_tft.fillRect(lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5), lua_tointeger(L, 6));
            break;
        default:
            break; // Nothing to do here
    }
    return 0;
}

int LuaInterpreter::push(lua_State* L) {
    if (lua_gettop(L) != 1) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, 1);
    // Cast to derived class before doing anything
    switch (gui->getType()) {
        case GUI_TYPE::CANVAS_TYPE:
            {
            Canvas* c = reinterpret_cast<Canvas*>(gui);
            c->l_tft.pushSprite(&tft_root, c->getAbsoluteX(), c->getAbsoluteY());
            break;
            }
        default:
            break; // Nothing to do here
    }
    return 0;
}

// Setup callback functions
int LuaInterpreter::onClick(lua_State* L) {
    if(!(lua_gettop(L) == 2) && !lua_isfunction(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    Gui *parent = get_checked_gui(L, -2);
    int callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    LuaEvent event = {parent, callback_ref};
    events.push_back(event);
    return 0;
}

int LuaInterpreter::readFile(lua_State* L) {
    storage::FileStream file(dir + "/" + lua_tostring(L, 1), storage::READ);
    std::string readed = file.read();
    lua_pushstring(L, readed.c_str());
    file.close();
    return 1;
}

int LuaInterpreter::writeFile(lua_State* L) {
    storage::FileStream file(dir + "/" + lua_tostring(L, 1), storage::WRITE);
    file.write(lua_tostring(L, 2));
    file.close();
    return 0;
}

// Paxo sleep!
// Add non blocking sleep that allows other thread to work!
int LuaInterpreter::special_sleep(lua_State* L) {
    if(lua_gettop(L) != 1 || !luaL_checknumber(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    // sleep(lua_tonumber(L, -1) * 1000);
    printf("Should be sleeping now?!");
    return 0;
}

// Easy checks for good pointer
Gui *get_checked_gui(lua_State* L, int idx) {
    Gui **p = static_cast<Gui **>(lua_touserdata(L, idx));
    luaL_argcheck(L, p != NULL, 1, "Parent gui expected!");
    return *p;
}

// Custom allocator
void* custom_allocator(void *ud, void *ptr, size_t osize, size_t nsize) {
    if (nsize == 0) {
        // Free the block
        if (ptr != NULL) {
            free(ptr);
        }
        return NULL;
    } else {
        // Allocate or resize the block
        #ifdef ESP32
            return ps_realloc(ptr, nsize);
        #else
            return realloc(ptr, nsize);
        #endif
    }
}

void execute_lua(lua_State* L, const std::string& functionName) {
    lua_getglobal(L, functionName.c_str());
    if(!lua_isfunction(L, -1)) std::cerr << "Lua function '" << functionName << "' not found." << std::endl;
    if(!lua_pcall(L, 0, LUA_MULTRET, 0) == 0) std::cerr << "Error executing Lua function '" << functionName << "': " << lua_tostring(L, -1) << std::endl;
}

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
