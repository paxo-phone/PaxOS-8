#ifndef GUI_CPP
#define GUI_CPP

#include "gui.hpp"
#include "../interface/interface.hpp"

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
void flushScreen();
#endif

// Déclaration des variables statiques de la classe Gui
bool Gui::isScreenAlreadyPressed = false;
Gui* Gui::widgetPressed = nullptr;

// Constructeur par défaut de la classe Gui
Gui::Gui()
{
    // Initialisation des membres de la classe avec des valeurs par défaut
    this->x = 0;
    this->y = 0;
    this->width = 0;
    this->height = 0;

    autoX = false;
    autoY = false;
    autoW = false;
    autoH = false;

    lockedSlide = false;

    onclick = nullptr;
    onlongclick = nullptr;
    onreleased = nullptr;
    onscroll = nullptr;

    appCallback = nullptr;
    dataCallback = nullptr;

    verticalSlide = false;
    horizontalSlide = false;

    verticalSlide=false;
    horizontalSlide=false;

    statePress = 0;
    isTouchedState = false;
    enabledBackground = true;

    scroolX = 0; scroolY = 0;

    inercie = 0;
    
    enabled = true;
    autoSize = true;
    rendered = false;
    drawn = false;

    color = COLOR_BLACK;
    backgroundColor = COLOR_LIGHT;
    borderColor = COLOR_GREY;

    borderSize = DEFAULT_BORDER_SIZE;
    radius = DEFAULT_RADIUS;

    H_alignment = DEFAULT_H_ALIGNEMENT;
    V_alignment = DEFAULT_V_ALIGNEMENT;

    timerPress = 0;

    parent = nullptr;

    reloadAlone();
}

// Constructeur surchargé de la classe Gui
Gui::Gui(int16_t x, int16_t y, int16_t width, int16_t height)
{
    // Initialisation des membres de la classe avec les valeurs passées en paramètres
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    autoX = false;
    autoY = false;
    autoW = false;
    autoH = false;

    if(x == AUTO)
        autoX = true;
    if(y == AUTO)
        autoY = true;
    if(width == AUTO)
        autoW = true;
    if(height == AUTO)
        autoH = true;

    lockedSlide = false;

    onclick = nullptr;
    onlongclick = nullptr;
    onreleased = nullptr;
    onscroll = nullptr;

    appCallback = nullptr;
    dataCallback = nullptr;

    verticalSlide = false;
    horizontalSlide = false;

    verticalSlide=false;
    horizontalSlide=false;

    statePress = 0;
    isTouchedState = false;
    enabledBackground = true;

    scroolX = 0; scroolY = 0;

    inercie = 0;
    
    enabled = true;
    autoSize = true;

    color = COLOR_BLACK;
    backgroundColor = COLOR_LIGHT;
    borderColor = COLOR_GREY;

    borderSize = DEFAULT_BORDER_SIZE;
    radius = DEFAULT_RADIUS;

    H_alignment = DEFAULT_H_ALIGNEMENT;
    V_alignment = DEFAULT_V_ALIGNEMENT;

    timerPress = 0;

    rendered = false;
    drawn = false;

    parent = nullptr;

    reloadAlone();
}

// Destructeur de la classe Gui
Gui::~Gui()
{
    // Libération de la mémoire allouée pour les enfants de l'objet
    for(int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
        {
            delete children[i];
        }
    }
}

// Initialisation de l'écran, spécifique à la plateforme ESP32
void Gui::initScreen()
{
    #ifdef ESP32
        uint16_t calibrationData[] = {316, 194, 307, 3778+300, 3771-200, 204, 3740-200, 3750+300};

        tft_root.setTouchCalibrate(calibrationData);

        pinMode(14, OUTPUT);
        digitalWrite(14, 1);
        pinMode(22, OUTPUT);
        digitalWrite(22, 1);
    #endif
    
    tft_root.init();
}

