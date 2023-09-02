#include "lua.hpp"

void LuaInterpreter::loadScript(std::string filename)
{
    storage::LFile file(filename, storage::OPEN_MODE::READ);
    data = file.read();
    file.close();
}

void LuaInterpreter::runApp()
{
    idcounter = 0;

    print ("vector" + to_string(gui.size()));

    lua_State* L = luaL_newstate();
    lua_setallocf(L, custom_allocator, NULL);

    luaL_openlibs(L);

    addFunction(L, "print", customPrint);
    addFunction(L, "Gui", luaNewObject);
    
    // GUI methods
    addFunction(L, "setX", setX);
    addFunction(L, "setY", setY);
    addFunction(L, "setWidth", setWidth);
    addFunction(L, "setHeight", setHeight);
    
    addFunction(L, "getX", getX);
    addFunction(L, "getY", getY);
    addFunction(L, "getWidth", getWidth);
    addFunction(L, "getHeight", getHeight);
    
    addFunction(L, "setColor", setColor);
    addFunction(L, "setText", setText);
    addFunction(L, "onClick", onClick);
    
    
    addVariable(L, "BOX_TYPE", GUI_TYPE::BOX_TYPE);
    addVariable(L, "BUTTON_TYPE", GUI_TYPE::BUTTON_TYPE);
    addVariable(L, "LABEL_TYPE", GUI_TYPE::LABEL_TYPE);
    addVariable(L, "WINDOW_TYPE", GUI_TYPE::WINDOW_TYPE);
    addVariable(L, "IMAGE_TYPE", GUI_TYPE::IMAGE_TYPE);

    setColorInit(L);

    if (luaL_loadstring(L, data.c_str()) == 0) {
        if (lua_pcall(L, 0, LUA_MULTRET, 0) == 0) {
            // Lua script executed successfully, and output is captured in luaOutput.
        } else {
            //print("Error executing Lua script: " + std::string(lua_tostring(L, -1)));
        }
    } else {
        printf("Error loading Lua script");
    }

    runLuaFunction(L, "run");

    while(!home_button.pressed())
    {
        gui[0]->updateAll();

        for (int i = 0; i < events.size(); i++)
        {
            if(((events[i].obj)->*(events[i].func))())
            {
                runLuaFunction(L, events[i].callback);
            }
        }
    }

    lua_close(L);

    for (int i = 0; i < gui.size(); i++)
    {
        if(gui[i]->parent == nullptr)
            delete gui[i];
    }

    gui.clear();
    events.clear();

    data = "";
}

int LuaInterpreter::luaNewObject(lua_State* L)
{
    uint type = lua_tonumber(L, 1);
    uint x = lua_tonumber(L, 2);
    uint y = lua_tonumber(L, 3);
    uint w = lua_tonumber(L, 4);
    uint h = lua_tonumber(L, 5);

    if(type==GUI_TYPE::WINDOW_TYPE)
    {
        gui.push_back(new Window("lua app"));
    }else if(type==GUI_TYPE::BOX_TYPE)
    {
        gui.push_back(new Box(x,y,w,h));
        gui[idcounter]->enabledBackground = true;
        gui[idcounter]->setBackgroundColor(COLOR_EXTRA_LIGHT);
    }else if(type==GUI_TYPE::LABEL_TYPE)
    {
        gui.push_back(new Label(x,y,w,h));
    }else if(type==GUI_TYPE::BUTTON_TYPE)
    {
        gui.push_back(new Button(x,y,w,h));
    }
    
    if(lua_gettop(L)==6) // has parent
    {
        uint id = lua_tonumber(L, 6);
        gui[id]->addChild(gui[idcounter]);
    }
    
    lua_pushinteger(L, idcounter);
    idcounter++;
    return 1;
}

void LuaInterpreter::setColorInit(lua_State* L)
{
    addVariable(L, "COLOR_LIGHT", COLOR_LIGHT);
    addVariable(L, "COLOR_BLACK", COLOR_BLACK);
    addVariable(L, "COLOR_PRIMARY", COLOR_PRIMARY);
    addVariable(L, "COLOR_SUCCESS", COLOR_SUCCESS);
    addVariable(L, "COLOR_WHITE", COLOR_WARNING);
    addVariable(L, "COLOR_WHITE", COLOR_BLUE);
}

int LuaInterpreter::setX(lua_State* L)
{
    Gui* g = gui[lua_tointeger(L, 1)];
    uint16_t xPos = lua_tointeger(L, 2);
    g->setX(xPos);
    return 0;
}

int LuaInterpreter::setY(lua_State* L)
{
    Gui* g = gui[lua_tointeger(L, 1)];
    uint16_t yPos = lua_tointeger(L, 2);
    g->setY(yPos);
    return 0;
}

int LuaInterpreter::setWidth(lua_State* L)
{
    Gui* g = gui[lua_tointeger(L, 1)];
    uint16_t width = lua_tointeger(L, 2);
    g->setWidth(width);
    return 0;
}

int LuaInterpreter::setHeight(lua_State* L)
{
    Gui* g = gui[lua_tointeger(L, 1)];
    uint16_t height = lua_tointeger(L, 2);
    g->setHeight(height);
    return 0;
}

int LuaInterpreter::getX(lua_State* L)
{
    Gui* g = gui[lua_tointeger(L, 1)];
    lua_pushnumber(L, g->getX());
    return 1;
}

int LuaInterpreter::getY(lua_State* L)
{
    Gui* g = gui[lua_tointeger(L, 1)];
    lua_pushnumber(L, g->getY());
    return 1;
}

int LuaInterpreter::getWidth(lua_State* L)
{
    Gui* g = gui[lua_tointeger(L, 1)];
    lua_pushnumber(L, g->getWidth());
    return 1;
}

int LuaInterpreter::getHeight(lua_State* L)
{
    Gui* g = gui[lua_tointeger(L, 1)];
    lua_pushnumber(L, g->getHeight());
    return 1;
}

int LuaInterpreter::setColor(lua_State* L)
{
    Gui* g = gui[lua_tointeger(L, 1)];
    uint color = lua_tointeger(L, 2);
    g->setBackgroundColor(color);
    return 0;
}

int LuaInterpreter::setText(lua_State* L)
{
    Gui* g = gui[lua_tointeger(L, 1)];
    //string text = lua_tostring(L, 2);


    if(g->getType() == GUI_TYPE::LABEL_TYPE || g->getType() == GUI_TYPE::BUTTON_TYPE)
    {
        reinterpret_cast<Label*>(g)->setText(lua_tostring(L, 2));
    }
    return 0;
}

int LuaInterpreter::onClick(lua_State* L)
{
    //print("onClick");
    Gui* g = gui[lua_tointeger(L, 1)];
    string text = lua_tostring(L, 2);
    LuaEvent event;
    event.callback = text;
    event.func = &Gui::isTouched;
    event.obj = g;
    events.push_back(event);
    return 0;
}