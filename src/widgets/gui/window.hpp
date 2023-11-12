#ifndef WINDOW_GUI
#define WINDOW_GUI

#include "box.hpp"
#include "keyboard.hpp"
#include "label.hpp"

class Image;

class Window : public Gui
{
    public:
    const static int WINDOW_WIDTH = 320;
    const static int WINDOW_HEIGHT = 480;
    int updateEventId;

    Window(std::string title);

    ~Window();

    void updateModules();

    void draw();

    void afterRender();

    void virtual_update();

    std::string title = "";
    Box* bar = nullptr;
    Label* hourLabel = nullptr;

    Image* netQual[5];
    Image* battLevel[5];

    int OldnetQual = 0;
    int OldbattLevel = 0;
    std::string oldhourString = "";

    Keyboard* keyboard = nullptr;
};

#endif
