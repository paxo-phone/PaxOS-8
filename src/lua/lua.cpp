#include "lua.hpp"
#include "../interface/filestream.hpp"
#include "../network/network.hpp"
#include "../interface/interface.hpp"
#include "../interface/console.hpp"

#define uint unsigned int // Windows compatibility

Window* LuaInterpreter::current_root = nullptr;
vector<LuaEvent> LuaInterpreter::events;
vector<LuaEventInterval*> LuaInterpreter::intervals;
vector<LuaEventTimeOut*> LuaInterpreter::timeOuts;
LuaEventTimeOut* LuaInterpreter::timeOutToRemove = nullptr;
uint64_t LuaInterpreter::timerFromStart;
std::string LuaInterpreter::dir;

LuaEventInterval::LuaEventInterval(lua_State *L, int callback_ref, int interval)
{
    this->callback_ref = callback_ref;
    this->L = L;
    this->id = setInterval(new CallbackMethod<LuaEventInterval>(this, &LuaEventInterval::call), interval);
}
LuaEventInterval::~LuaEventInterval()
{
    removeInterval(this->id);
}
void LuaEventInterval::call(void)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback_ref);
    if (lua_pcall(L, 0, 0, 0) != 0) {
        // Gestion des erreurs en cas d'échec de l'appel
        console.log(lua_tostring(L, -1));
        // Faites quelque chose avec l'erreur...
        lua_pop(L, 1);  // Nettoyez le message d'erreur de la pile
    }
}

LuaEventTimeOut::LuaEventTimeOut(lua_State *L, int callback_ref, int timer)
{
    this->callback_ref = callback_ref;
    this->L = L;
    this->id = setTimeout(new CallbackMethod<LuaEventTimeOut>(this, &LuaEventTimeOut::call), timer);
}
LuaEventTimeOut::~LuaEventTimeOut()
{
    removeTimeout(this->id);
}
void LuaEventTimeOut::call(void)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback_ref);
    if (lua_pcall(L, 0, 0, 0) != 0) {
        // Gestion des erreurs en cas d'échec de l'appel
        console.log(lua_tostring(L, -1));
        // Faites quelque chose avec l'erreur...
        lua_pop(L, 1);  // Nettoyez le message d'erreur de la pile
    }
    LuaInterpreter::timeOutToRemove = this;
}

LuaInterpreter::LuaInterpreter(string dir) {
    if (dir.size() == 0)
        return;
    if(dir[dir.size() - 1] != '/')
        dir += '/';
    console.log("dir: " + dir);
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
            console.log("Error executing Lua script: " + std::string(lua_tostring(L, -1)));
            return;
        }
    } else {
        console.log("Error loading Lua script");
        return;
    }
    console.log("LuaInterpreter");

    timerFromStart = millis();
    // Execute our lua app
    execute_lua(L, LUA_MAIN_FUNCTION);

    // Also checks that a window has been set!
    while(!home_button::isPressed() && current_root != nullptr) {
        current_root->updateAll();
        for (LuaEvent event : events) {
            if(event.ref->isTouched()){
                lua_rawgeti(L, LUA_REGISTRYINDEX, event.callback_ref);
                lua_call(L, 0, 0);
            }
       }
       if(timeOutToRemove != nullptr)
       {
            for (uint i = 0; i < timeOuts.size(); i++)
            {
                if (timeOuts[i] == timeOutToRemove)
                {
                    delete timeOuts[i];
                    timeOuts.erase(timeOuts.begin() + i);
                    break;
                }
            }
       }
    }

    // LUA_GC will take care of memory!
    lua_close(L);

    // Resets our vars
    current_root = nullptr;
    events.clear();
    data.clear();

    for (uint i = 0; i < intervals.size(); i++)
        delete intervals[i];
    intervals.clear();

    for (uint i = 0; i < timeOuts.size(); i++)
        delete timeOuts[i];
    timeOuts.clear();

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

