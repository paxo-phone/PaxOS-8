/* GUI SYSTEM */

#ifndef GUI_HPP
#define GUI_HPP

#include <vector>

#define WIDTH 320
#define HEIGHT 480

#ifdef BUILD_EMU
    #define LGFX_USE_V1
    #include "../interface/LovyanGFX/LovyanGFX.hpp"
    #include "../interface/LovyanGFX/LGFX_AUTODETECT.hpp"
    //#include "../sdl_main.cpp"
    LGFX tft_root(WIDTH, HEIGHT);
#endif
#ifdef BUILD_PAXO
    #define LGFX_USE_V1
    #include <LovyanGFX.hpp>
    #include "../interface/LovyanGFX/lovyangfx_config.hpp"
    LGFX tft_root;
#endif

#include "color.hpp"
#include "touchManager.hpp"
#include "../tasks/tasks.hpp"

class App;
typedef uint8_t Alignment;
typedef void event;

enum GUI_TYPE
{
    BASIC_TYPE,
    BOX_TYPE,
    BUTTON_TYPE,
    IMAGE_TYPE,
    KEYBOARD_TYPE,
    LABEL_TYPE,
    WINDOW_TYPE,
    SWITCH_TYPE,
    LIST_TYPE,
    WINDOW_BAR_TYPE
};

#define AUTO -1
#define AUTO_FULL -2

#define CONTROL_BAR_SIZE 25
#define LONG_PRESS_TIME 500

#define BASE_FONT_SIZE 16

LGFX_Sprite tft(&tft_root);

bool reload_afterunlocked = false;

class Gui // widget system
{
    public:
    void init(int16_t x, int16_t y, int16_t width, int16_t height); // constructeur personnalisé
    
    virtual ~Gui(); // supprime le widget

    static void initScreen(); // initialise l'ecran (tft_root)
    
    virtual GUI_TYPE getType() { return BASIC_TYPE; };  // retourne le type de l'objet selon GUI_TYPE

    void renderAll();           // genere un rendu de tous les enfants
    virtual void draw() = 0;    // draw objet
    virtual void afterRender() {};

    bool updateAll();       // update all children and itself 
    virtual bool update();  // update objet
    virtual void virtual_update() {};

    virtual void reload();
    virtual void updateSizes() {}
    void determineSize();
    void reloadWidget();


    virtual bool isFocuced();   // return true if is touched    
    virtual bool isTouched(); // clique relaché sur l'élément


    void setX(int16_t x); // set relative x
    void setY(int16_t y); // set relative y
    void setWidth(int16_t width);   // set width
    void setHeight(int16_t height); // set height

    int16_t getX();            // get relative x
    int16_t getY();            // get relative y
    int16_t getWidth();        // get width
    int16_t getHeight();       // get height

    void setMarginX(int16_t marginX); // set relative margin
    void setMarginY(int16_t marginY); // set relative margin

    int16_t getMarginX(); // get relative margin
    int16_t getMarginY(); // get relative margin

    virtual int16_t getAbsoluteX();    // get absolute position on the screen
    virtual int16_t getAbsoluteY();
    virtual int16_t getAbsoluteFixX();
    virtual int16_t getAbsoluteFixY();
    virtual int16_t getRelativeX();    // get sprite position relative to parent sprite
    virtual int16_t getRelativeY();
    virtual int16_t getRelativeFixX();
    virtual int16_t getRelativeFixY();


    void addChild(Gui *child);    // add child
    void removechildren();        // remove all children
    void setParent(Gui *parent);  // set parent

    Gui* getParent();
    Gui* getMaster();


    void setColor(color_t color);       // colors
    void setBackgroundColor(color_t backgroundColor);
    void setBorderColor(color_t borderColor);

    color_t getColor();         // get color
    color_t getBackgroundColor();   // get background color
    color_t getBorderColor();   // get border color


    void setBorderSize(int16_t borderSize); // set border size
    int16_t getBorderSize();   // get border size

    void setHorizontalAlignment(Alignment alignment); // set alignment
    void setVerticalAlignment(Alignment alignment); // set alignment
    Alignment getHorizontalAlignment(); // get alignment
    Alignment getVerticalAlignment(); // get alignment


    void setRadius(uint16_t radius); // set radius
    uint16_t getRadius(){ return this->radius; } // retourne le rayan de l'objet
    

    void enable(){this->enabled=true; reloadWidget(); }
    void disable(){this->enabled=false; reloadWidget(); }
    bool isEnabled(){ return this->enabled; } // retourne l'état d'activation de l'objet


    int16_t getLowestX();
    int16_t getHighestX();
    
    int16_t getLowestY();
    int16_t getHighestY();


    void setTheme(uint8_t theme);

    virtual void EventOnClick();
    virtual void EventOnLongClick();
    virtual void EventOnReleased();
    virtual void EventOnScreenReleased(){}
    virtual bool EventOnScroll();

    virtual void ClickEffect() {}
    virtual void ReleasedEffect() {}

    bool lockedSlide = false;
    bool isTouchedState = false;
    uint8_t statePress = 0; // 0=no 1=yes 2=wait realesed

    enum pressedState
    {
        NOT_PRESSED,
        PRESSED,
        SLIDED,
        RELEASED
    };

    uint8_t objectPressState = 0;
    static bool isScreenAlreadyPressed; // used for locking touch events for other widgets
    static Gui* widgetPressed;

    void (*onclick)(App *app, Gui* object, void* data) = nullptr;
    void (*onlongclick)(App *app, Gui* object, void* data) = nullptr;
    void (*onreleased)(App *app, Gui* object, void* data) = nullptr;
    void (*onscroll)(App *app, Gui* object, void* data) = nullptr;

    App* appCallback = nullptr;
    void* dataCallback = nullptr;
    bool hasEvent = true;

    bool verticalSlide=false;
    bool horizontalSlide=false;

    bool enabledBackground = true;

    LGFX_Sprite l_tft;
    bool rendered = false;

    bool noMargin = false;
    int16_t scroolX, scroolY = 0;          // scrool position

    std::vector<Gui *> children;
    Gui *parent = nullptr;
    
    protected:
    int16_t x, y = 0;                      // position
    int16_t width, height = 0;             // sizes
    bool autoX, autoY, autoW, autoH = false;

    int16_t marginX = 0;                   // marginX
    int16_t marginY = 0;                   // marginY

    int16_t inercie = 0;

    color_t color = theme_color[DEFAULT_THEME][0];
    color_t backgroundColor = theme_color[DEFAULT_THEME][1];
    color_t borderColor = theme_color[DEFAULT_THEME][2];

    int16_t borderSize = DEFAULT_BORDER_SIZE;
    int16_t radius = DEFAULT_RADIUS;

    Alignment H_alignment = DEFAULT_H_ALIGNEMENT;
    Alignment V_alignment = DEFAULT_V_ALIGNEMENT;

    unsigned int timerPress = 0;

    bool autoSize = true;
    bool enabled = true;


};

Gui *upFromDrawAll = nullptr;
Gui *mainWindow = nullptr;
bool drawing = false;
bool Gui::isScreenAlreadyPressed = false;
Gui* Gui::widgetPressed = nullptr;

#include "gui/box.hpp"
#include "gui/label.hpp"
#include "gui/button.hpp"
#include "gui/image.hpp"
#include "gui/window.hpp"
#include "gui/keyboard.hpp"
#include "gui/back.hpp"

#endif /* GUI_HPP */
