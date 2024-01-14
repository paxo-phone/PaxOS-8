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
    widget->hasEvent = false;
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
        console.log("onClickFunc");
        onClickFunc();
    }

    if(onScrollUpFunc && touch.isSlidingVertically() < -30)
    {
        onScrollUpFunc();
        touch.resetScrollVertical();
    }

    if(onScrollDownFunc && touch.isSlidingVertically() > 30)
    {
        onScrollDownFunc();
        touch.resetScrollVertical();
    }

    if(onScrollRightFunc && touch.isSlidingHorizontally() > 30)
    {
        onScrollRightFunc();
        touch.resetScrollHorizontal();
    }

    if(onScrollLeftFunc && touch.isSlidingHorizontally() < -30)
    {
        onScrollLeftFunc();
        touch.resetScrollHorizontal();
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
    widget->setVerticalAlignment(CENTER_ALIGNMENT);
    widget->setFontSize(18);
    widget->setFontName("sans");

    Box* box = new Box(0, height-3, width, 3);
    box->enabledBackground = true;
    box->setBackgroundColor(COLOR_BLACK);
    box->setRadius(0);
    widget->addChild(box);
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

LuaButton* LuaGui::button(LuaWidget* parent, int x, int y, int width, int height)
{
    LuaButton* w = new LuaButton(parent, x, y, width, height);
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

// Storage

LuaStorageFile::LuaStorageFile(const std::string& filename, bool mode)
{
    std::cout << filename << std::endl;
    this->filename = filename;
    this->mode = mode;
}

void LuaStorageFile::open()
{
    file.open(filename, (mode)?(storage::WRITE):(storage::READ));
}

void LuaStorageFile::close()
{
    file.close();
}

void LuaStorageFile::write(const std::string& text)
{
    file.write(text);
}

char LuaStorageFile::readChar()
{
    return file.readchar();
}

std::string LuaStorageFile::readLine()
{
    return file.readline();
}

std::string LuaStorageFile::readAll()
{
    return file.read();
}

LuaStorage::LuaStorage(LuaFile* lua)
{
    this->lua = lua;
}

LuaStorageFile* LuaStorage::file(const std::string& filename, bool mode)
{
    if (!legalPath(filename))
        return nullptr;

    if(filename[0]=='/')
        return new LuaStorageFile(filename, mode);
    else
        return new LuaStorageFile(this->lua->directory + "/" + filename, mode);
}

bool LuaStorage::newDir(const std::string& path)
{
    return storage::newdir(convertPath(path));
}

bool LuaStorage::renameFile(const std::string& oldpath, const std::string& newpath)
{
    return storage::rename(convertPath(oldpath), convertPath(newpath));
}

bool LuaStorage::renameDir(const std::string& oldpath, const std::string& newpath)
{
    return storage::rename(convertPath(oldpath), convertPath(newpath));
}

bool LuaStorage::deleteFile(const std::string& path)
{
    return storage::remove(convertPath(path));
}

bool LuaStorage::deleteDir(const std::string& path)
{
    return storage::remove(convertPath(path));
}

bool LuaStorage::isFile(const std::string& path)
{
    return storage::isfile(convertPath(path));
}

bool LuaStorage::isDir(const std::string& path)
{
    return storage::isdir(convertPath(path));
}

std::vector<std::string> LuaStorage::listDir(const std::string& path)
{
    return storage::listdir(convertPath(path));
}

bool LuaStorage::legalPath(const std::string& path)
{
    if (!this->lua->perms.acces_files)  // a acces aux fichiers
        return false;

    if (path.empty())
        return true;

    if (!this->lua->perms.acces_files_root) // a acces a la racine de la mémoire
        if(path.find("../") == std::string::npos || path[0] == '/')
            return false;
    
    return true;
}

std::string LuaStorage::convertPath(const std::string& path)
{
    if (!legalPath(path))
        return this->lua->directory;
    
    if(path[0]=='/')
        return path;
    else
        return this->lua->directory + "/" + path;
}

// end storage

LuaTime::LuaTime(LuaFile* lua)
{
    this->lua = lua;
    timerFromStart = millis();
}

uint32_t LuaTime::monotonic()
{
    return millis() - timerFromStart;
}

int findIndex(const std::vector<std::string>& vec, const std::string& target) {
    for (std::size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return static_cast<int>(i); // Convertir en int si nécessaire
        }
    }
    return -1; // Retourner -1 si la chaîne n'est pas trouvée
}

sol::table LuaTime::get(std::string format)
{
    // découper le format en identifiers
    const std::string delimiter = ",";
    std::vector<std::string> result;
    std::string::size_type start = 0;
    std::string::size_type end = format.find(delimiter);

    while (end != std::string::npos) {
        result.push_back(format.substr(start, end - start));
        start = end + 1;
        end = format.find(delimiter, start);
    }

    result.push_back(format.substr(start));

    std::vector<std::string> identifiers = {"s","mi","h","d","mo","y"};
    std::vector<int> date = {gsm.seconds,gsm.minutes,gsm.hours,gsm.days,gsm.months,gsm.years};
    
    // ajouter les valeurs aux index des identifiers

    sol::table array = lua->lua.create_table();

    // Remplir le tableau avec des nombres
    for (int i = 0; i < result.size(); i++) {
        int index = findIndex(identifiers, result[i]);
        if(index!=-1)
            array[i+1] = date[index];
        else
            return lua->lua.create_table();
    }

    // Retourner le tableau Lua
    return array;
}

LuaTimeInterval::LuaTimeInterval(LuaFile* lua, sol::function func, uint32_t interval)
{
    this->lua = lua;
    this->func = func;
    this->interval = interval;
    this->id = setInterval(new CallbackMethod<LuaTimeInterval>(this, &LuaTimeInterval::call), interval);
}

int LuaTimeInterval::getId()
{
    return id;
}

void LuaTimeInterval::call()
{
    if(func)
        func();
}

LuaTimeInterval::~LuaTimeInterval()
{
    removeInterval(id);
}

LuaTimeTimeout::LuaTimeTimeout(LuaFile* lua, sol::function func, uint32_t timeout)
{
    this->lua = lua;
    this->func = func;
    this->timeout = timeout;
    this->id = setTimeout(new CallbackMethod<LuaTimeTimeout>(this, &LuaTimeTimeout::call), timeout);
}

int LuaTimeTimeout::getId()
{
    return id;
}

void LuaTimeTimeout::call()
{
    if(func)
        func();
    done = true;
}

LuaTimeTimeout::~LuaTimeTimeout()
{
    if(!done)
        removeTimeout(id);
}

void LuaTime::update()
{
    for (int it = 0; it < timeouts.size(); it++)
    {
        if (timeouts[it]->done)
        {
            delete timeouts[it];
            timeouts.erase(timeouts.begin() + it);
            it = 0; // reset the loop
        }
    }
}

int LuaTime::setInterval(sol::function func, uint32_t interval)
{
    LuaTimeInterval* n = new LuaTimeInterval(lua, func, interval);
    intervals.push_back(n);
    return n->getId();
}

int LuaTime::setTimeout(sol::function func, uint32_t interval)
{
    LuaTimeTimeout* n = new LuaTimeTimeout(lua, func, interval);
    timeouts.push_back(n);
    return n->getId();
}

void LuaTime::removeInterval(int id)
{
    for (int it = 0; it < intervals.size(); it++)
    {
        if (intervals[it]->getId() == id)
        {
            delete intervals[it];
            intervals.erase(intervals.begin() + it);
        }
    }
}

void LuaTime::removeTimeout(int id)
{
    for (int it = 0; it < timeouts.size(); it++)
    {
        if (timeouts[it]->getId() == id)
        {
            delete timeouts[it];
            timeouts.erase(timeouts.begin() + it);
        }
    }
}

LuaTime::~LuaTime()
{
    for (int i = 0; i < intervals.size(); i++)
        delete intervals[i];
    for (int i = 0; i < intervals.size(); i++)
        delete timeouts[i];
}

LuaHttpClient::LuaHttpClient(LuaFile* lua)
    : httpClient()
{
    this->lua = lua;
}

LuaHttpClient::~LuaHttpClient()
{
    std::cout << "LuaHttpClient deleted" << std::endl;
}

std::string LuaHttpClient::get(std::string url)
{
    // ajouter la verrification de l'url
    return httpClient.get(url);
}

std::string LuaHttpClient::post(std::string url)
{
    // ajouter la verrification de l'url
    return httpClient.get(url); // a changer quand il y aura le post :/
}

LuaNetwork::LuaNetwork(LuaFile* lua)
{
    this->lua = lua;
}

std::shared_ptr<LuaHttpClient> LuaNetwork::createHttpClient()
{
    return std::make_shared<LuaHttpClient>(lua);
}

LuaFile::LuaFile(std::string filename)
    :lua_gui(this),
    lua_storage(this),
    lua_time(this),
    lua_network(this)
{
    this->filename = filename;

    size_t lastSlashPos = filename.find_last_of("/\\"); // trouver le path du dossier

    if (lastSlashPos != std::string::npos) {
        this->directory = filename.substr(0, lastSlashPos-1);
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
    lua.open_libraries(sol::lib::math);
    lua.open_libraries(sol::lib::table);

    // Lire la configuration
    storage::FileStream file2(directory+"conf.txt", storage::READ);
    std::string conf = file2.read();
    file2.close();

    // en fonction de la configuration, choisir les permissions

    // perms....

    // en fonction des permissions, charger certains modules

    if (perms.acces_hardware)   // si hardware est autorisé
    {
        lua.new_usertype<LuaHardware>("hardware",
            "flash", &LuaHardware::flash
        );

        lua["hardware"] = &lua_hardware;
    }

    if (perms.acces_files)   // si storage est autorisé
    {
        lua.new_usertype<LuaStorageFile>("File",
            "open", &LuaStorageFile::open,
            "close", &LuaStorageFile::close,
            "write", &LuaStorageFile::write,
            "readChar", &LuaStorageFile::readChar,
            "readLine", &LuaStorageFile::readLine,
            "readAll", &LuaStorageFile::readAll
        );

        lua.new_usertype<LuaStorage>("storage",
            "file", &LuaStorage::file,
            "mkdir", &LuaStorage::newDir,
            "mvFile", &LuaStorage::renameFile,
            "mvDir", &LuaStorage::renameDir,
            "rmFile", &LuaStorage::deleteFile,
            "rmDir", &LuaStorage::deleteDir,
            "isDir", &LuaStorage::isDir,
            "isFile", &LuaStorage::isFile,
            "listDir", &LuaStorage::listDir
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
            "setMainColor", &LuaWidget::setBackgroundColor,
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
            "setBold", &LuaLabel::setBold,
            "setItalic", &LuaLabel::setItalic,
            "setVerticalAlignment", &LuaLabel::setVerticalAlignment,
            "setHorizontalAlignment", &LuaLabel::setHorizontalAlignment,
            sol::base_classes, sol::bases<LuaWidget>());

        lua.new_usertype<LuaInput>("LuaInput",
            "setText", &LuaInput::setText,
            "getText", &LuaInput::getText,
            "setFontSize", &LuaInput::setFontSize,
            "getTextHeight", &LuaInput::getTextHeight,
            "onChange", &LuaInput::onChange,
            "setVerticalAlignment", &LuaInput::setVerticalAlignment,
            "setHorizontalAlignment", &LuaInput::setHorizontalAlignment,
            sol::base_classes, sol::bases<LuaWidget>());
        
        lua.new_usertype<LuaButton>("LuaButton",
            "setText", &LuaButton::setText,
            "getText", &LuaButton::getText,
            "setFontSize", &LuaButton::setFontSize,
            "getTextHeight", &LuaButton::getTextHeight,
            "setVerticalAlignment", &LuaButton::setVerticalAlignment,
            "setHorizontalAlignment", &LuaButton::setHorizontalAlignment,
            sol::base_classes, sol::bases<LuaWidget>());

        lua.set("LEFT_ALIGNMENT", LEFT_ALIGNMENT);
        lua.set("RIGHT_ALIGNMENT", RIGHT_ALIGNMENT);
        lua.set("CENTER_ALIGNMENT", CENTER_ALIGNMENT);
        lua.set("UP_ALIGNMENT", UP_ALIGNMENT);
        lua.set("DOWN_ALIGNMENT", DOWN_ALIGNMENT);

        lua.new_usertype<LuaWindow>("LuaWindow",
            sol::base_classes, sol::bases<LuaWidget>());
    }

    if(perms.acces_time)
    {
        lua.new_usertype<LuaTime>("time",
            "monotonic", &LuaTime::monotonic,
            "get", &LuaTime::get,
            "setInterval", &LuaTime::setInterval,
            "setTimeout", &LuaTime::setTimeout,
            "removeInterval", &LuaTime::removeInterval,
            "removeTimeout", &LuaTime::removeTimeout
        );

        lua["time"] = &lua_time;
    }

    if(perms.acces_web)
    {
        lua.new_usertype<LuaNetwork>("network",
            "HttpClient", &LuaNetwork::createHttpClient
        );

        lua["network"] = &lua_network;

        lua.new_usertype<LuaHttpClient>("HttpClient",
            "get", &LuaHttpClient::get,
            "post", &LuaHttpClient::post,
            sol::meta_function::garbage_collect, sol::destructor([](LuaHttpClient& obj) { obj.~LuaHttpClient(); })
        );
    }

    try
    {
        lua.script(code, sol::script_throw_on_error);
        lua.script("run()", sol::script_throw_on_error);

        while (!home_button::isPressed() && lua_gui.mainWindow != nullptr)
        {
            lua_gui.update();
            lua_time.update();
        }
    }
    catch (const sol::error& e)
    {
        std::cerr << "Erreur Lua : " << e.what() << std::endl;
        return;
    }
}