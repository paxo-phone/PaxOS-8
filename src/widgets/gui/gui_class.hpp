#ifndef GUI_CLASS
#define GUI_CLASS

#ifdef ESP32
    #include <Arduino.h>
    #include "soc/rtc_wdt.h"
    #include "esp_heap_caps.h"
    #include <esp_task_wdt.h>
#endif

#include <stdint.h>
#include <string.h>

#include <vector>

#include "../../interface/screen.hpp"
#include "../../tasks/tasks.hpp"

#include "../color.hpp"
#include "../alignment.hpp"
#include "../default.hpp"

#include "../touch_manager.hpp"

#include "../../app/CApp.hpp"

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
    WINDOW_BAR_TYPE,
    CANVAS_TYPE,
    IFRAME_TYPE
};

#define AUTO -1
#define AUTO_FULL -2

#define CONTROL_BAR_SIZE 25
#define LONG_PRESS_TIME 500

#define BASE_FONT_SIZE 16

inline LGFX_Sprite tft(&tft_root);

class Gui // widget system
{
    public:
    Gui();
    Gui(int16_t x, int16_t y, int16_t width, int16_t height); // constructeur personnalisé
    
    virtual ~Gui(); // supprime le widget

    virtual GUI_TYPE getType() = 0;  // retourne le type de l'objet selon GUI_TYPE

    void renderAll();           // genere un rendu de tous les enfants
    virtual void draw() = 0;    // draw objet
    virtual void afterRender();

    bool updateAll();       // update all children and itself 
    virtual bool update();  // update objet
    virtual void background_update() {};

    void reloadAlone();  // mise a jour sur les coordonées de l'objet
    void reloadParent();  // mise a jour qui concerne le parent
    void childrenAreDrawn();
    void parentAreNotRendered();
    void parentAreNotDrawn();

    virtual void updateSizes() {}
    void determineSize();
    void reloadWidget();


    virtual bool isFocused();   // return true if is touched    
    virtual bool isTouched(); // clique relaché sur l'élément


    void setX(int16_t x); // set relative x
    void setY(int16_t y); // set relative y
    void setWidth(int16_t width);   // set width
    void setHeight(int16_t height); // set height

    int16_t getX();            // get relative x
    int16_t getY();            // get relative y
    int16_t getWidth();        // get width
    int16_t getHeight();       // get height

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
    

    void enable(){this->enabled=true; reloadParent(); }
    void disable(){this->enabled=false; reloadParent(); }
    bool isEnabled(){ return this->enabled; } // retourne l'état d'activation de l'objet


    int16_t getLowestX();
    int16_t getHighestX();
    
    int16_t getLowestY();
    int16_t getHighestY();


    virtual void EventOnClick();
    virtual void EventOnLongClick();
    virtual void EventOnReleased();
    virtual void EventOnScreenReleased(){}
    virtual bool EventOnScroll();

    virtual void ClickEffect() {}
    virtual void ReleasedEffect() {}


    void (*onclick)(CApp *app, Gui* object, void* data) = nullptr;
    void (*onlongclick)(CApp *app, Gui* object, void* data) = nullptr;
    void (*onreleased)(CApp *app, Gui* object, void* data) = nullptr;
    void (*onscroll)(CApp *app, Gui* object, void* data) = nullptr;

    bool verticalSlide=false;
    bool horizontalSlide=false;

    CApp* appCallback = nullptr;
    void* dataCallback = nullptr;
    bool hasEvent = true;

    bool enabledBackground = true;

    std::vector<Gui *> children;
    Gui *parent = nullptr;

    bool autoSize = true;
    int16_t scroolX, scroolY = 0;          // scrool position

    bool rendered = false; // le buffer local est a jour
    bool drawn = false; // l'écran physique est a jour sur ces coordonées

    LGFX_Sprite l_tft;

    bool motionless = false;

    protected:

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
    
    int16_t x, y = 0;                      // position
    int16_t width, height = 0;             // sizes
    bool autoX, autoY, autoW, autoH = false;

    color_t color = COLOR_BLACK;
    color_t backgroundColor = COLOR_LIGHT;
    color_t borderColor = COLOR_BLACK;

    int16_t borderSize = DEFAULT_BORDER_SIZE;
    int16_t radius = DEFAULT_RADIUS;

    Alignment H_alignment = DEFAULT_H_ALIGNEMENT;
    Alignment V_alignment = DEFAULT_V_ALIGNEMENT;

    uint64_t timerPress = 0;

    bool enabled = true;
};

inline Gui *upFromDrawAll = nullptr;
inline Gui *mainWindow = nullptr;
inline bool drawing = false;
#endif
