#include "lua2.hpp"
#include "../interface/filestream.hpp"
#include "../network/network.hpp"
#include "../interface/interface.hpp"
#include "../interface/console.hpp"

void LuaWidget::init(Gui* obj, LuaWidget* parent, int x, int y, int width, int height)
{
    widget = obj;
    parent->addChild(this);
    parent->widget->addChild(this->widget);
}

LuaWidget::~LuaWidget()
{
    for (LuaWidget* child : children)
    {
        delete child;
    }

    if (this->widget->parent == nullptr)
    {
        delete this->widget;
        this->widget = nullptr;
    }
}

void LuaWidget::update()
{
    if(this->widget->parent == nullptr)
        this->widget->updateAll();

    if(onClickFunc && this->widget->isTouched())
    {
        onClickFunc();
    }

    for (int i = 0; i < this->children.size(); i++)
    {
        children[i]->update();
    }
}

// Box

LuaBox::LuaBox(LuaWidget* parent, int x, int y, int width, int height)
{
    widget = new Box(x, y, width, height);
    init(widget, parent, x, y, width, height);
}

// Image

LuaImage::LuaImage(LuaWidget* parent, const std::string& path, int x, int y, int width, int height)
{
    widget = new Image(path, x, y, width, height);
    init(widget, parent, x, y, width, height);
    widget->load();
}

// Label

LuaLabel::LuaLabel(LuaWidget* parent, int x, int y, int width, int height)
{
    widget = new Label(x, y, width, height);
    init(widget, parent, x, y, width, height);
}

// Input

LuaInput::LuaInput(LuaWidget* parent, int x, int y, int width, int height)
{
    widget = new Label(x, y, width, height);    // a mettre a jour sur le paxos9
    init(widget, parent, x, y, width, height);
    widget->setCanBeEdited(true);
}


// Window

LuaWindow::LuaWindow()  
{
    widget = new Window();
    init(widget);
}

// Gui

LuaGui::LuaGui(LuaFile* lua)
{
    this->lua = lua;
}

LuaBox* LuaGui::box(LuaWidget* parent, int x, int y, int width, int height)
{
    LuaBox* w = new LuaBox(parent, x, y, width, height);
    return w;
}

LuaImage* LuaGui::image(LuaWidget* parent, const std::string& path, int x, int y, int width, int height)
{
    std::string filename = path;

    if (filename.empty())
        return nullptr;

    if (!this->lua->perms.acces_files)  // a acces aux fichiers
        return nullptr;

    if (!this->lua->perms.acces_files_root) // a acces a la racine de la mémoire
        if(filename.find("../") == std::string::npos || filename[0] == '/')
            return nullptr;
    
    LuaImage* w = new LuaImage(parent, filename, x, y, width, height);

    return w;
}

LuaLabel* LuaGui::label(LuaWidget* parent, int x, int y, int width, int height)
{
    LuaLabel* w = new LuaLabel(parent, x, y, width, height);
    return w;
}

LuaInput* LuaGui::input(LuaWidget* parent, int x, int y, int width, int height)
{
    LuaInput* w = new LuaInput(parent, x, y, width, height);
    return w;
}

LuaWindow* LuaGui::window()
{
    return new LuaWindow();
}

void LuaGui::update()
{
    if(mainWindow != nullptr)
    {
        mainWindow->update();
    }
}

// Gui end

void LuaHardware::flash(bool flash)
{
    std::cout << "flash" << flash << std::endl;
}

LuaFile::LuaFile(std::string filename)
    :lua_gui(this)
{
    this->filename = filename;

    size_t lastSlashPos = filename.find_last_of("/\\"); // trouver le path du dossier

    if (lastSlashPos != std::string::npos) {
        this->directory = filename.substr(0, lastSlashPos);
    } else {
        this->directory = "";
    }
}

LuaFile::~LuaFile()
{
    // libérer les ressources (events, etc)
}

void LuaFile::run()
{
    // Charger le fichier lua
    storage::FileStream file(filename, storage::READ);
    std::string code = file.read();
    file.close();

    // Charger le module dans l'environnement Lua
    lua.open_libraries(sol::lib::base);

    // Lire la configuration
    storage::FileStream file2(directory+"conf.txt", storage::READ);
    std::string conf = file2.read();
    file2.close();

    // en fonction de la configuration, choisir les permissions

    // perms....

    // en fonction des permissions, charger certains modules

    lua.open_libraries(sol::lib::base);

    if (perms.acces_hardware)   // si hardware est autorisé
    {
        lua.new_usertype<LuaHardware>("hardware",
            "flash", &LuaHardware::flash
        );

        lua["hardware"] = &lua_hardware;
    }

    if (perms.acces_files)   // si storage est autorisé
    {
        lua.new_usertype<LuaStorage>("storage"
            //"flash", &LuaStorage::flash
        );

        lua["storage"] = &lua_storage;
    }

    if (perms.acces_gui)   // si gui est autorisé
    {
        lua.new_usertype<LuaGui>("gui",
            "box", &LuaGui::box,
            "image", &LuaGui::image,
            "label", &LuaGui::label,
            "input", &LuaGui::input,
            "window", &LuaGui::window,
            "setWindow", &LuaGui::setMainWindow
        );

        lua["gui"] = &lua_gui;

        lua.new_usertype<LuaWidget>("widget",
            "setX", &LuaWidget::setX,
            "setY", &LuaWidget::setY,
            "setWidth", &LuaWidget::setWidth,
            "setHeight", &LuaWidget::setHeight,
            "getX", &LuaWidget::getX,
            "getY", &LuaWidget::getY,
            "getWidth", &LuaWidget::getWidth,
            "getHeight", &LuaWidget::getHeight,
            "setColor", &LuaWidget::setColor,
            "getColor", &LuaWidget::getColor,
            "enable", &LuaWidget::enable,
            "disable", &LuaWidget::disable,
            "isTouched", &LuaWidget::isTouched,
            "onClick", &LuaWidget::onClick
        );

        lua.new_usertype<LuaBox>("LuaBox",
            sol::base_classes, sol::bases<LuaWidget>());

        lua.new_usertype<LuaImage>("LuaImage",
            sol::base_classes, sol::bases<LuaWidget>());

        lua.new_usertype<LuaLabel>("LuaLabel",
            "setText", &LuaLabel::setText,
            "getText", &LuaLabel::getText,
            "setFontSize", &LuaLabel::setFontSize,
            "getTextHeight", &LuaLabel::getTextHeight,
            sol::base_classes, sol::bases<LuaWidget>());

        lua.new_usertype<LuaInput>("LuaLabel",
            "setText", &LuaInput::setText,
            "getText", &LuaInput::getText,
            "setFontSize", &LuaInput::setFontSize,
            "getTextHeight", &LuaInput::getTextHeight,
            "onChange", &LuaInput::onChange,
            sol::base_classes, sol::bases<LuaWidget>());

        lua.new_usertype<LuaWindow>("LuaWindow",
            sol::base_classes, sol::bases<LuaWidget>());
    }

    try
    {
        lua.script(code, sol::script_throw_on_error);
        lua.script("run()", sol::script_throw_on_error);

        while (!home_button::isPressed())
        {
            lua_gui.update();
        }
    }
    catch (const sol::error& e)
    {
        std::cerr << "Erreur Lua : " << e.what() << std::endl;
        return;
    }
}