// Méthode pour déterminer la taille de l'objet en fonction de son parent
void Gui::determineSize()
{
    if(parent!=nullptr)
    {
        if(autoX)
            setX(0);
        if(autoY)
        {
            if(parent->children[0] == this)
                setY(0);
            else
            {
                uint16_t i = 0;
                for(; i<parent->children.size() && parent->children[i] != this; i++);
                setY(parent->children[i-1]->getY() + parent->children[i-1]->getHeight() + 10);
            }
        }

        if(getType() == LABEL_TYPE && autoH)
            setHeight(reinterpret_cast<Label*>(this)->getTextHeight());
        
        if(autoW)
            width=parent->getWidth()-getRelativeX()*2;

        updateSizes();
    }
}

// Méthode pour rendre tous les éléments graphiques
void Gui::renderAll()
{
    determineSize();
    l_tft.setClipRect(0, 0, getWidth(), getHeight());

    if(parent!=nullptr)
    {
        updateSizes();
    }

    if(!isEnabled())
        return;

    if(parent!=nullptr && (getRelativeY() + getHeight() < 0 || getRelativeY() > parent->getHeight()))
    {
        return;
    }

    if(upFromDrawAll==nullptr)
        upFromDrawAll=this;
    
    if(!rendered) // si des changement necessitent de refaire le rendu
    {
        // faire le rendu
        if(getType() != IMAGE_TYPE && getType() != CANVAS_TYPE || children.size()!=0)
        {
            l_tft.deleteSprite();
            l_tft.setPsram(true);
            l_tft.setColorDepth(16);
            l_tft.createSprite(this->getWidth(), this->getHeight());

            if(parent == nullptr)
            {
                l_tft.fillSprite(COLOR_LIGHT);
            }
            else
            {
                if(!enabledBackground)
                    setBackgroundColor(parent->getBackgroundColor());
                
                l_tft.fillSprite(parent->getBackgroundColor());
            }
        }

        draw();
        
        for (int i = 0; i < children.size(); i++)
        {
            if(children[i] != nullptr)
            {
                l_tft.setClipRect(children[i]->getRelativeX(), children[i]->getRelativeY(), children[i]->getWidth(), children[i]->getHeight());
                children[i]->renderAll();
            }
        }

        afterRender();

        rendered = true; // le rendu a été effectué mais les modifications ne sont pas prises en compte sur l'écran
    }

    if ((!drawn || (parent!=nullptr && parent->rendered==false)) && getType() != IMAGE_TYPE)  // si des changements déja rendus necessitent d'être push sur l'écran ou le parent
    {
        if(parent != nullptr && parent->drawn == false)  // le parent demande le rendu
        {   // alors push les modifications vers le parent
            l_tft.pushSprite(&parent->l_tft, getRelativeX(), getRelativeY(), ALPHA_16B);
            console.log("on push to parent");
        }else   // le parent ne demande pas de rendu ou le parent n'existe pas
        {   // alors push les modifications vers l'écran
            int x1 = getAbsoluteX();
            int y1 = getAbsoluteY();
            tft_root.setClipRect(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight());

            l_tft.pushSprite(&tft_root, getAbsoluteX(), getAbsoluteY());
            
            childrenAreDrawn();
            console.log("on push to screen");
        }
    }

    /*if(getType() != IMAGE_TYPE)
    {
        if(upFromDrawAll==this)
        {
            upFromDrawAll=nullptr;
            if(parent != nullptr)
                l_tft.pushSprite(&tft_root, getAbsoluteX(), getAbsoluteY());
            else
                l_tft.pushSprite(&tft_root, getAbsoluteX(), getAbsoluteY(), ALPHA_16B);
        }
        else
        {
            l_tft.pushSprite(&parent->l_tft, getRelativeX(), getRelativeY(), ALPHA_16B);
        }
    }*/
    
    light::turnOn();
    #if defined(__linux__) || defined(_WIN32__) || defined(_WIN64__) || defined(__APPLE__)
        flushScreen();
    #endif
}

