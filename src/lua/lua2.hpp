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
    void setColor(color_t color){this->widget->setBackgroundColor(color);}
    color_t getColor(){return this->widget->getColor();}
    void enable(){this->widget->enable();}
    void disable(){this->widget->disable();}
    bool isTouched(){return this->widget->isFocused();}
    void onClick(sol::function func){onClickFunc = func;}

    void update();

    Gui* widget = nullptr;
    
    private:
    void addChild(LuaWidget* child){this->children.push_back(child); child->parent = this;}

    LuaWidget* parent = nullptr;
    std::vector<LuaWidget*> children;
    sol::function onClickFunc;
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

    Label* widget = nullptr;
    sol::function onChangeFunc;
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
    LuaWindow* window();

    void update();

    void setMainWindow(LuaWindow* window) {this->mainWindow = window; }
    LuaWindow* mainWindow = nullptr;
    LuaFile* lua = nullptr;
};

class LuaStorageFile
{
    /*public:
    LuaStorageFile(const std::string& filename);

    void open();
    void write(const std::string& text);
    char readChar();
    std::string readLine();
    std::string readAll();*/
};

class LuaStorage
{
    /*public:
    LuaStorage(LuaFile* lua);
    
    void file();
    void renameFile();
    void deleteFile();
    void newDir();
    void renameDir();
    void deleteDir();
    bool isDir();
    bool isFile();
    std::vector<std::string> listDir(std::string path);

    private:
    LuaFile* lua = nullptr;*/
};

struct Permissions
{
    bool acces_gui = true;
    bool acces_files = true;
    bool acces_files_root = true;
    bool acces_hardware = true;
};

class LuaFile
{
    public:
    LuaFile(std::string filename);
    ~LuaFile();

    void run();

    Permissions perms;
    std::string directory;

    private:
    std::string filename;
    Window* current_root;
    sol::state lua;

    LuaHardware lua_hardware;
    LuaGui lua_gui;
    LuaStorage lua_storage;
};