int LuaInterpreter::delWindow(lua_State* L) {
    Window **parent = static_cast<Window **>(lua_touserdata(L, 1));
    luaL_argcheck(L, parent != NULL, 1, "Window gui expected!");
    delete (*parent);
    current_root = nullptr;
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

int LuaInterpreter::label(lua_State* L)
{
    // Vérifier les arguments
    if (lua_gettop(L) != 5
        && !lua_isnumber(L, 2)
        && !lua_isnumber(L, 3)
        && !lua_isnumber(L, 4)
        && !lua_isnumber(L, 5)) return luaL_error(L, LUA_FUNC_ERR);

    // Récuperer le parent (la fenêtre)
    Window** pParent = static_cast<Window**>(lua_touserdata(L, 1));

    luaL_argcheck(L, pParent != nullptr, 1, "Parent gui expected!");
    int16_t x = lua_tonumber(L, 2);
    int16_t y = lua_tonumber(L, 3);
    int16_t w = lua_tonumber(L, 4);
    int16_t h = lua_tonumber(L, 5);

    // Créer le nouveau label
    Label* pLabel = new Label(x, y, w, h);
    Label** pUserData = static_cast<Label**>(lua_newuserdata(L, sizeof(Label*)));
    *pUserData = pLabel;

    luaL_getmetatable(L, METATABLE_LABEL_GUI);
    lua_setmetatable(L, -2);

    // Remplir la usertable (label) avec ses méthodes associées (ex: setText ...)
    fill_gui_metatable(L, METATABLE_LABEL_GUI, [](lua_State* L) {
        return 0;
    });

    // Ajouter le label à la fenêtre
    (*pParent)->addChild(pLabel);

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

int LuaInterpreter::image(lua_State* L) {
    if ((lua_gettop(L) != 6 && !lua_isnumber(L, 3)
                           && !lua_isnumber(L, 4)
                           && !lua_isnumber(L, 5)
                           && !lua_isnumber(L, 6)
                           && !lua_isstring(L, 2))
        && (lua_gettop(L) != 4 && !lua_isnumber(L, 2)
                           && !lua_isnumber(L, 3)
                           && !lua_isnumber(L, 4)
                           && !lua_isstring(L, 2)))

        return luaL_error(L, LUA_FUNC_ERR);
    
    Window **parent = static_cast<Window **>(lua_touserdata(L, 1));
    luaL_argcheck(L, parent != NULL, 1, "Parent gui expected!");
    uint16_t x         = lua_tonumber(L, 3);
    uint16_t y         = lua_tonumber(L, 4);

    uint16_t w         = AUTO;
    uint16_t h         = AUTO;

    if (lua_gettop(L)==6)
    {
        w         = lua_tonumber(L, 5);
        h         = lua_tonumber(L, 6);
    }
    
    std:string path    = dir+lua_tostring(L, 2);

    // Allocate our userdata and assign our metatable
    Image **b = static_cast<Image **>(lua_newuserdata(L, sizeof *b));
    luaL_getmetatable(L, METATABLE_BTN_GUI);
    lua_setmetatable(L, -2);

    // Fill metatable
    fill_gui_metatable(L, METATABLE_BTN_GUI, [](lua_State* L) {
        Image **b = static_cast<Image **>(lua_touserdata(L, -1));
        delete *b;
        return 0;
    });

    console.log(path);

    // Instance of button
    *b = new Image(path, x, y, w, h);
    (*b)->load();
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

int LuaInterpreter::setVerticalAlignment(lua_State* L)
{
    if (lua_gettop(L) != 2) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -2);
    gui->setVerticalAlignment(lua_tointeger(L, 2));
    return 0;
}

int LuaInterpreter::setHorizontalAlignment(lua_State* L)
{
    if (lua_gettop(L) != 2 || !lua_isnumber(L, 2)) return luaL_error(L, LUA_FUNC_ERR);

    Gui* gui       = get_checked_gui(L, 1);
    gui->setHorizontalAlignment(lua_tointeger(L, 2));
    return 0;
}

// Maybe introduce later on a userdata for colors?
int LuaInterpreter::setColor(lua_State* L) {
    std::cout << "color cpp: " << std::endl;
    if (lua_gettop(L) != 2 || !lua_isnumber(L, 2)) return luaL_error(L, LUA_FUNC_ERR);

    Gui* gui       = get_checked_gui(L, 1);
    uint16_t color = lua_tointeger(L, 2);
    std::cout << "color cpp: " << color << std::endl;
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

int LuaInterpreter::getText(lua_State* L) {
    if (lua_gettop(L) != 1) return luaL_error(L, LUA_FUNC_ERR);
    Gui *gui = get_checked_gui(L, -1);
    // Cast to derived class before doing anything
    switch (gui->getType()) {
        case GUI_TYPE::LABEL_TYPE:
            lua_pushstring(L, (reinterpret_cast<Label*>(gui))->getText().c_str());
            break;
        case GUI_TYPE::BUTTON_TYPE:
            lua_pushstring(L, (reinterpret_cast<Button*>(gui))->getText().c_str());
            break;
        default:
            luaL_error(L, LUA_FUNC_ERR);
            break; // Nothing to do here
    }
    return 1;
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
    if(lua_gettop(L) != 1 || !lua_isstring(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    storage::FileStream file(dir + lua_tostring(L, 1), storage::READ);
    std::string readed = file.read();
    lua_pushstring(L, readed.c_str());
    file.close();
    return 1;
}

int LuaInterpreter::writeFile(lua_State* L) {
    if(lua_gettop(L) != 2 || !lua_isstring(L, 1) || !lua_isstring(L, 2)) return luaL_error(L, LUA_FUNC_ERR);
    storage::FileStream file(dir + lua_tostring(L, 1), storage::WRITE);
    file.write(lua_tostring(L, 2));
    file.close();
    return 0;
}

int LuaInterpreter::renameFile(lua_State* L)
{
    if(lua_gettop(L) != 2 || !lua_isstring(L, 1) || !lua_isstring(L, 2)) return luaL_error(L, LUA_FUNC_ERR);
    lua_pushboolean(L, storage::rename(dir+lua_tostring(L, 1), dir+lua_tostring(L, 2)));
    return 1;
}

int LuaInterpreter::deleteFile(lua_State* L) {
    if(lua_gettop(L) != 1 || !lua_isstring(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    lua_pushboolean(L, storage::remove(dir+lua_tostring(L, 1)));
    console.log("removing file: " + dir + lua_tostring(L, 1));
    return 1;
}

int LuaInterpreter::newDir(lua_State* L) {
    if(lua_gettop(L) != 1 || !lua_isstring(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    lua_pushboolean(L, storage::newdir(dir + lua_tostring(L, 1)));
    return 1;
}

int LuaInterpreter::renameDir(lua_State* L) {
    if(lua_gettop(L) != 2 || !lua_isstring(L, 1) || !lua_isstring(L, 2)) return luaL_error(L, LUA_FUNC_ERR);
    lua_pushboolean(L, storage::rename(dir+lua_tostring(L, 1), dir+lua_tostring(L, 2)));
    return 1;
}

int LuaInterpreter::deleteDir(lua_State* L) {
    if(lua_gettop(L) != 1 || !lua_isstring(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    lua_pushboolean(L, storage::remove(dir+lua_tostring(L, 1)));
    return 1;
}

int LuaInterpreter::isDir(lua_State* L) {
    if(lua_gettop(L) != 1 || !lua_isstring(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    lua_pushboolean(L, storage::isdir(dir+lua_tostring(L, 1)));
    return 1;
}

int LuaInterpreter::isFile(lua_State* L) {
    if(lua_gettop(L) != 1 || !lua_isstring(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    lua_pushboolean(L, storage::isfile(dir+lua_tostring(L, 1)));
    return 1;
}

int LuaInterpreter::listDir(lua_State* L)
{
    if (lua_gettop(L) != 1 || !lua_isstring(L, -1)) return luaL_error(L, LUA_FUNC_ERR);

    std::vector<std::string> list = storage::listdir(dir + lua_tostring(L, 1));

    lua_newtable(L);

    for (size_t i = 0; i < list.size(); ++i) {
        // Push the key (index starting from 1)
        lua_pushinteger(L, i + 1);

        // Push the value (string)
        lua_pushstring(L, list[i].c_str());

        // Set the key-value pair in the table
        lua_settable(L, -3);
    }

    // Retournez la table
    return 1;
}

// Paxo sleep!
// Add non blocking sleep that allows other thread to work!
int LuaInterpreter::specialSleep(lua_State* L) {
    if(lua_gettop(L) != 1 || !luaL_checknumber(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    // sleep(lua_tonumber(L, -1) * 1000);
    printf("Should be sleeping now?!");
    return 0;
}

int LuaInterpreter::setInterval(lua_State* L)
{
    if(!(lua_gettop(L) == 2) || !lua_isfunction(L, 1) || !lua_isinteger(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    int timer = lua_tointeger(L, -1);
    lua_pop(L, 1);
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    console.log("timer: " + to_string(timer));
    LuaEventInterval* event = new LuaEventInterval(L, ref, timer);
    intervals.push_back(event);
    return 0;
}

int LuaInterpreter::setTimeOut(lua_State* L)
{
    if(!(lua_gettop(L) == 2) || !lua_isfunction(L, 1) || !lua_isinteger(L, -1)) return luaL_error(L, LUA_FUNC_ERR);
    int timer = lua_tointeger(L, -1);
    lua_pop(L, 1);
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    console.log("timer: " + to_string(timer));
    LuaEventTimeOut* event = new LuaEventTimeOut(L, ref, timer);
    timeOuts.push_back(event);
    return 0;
}

int LuaInterpreter::monotonic(lua_State* L) {
    if(!(lua_gettop(L) == 0))
        return luaL_error(L, LUA_FUNC_ERR);
    
    lua_pushnumber(L, millis()-timerFromStart);

    return 1;
}

int LuaInterpreter::getTime(lua_State* L) {
    if(!(lua_gettop(L) == 0))
        return luaL_error(L, LUA_FUNC_ERR);
        
    lua_newtable(L);

    lua_pushinteger(L, gsm.hours);
    lua_rawseti(L, -2, 1);

    lua_pushinteger(L, gsm.minutes);
    lua_rawseti(L, -2, 2);

    lua_pushinteger(L, gsm.seconds);
    lua_rawseti(L, -2, 3);

    lua_pushinteger(L, gsm.days);
    lua_rawseti(L, -2, 4);

    lua_pushinteger(L, gsm.months);
    lua_rawseti(L, -2, 5);

    lua_pushinteger(L, gsm.years);
    lua_rawseti(L, -2, 6);


    return 1;
}

int LuaInterpreter::setEditable(lua_State* L) {
    if (lua_gettop(L) != 2 || !lua_isboolean(L, -1))
        return luaL_error(L, LUA_FUNC_ERR);

    Gui* gui       = get_checked_gui(L, -2);

    if(gui->getType() != LABEL_TYPE)
        return luaL_error(L, LUA_FUNC_ERR);
    
    bool isEditable = lua_toboolean(L, -1);
    reinterpret_cast<Label*>(gui)->setCanBeEdited(isEditable);

    return 0;
}

int LuaInterpreter::setFontSize(lua_State* L) {
    if (lua_gettop(L) != 2 || !lua_isinteger(L, -1))
        return luaL_error(L, LUA_FUNC_ERR);

    Gui* gui       = get_checked_gui(L, -2);

    if(gui->getType() != LABEL_TYPE)
        return luaL_error(L, LUA_FUNC_ERR);
    
    uint8_t fontSize = lua_tointeger(L, -1);
    reinterpret_cast<Label*>(gui)->setFontSize(fontSize);

    return 0;
}

int LuaInterpreter::isFocused(lua_State* L) {
    if (lua_gettop(L) != 1)
        return luaL_error(L, LUA_FUNC_ERR);

    Gui* gui       = get_checked_gui(L, -1);

    lua_pushboolean(L, gui->isFocused());

    return 1;
}

int LuaInterpreter::getWeb(lua_State* L) {
    if (lua_gettop(L) != 1 || !lua_isstring(L, 1))
        return luaL_error(L, LUA_FUNC_ERR);

    network::HttpClient client;
    lua_pushstring(L, client.get(lua_tostring(L, 1)).c_str());

    return 1;
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