// Méthode pour effectuer des actions après le rendu
void Gui::afterRender()
{
    uint16_t maxH = getLowestY();
    uint16_t windowSize = getHeight() - CONTROL_BAR_SIZE;
    uint16_t slideBarSize = windowSize*windowSize / maxH;
    
    if(maxH > getHeight() && verticalSlide)
    {
        l_tft.fillRoundRect(getWidth()-2-6, 2 + (windowSize)*(-scroolY)/maxH, 6, slideBarSize, 3, COLOR_GREY);
    }
}

// Méthode pour mettre à jour tous les éléments
bool Gui::updateAll()
{
    if (!hasEvent)
        return false;
    
    if(parent == nullptr)
    {
        eventHandler.update();
        touch.update();
    }
    background_update();

    if(drawn == false)
    {
        renderAll();
    }

    if(!isEnabled())
        return false;

    bool children_updated = false;
    
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr && children[i]->getType() == KEYBOARD_TYPE)
            if(children[i]->updateAll())
            {
                children_updated=true;
                console.log("keyboard update");
            }
    }

    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr && children[i]->getType() != KEYBOARD_TYPE)
            if(children[i]->updateAll()) 
                children_updated=true;
    }

    if(children_updated || update())
    {
        return true;
    }

    return false;
}

// Méthode pour mettre à jour l'objet courant
bool Gui::update()
{
    if (widgetPressed != nullptr && widgetPressed != this)
        return false;
    
    bool focused = isFocused();
    bool screenTouched = touch.isTouch();

    bool returnValue = false;

    if (screenTouched && focused)
    {
        if (widgetPressed == nullptr)
        {
            console.log("Gui::update: " + std::to_string(getType()));
            widgetPressed = this;
            objectPressState = PRESSED;
            
            timerPress=millis();
            ClickEffect();

            EventOnClick();

            if(onclick!=nullptr)
            {
                onclick(appCallback, this, dataCallback);
                returnValue = true;
            }
        }
        else
        {
            if(objectPressState != PRESSED && timerPress+LONG_PRESS_TIME<millis())
            {
                EventOnLongClick();

                if(onlongclick!=nullptr)
                {
                    onlongclick(appCallback, this, dataCallback);
                    
                    return true;
                }

                objectPressState = RELEASED;
                ReleasedEffect();
            }

            if(touch.stateSlider)
            {
                objectPressState = SLIDED;

                if(onscroll!=nullptr)
                {
                    onscroll(appCallback, this, dataCallback);
                    return true;
                }

                if (!horizontalSlide && !verticalSlide && parent != nullptr)
                {
                    widgetPressed = parent;

                    parent->objectPressState = SLIDED;
                    this->objectPressState = NOT_PRESSED;


                    ReleasedEffect();
                }

                EventOnScroll();
            }
        }
    }

    if (screenTouched && !focused)
    {
        if(touch.stateSlider)
        {
            if(onscroll!=nullptr)
            {
                onscroll(appCallback, this, dataCallback);
                return true;
            }

            if (!horizontalSlide && !verticalSlide && parent != nullptr)
            {
                widgetPressed = parent;

                parent->objectPressState = SLIDED;
                this->objectPressState = NOT_PRESSED;

                ReleasedEffect();
            }

            EventOnScroll();
        }
    }

    if (!screenTouched && objectPressState != NOT_PRESSED)
    {
        widgetPressed = nullptr;
        ReleasedEffect();

        if (objectPressState == PRESSED)
        {
            isTouchedState = true;

            EventOnReleased();

            if(onreleased!=nullptr)
            {
                onreleased(appCallback, this, dataCallback);
            }
        }

        objectPressState = NOT_PRESSED;
    }

    return false;
}

void Gui::reloadAlone()
{
    this->drawn = false;
    this->rendered = false;
}

void Gui::reloadParent()
{
    this->drawn = false;
    if(parent != nullptr)
    {
        parent->rendered = false;
    }
}

