#include "../lib/SOL2/sol.hpp"

#include "../widgets/gui.hpp"

class Window;

class LuaHardware
{
    public:
    void flash(bool flash);
};

class LuaWidget
{
    public:
    void init(Gui* obj, LuaWidget* parent, int x, int y, int width, int height);
    void init(Gui* obj) {widget = obj;}
    ~LuaWidget();

    void setX(int x){this->widget->setX(x);}
    void setY(int y){this->widget->setY(y);}
    void setWidth(int width){this->widget->setWidth(width);}
    void setHeight(int height){this->widget->setHeight(height);}
    int getX(){return this->widget->getX();}
    int getY(){return this->widget->getY();}
    int getWidth(){return this->widget->getWidth();}
    int getHeight(){return this->widget->getHeight();}
    void setBackgroundColor(color_t color){this->widget->setBackgroundColor(color);}
    color_t getColor(){return this->widget->getColor();}
    void enable(){this->widget->enable();}
    void disable(){this->widget->disable();}
    bool isTouched(){return this->widget->isFocused();}
    void onClick(sol::function func){onClickFunc = func; widget->hasEvent = true;}
    void onScrollUp(sol::function func){onScrollUpFunc = func;}
    void onScrollDown(sol::function func){onScrollDownFunc = func;}
    void onScrollRight(sol::function func){onScrollRightFunc = func;}
    void onScrollLeft(sol::function func){onScrollLeftFunc = func;}

    void update();

    Gui* widget = nullptr;
    
    private:
    void addChild(LuaWidget* child){this->children.push_back(child); child->parent = this;}

    LuaWidget* parent = nullptr;
    std::vector<LuaWidget*> children;
    sol::function onClickFunc;
    sol::function onScrollUpFunc;
    sol::function onScrollDownFunc;
    sol::function onScrollRightFunc;
    sol::function onScrollLeftFunc;
};

class LuaBox : public LuaWidget
{
    public:
    LuaBox(LuaWidget* parent, int x, int y, int width, int height);

    Box* widget = nullptr;
};

class LuaImage : public LuaWidget
{
    public:
    LuaImage(LuaWidget* parent, const std::string& path, int x, int y, int width, int height);

    Image* widget = nullptr;
};

class LuaLabel : public LuaWidget
{
    public:
    LuaLabel(LuaWidget* parent, int x, int y, int width, int height);
    void setText(const std::string& text){ widget->setText(text); }
    std::string getText(){ return widget->getText(); }
    void setFontSize(int fontSize){ widget->setFontSize(fontSize); }
    int getTextHeight(){ return widget->getTextHeight(); }
    void setBold(bool bold){ widget->setBold(bold); }
    void setItalic(bool italic){ widget->setItalic(italic); }
    void setVerticalAlignment(int alignment){ widget->setVerticalAlignment(alignment); }
    void setHorizontalAlignment(int alignment){ widget->setHorizontalAlignment(alignment); }

    Label* widget = nullptr;
};

class LuaInput : public LuaWidget
{
    public:
    LuaInput(LuaWidget* parent, int x, int y, int width, int height);
    void setText(const std::string& text){ widget->setText(text); }
    std::string getText(){ return widget->getText(); }
    void setFontSize(int fontSize){ widget->setFontSize(fontSize); }
    int getTextHeight(){ return widget->getTextHeight(); }
    void onChange(sol::function func){ onChangeFunc = func;}
    void setVerticalAlignment(int alignment){ widget->setVerticalAlignment(alignment); }
    void setHorizontalAlignment(int alignment){ widget->setHorizontalAlignment(alignment); }

    Label* widget = nullptr;
    sol::function onChangeFunc;
};

class LuaButton : public LuaWidget
{
    public:
    LuaButton(LuaWidget* parent, int x, int y, int width, int height);
    void setText(const std::string& text){ widget->setText(text); }
    std::string getText(){ return widget->getText(); }
    void setFontSize(int fontSize){ widget->setFontSize(fontSize); }
    int getTextHeight(){ return widget->getTextHeight(); }
    void setVerticalAlignment(int alignment){ widget->setVerticalAlignment(alignment); }
    void setHorizontalAlignment(int alignment){ widget->setHorizontalAlignment(alignment); }