void Gui::childrenAreDrawn()
{
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            children[i]->childrenAreDrawn();
    }

    drawn = true;
}

// Méthode pour vérifier si l'objet courant est focus
bool Gui::isFocused()
{
    return Touched(getAbsoluteX(), getAbsoluteY(), width, height); 
}

// Méthode pour vérifier si l'objet courant a été touché
bool Gui::isTouched()
{
    if(isTouchedState)
    {
        isTouchedState = false;
        return true;
    }
    return 0;
}

// Méthode pour recharger l'objet courant et ses enfants
void Gui::reloadWidget()
{
    if(parent!=nullptr)
        parent->reloadAlone();
    rendered=false;
}

// Méthode pour obtenir la coordonnée X de l'objet courant
int16_t Gui::getX()
{
    return this->x;
}

// Méthode pour obtenir la coordonnée Y de l'objet courant
int16_t Gui::getY()
{
    return this->y;
}

// Méthode pour obtenir la largeur de l'objet courant
int16_t Gui::getWidth()
{
    return this->width;
}

// Méthode pour obtenir la hauteur de l'objet courant
int16_t Gui::getHeight()
{
    return this->height;
}

// Méthode pour obtenir la coordonnée X absolue de l'objet courant
int16_t Gui::getAbsoluteX()
{
    if(parent==nullptr)
        return getRelativeX();
    else
        return getRelativeX() + parent->getAbsoluteX();
}

// Méthode pour obtenir la coordonnée Y absolue de l'objet courant
int16_t Gui::getAbsoluteY()
{
    if(parent==nullptr)
        return getRelativeY();
    else
        return getRelativeY() + parent->getAbsoluteY();
}

// Méthode pour obtenir la coordonnée X absolue fixe de l'objet courant
int16_t Gui::getAbsoluteFixX()
{
    if(parent==nullptr)
        return getRelativeFixX();
    else
        return getRelativeFixX() + parent->getAbsoluteFixX();
}

// Méthode pour obtenir la coordonnée Y absolue fixe de l'objet courant
int16_t Gui::getAbsoluteFixY()
{
    if(parent==nullptr)
        return getRelativeFixY();
    else
        return getRelativeFixY() + parent->getAbsoluteFixY();
}

// Méthode pour obtenir la coordonnée X relative de l'objet courant
int16_t Gui::getRelativeX()
{
    return this->getX() + ((parent!=nullptr && !motionless) ? (parent->getBorderSize()+ parent->getRadius()/2 + parent->scroolX) : (0));
}

// Méthode pour obtenir la coordonnée Y relative de l'objet courant
int16_t Gui::getRelativeY()
{
    return this->getY() + ((parent!=nullptr && !motionless) ? (parent->getBorderSize() + parent->getRadius()/2 + parent->scroolY) : (0));
}

int16_t Gui::getRelativeFixX()
{
    return this->getX() + ((parent!=nullptr && !motionless) ? (parent->getBorderSize()+ parent->getRadius()/2) : (0));
}

int16_t Gui::getRelativeFixY()
{
    return this->getY() + ((parent!=nullptr && !motionless) ? (parent->getBorderSize() + parent->getRadius()/2) : (0));
}

void Gui::setX(int16_t x)
{
    this->x=x;
    reloadParent();
}                               

void Gui::setY(int16_t y)
{
    this->y=y;
    reloadParent();
}

void Gui::setWidth(int16_t width)
{
    this->width=width;
    reloadParent();
}

void Gui::setHeight(int16_t height)
{
    this->height=height;
    reloadParent();
}

void Gui::addChild(Gui *child)
{
    if(child == nullptr)
        return;
    children.push_back(child);
    child->setParent(this);

    reloadAlone();
}

void Gui::removechildren()
{
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            delete children[i];
    }
    for (; children.size();)
    {
        children.erase(children.begin());
    }

    reloadAlone();
}

void Gui::setParent(Gui *parent)
{
    this->parent = parent;
    reloadParent();
}

Gui* Gui::getMaster()
{
    if(parent == nullptr)
        return this;
    else
        return parent->getMaster();
}

void Gui::setColor(color_t color)
{
    this->color=color;
    reloadAlone();
}

void Gui::setBackgroundColor(color_t backgroundColor)
{
    this->backgroundColor=backgroundColor;
    reloadAlone();
}

void Gui::setBorderColor(color_t borderColor)
{
    this->borderColor=borderColor;
    reloadAlone();
}

void Gui::setBorderSize(int16_t borderSize)
{
    this->borderSize=borderSize;
    reloadAlone();
}

color_t Gui::getColor()
{
    return this->color;
}

color_t Gui::getBackgroundColor()
{
    return ((l_tft.getColorDepth() == 8) ? (l_tft.color16to8(backgroundColor)) : (backgroundColor));
}

color_t Gui::getBorderColor()
{
    return this->borderColor;
}

int16_t Gui::getBorderSize()
{
    return this->borderSize;
}

void Gui::setHorizontalAlignment(Alignment alignment)
{
    this->H_alignment=alignment;
    reloadAlone();
}

void Gui::setVerticalAlignment(Alignment alignment)
{
    this->V_alignment=alignment;
    reloadAlone();
}

Alignment Gui::getHorizontalAlignment()
{
    return this->H_alignment;
}

Alignment Gui::getVerticalAlignment()
{
    return this->V_alignment;
}

void Gui::setRadius(uint16_t radius)
{
    this->radius=radius;
    reloadParent();
}

int16_t Gui::getLowestX()
{
    int16_t x = this->getAbsoluteX()+getWidth();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestX()+children[i]->getWidth()>x)
                x=children[i]->getLowestX()+children[i]->getWidth();
    }
    return x;
}

int16_t Gui::getHighestX()
{
    int16_t x = this->getAbsoluteX();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestX()<x)
                x=children[i]->getLowestX();
    }
    return x;
}

int16_t Gui::getLowestY()
{
    int16_t y = getAbsoluteFixY() + getHeight();

    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getAbsoluteFixY() + children[i]->getHeight()>y)
                y=children[i]->getAbsoluteFixY() + children[i]->getHeight();
    }

    return y;
}

int16_t Gui::getHighestY()
{
    int16_t y = this->getAbsoluteY();
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i] != nullptr)
            if(children[i]->getLowestY()<y)
                y=children[i]->getLowestY();
    }
    return y;
}

Gui* Gui::getParent()
{
    return this->parent; // warning, can return nullptr
}

void Gui::EventOnClick()
{
    return; // do nothing
}

void Gui::EventOnLongClick()
{
    return; // do nothing
}

void Gui::EventOnReleased()
{
    return; // do nothing
}

bool Gui::EventOnScroll()
{
    int16_t mvtX=touch.isSlidingHorizontally();
    int16_t mvtY=touch.isSlidingVertically();

    if((mvtX && horizontalSlide) || (mvtY && verticalSlide))
        reloadAlone();
    
    if(mvtX && horizontalSlide)
    {
        this->scroolX-=mvtX;
        touch.resetScrollHorizontal();
    }
    if(mvtY && verticalSlide)
    {
        if(getLowestY() + 20 > -(this->scroolY-mvtY) + this->height  && 0 < -(this->scroolY-mvtY))
        {
            this->scroolY-=mvtY;
            touch.resetScrollVertical();
        }else
        {
            if(getLowestY() + 20 <= -(this->scroolY-mvtY) + this->height)
                this->scroolY = this->height - 20 - getLowestY();
            if(-(this->scroolY-mvtY) <= 0)
                this->scroolY = 0;
            touch.resetScrollVertical();
        }
    }

    if((mvtX && horizontalSlide) || (mvtY && verticalSlide))
        renderAll();
    return mvtX || mvtY;
}

#endif