    Button* widget = nullptr;
};

class LuaWindow : public LuaWidget
{
    public:
    LuaWindow();

    Window* widget = nullptr;
};

class LuaFile;

class LuaGui
{
    public:
    LuaGui(LuaFile* lua);
    LuaBox* box(LuaWidget* parent, int x, int y, int width, int height);
    LuaImage* image(LuaWidget* parent, const std::string& path, int x, int y, int width, int height);
    LuaLabel* label(LuaWidget* parent, int x, int y, int width, int height);
    LuaInput* input(LuaWidget* parent, int x, int y, int width, int height);
    LuaButton* button(LuaWidget* parent, int x, int y, int width, int height);
    LuaWindow* window();

    void update();

    void setMainWindow(LuaWindow* window) {this->mainWindow = window; }
    LuaWindow* mainWindow = nullptr;
    LuaFile* lua = nullptr;
};

class LuaStorageFile
{
    public:
    LuaStorageFile(const std::string& filename, bool mode = false);

    void open();
    void close();
    void write(const std::string& text);
    char readChar();
    std::string readLine();
    std::string readAll();

    private:
    bool mode;  // false->read;   true->write;
    std::string filename;
    storage::FileStream file;
};

class LuaStorage
{
    public:
    LuaStorage(LuaFile* lua);
    
    LuaStorageFile* file(const std::string& filename, bool mode);
    bool newDir(const std::string& path);
    bool renameFile(const std::string& oldpath, const std::string& newpath);
    bool renameDir(const std::string& oldpath, const std::string& newpath);
    bool deleteFile(const std::string& path);
    bool deleteDir(const std::string& text);
    bool isDir(const std::string& text);
    bool isFile(const std::string& text);
    std::vector<std::string> listDir(const std::string& path);

    bool legalPath(const std::string& path);
    std::string convertPath(const std::string& path);

    private:
    LuaFile* lua = nullptr;
};

class LuaTimeInterval
{
    public:
    LuaTimeInterval(LuaFile* lua, sol::function func, uint32_t interval);
    int getId();
    void call();
    ~LuaTimeInterval();

    private:
    LuaFile* lua;
    sol::function func;
    uint32_t interval;
    int id;
};

class LuaTimeTimeout
{
    public:
    LuaTimeTimeout(LuaFile* lua, sol::function func, uint32_t timeout);
    int getId();
    void call();
    ~LuaTimeTimeout();

    bool done = false;

    private:
    LuaFile* lua;
    sol::function func;
    uint32_t timeout;
    int id;
};

class LuaTime
{
    public:
    LuaTime(LuaFile* lua);
    ~LuaTime();
    void update();
    
    uint32_t monotonic();
    sol::table get(std::string format);
    int setInterval(sol::function func, uint32_t interval);
    int setTimeout(sol::function func, uint32_t timeout);
    void removeInterval(int id);
    void removeTimeout(int id);

    private:
    LuaFile* lua = nullptr;
    uint32_t timerFromStart = 0;

    std::vector<LuaTimeInterval*> intervals;
    std::vector<LuaTimeTimeout*> timeouts;
};

#include "../network/network.hpp"

class LuaHttpClient
{
    public:
    LuaHttpClient(LuaFile* lua);
    ~LuaHttpClient();
    std::string get(std::string url);
    std::string post(std::string url);

    network::HttpClient httpClient;

    private:
    LuaFile* lua;
};

class LuaNetwork
{
    public:
    LuaNetwork(LuaFile* lua);
    std::shared_ptr<LuaHttpClient> createHttpClient();

    private:
    LuaFile* lua;
};

struct Permissions
{
    bool acces_gui = true;
    bool acces_files = true;
    bool acces_files_root = true;
    bool acces_hardware = true;
    bool acces_time = true;
    bool acces_web_paxo = true;
    bool acces_web = true;
};

class LuaFile
{
    public:
    LuaFile(std::string filename);
    ~LuaFile();

    void run();

    Permissions perms;
    std::string directory;

    sol::state lua;

    private:
    std::string filename;
    Window* current_root;

    LuaHardware lua_hardware;
    LuaGui lua_gui;
    LuaStorage lua_storage;
    LuaTime lua_time;
    LuaNetwork lua_network;
};