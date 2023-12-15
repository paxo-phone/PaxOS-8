#ifndef Keyboard_CPP
#define Keyboard_CPP

#include "gui_class.hpp"

#include "label.hpp"
#include "box.hpp"
#include "image.hpp"
#include "keyboard.hpp"
#include "../../interface/interface.hpp"

Keyboard::Keyboard() : Gui(15, 323, 290, 157)
{
    motionless = true;
    enabled=false;
    this->label=nullptr;
    
    Image* maj = new Image("system/keyboard/maj_"+to_string(mode)+".bmp", 10, 10, 15, 17);   // majuscule
    Image* type = new Image("system/keyboard/type_"+to_string(mode)+".bmp", 0, 2);   // type
    Image* erase = new Image("system/keyboard/delete.bmp", 30, 10);   // erase char

    maj->load(); maj->hasEvent=false;
    type->load(); type->hasEvent=false;
    erase->load(); erase->hasEvent=false;

    box_maj = new Box(0, 73, 58, 37);
    box_type = new Box(0, 110, 88, 36);
    box_erase = new Box(231, 73, 59, 36);

    box_maj->setBorderSize(0); box_maj->enabledBackground=false;
    box_type->setBorderSize(0); box_type->enabledBackground=false;
    box_erase->setBorderSize(0); box_erase->enabledBackground=false;

    space_label = new Label(88, 115, 112, 37);
    return_label = new Label(200, 115, 90, 37);

    space_label->setRadius(0); space_label->setBorderSize(0); space_label->enabledBackground=true; space_label->setBackgroundColor(COLOR_LIGHT); space_label->setTextColor(COLOR_BLACK);
    return_label->setRadius(0); return_label->setBorderSize(0); return_label->enabledBackground=true; return_label->setBackgroundColor(COLOR_LIGHT); return_label->setTextColor(COLOR_BLACK);

    space_label->setFontSize(17);
    return_label->setFontSize(17);

    space_label->setHorizontalAlignment(CENTER_ALIGNMENT);
    return_label->setHorizontalAlignment(CENTER_ALIGNMENT);

    space_label->setText("space");
    return_label->setText("return");

    space_label->setVerticalAlignment(CENTER_ALIGNMENT);
    return_label->setVerticalAlignment(CENTER_ALIGNMENT);

    box_maj->addChild(maj);
    box_type->addChild(type);
    box_erase->addChild(erase);

    this->addChild(box_maj);
    this->addChild(box_type);
    this->addChild(box_erase);
    this->addChild(space_label);
    this->addChild(return_label);
}

Keyboard::~Keyboard()
{
    removechildren();
}

void Keyboard::draw()
{
    l_tft.fillRect(0, getAbsoluteY(), 320, 200, getBackgroundColor());
    l_tft.setTextColor(COLOR_BLACK);

    for (uint8_t j = 0; j < 3; j++)
    {
        for (uint8_t i = 0; i < 10; i++)
        {
            if(alphabet[mode][j][i]!=' ')
            {
                std::string c = "";
                c.push_back(alphabet[mode][j][i]);
                l_tft.drawCentreString(c.c_str(), 15 + i * 29, 7 + j * 37, &fonts::Font4);
            }
        }
    }
}

char Keyboard::getKey()
{
    if(return_label->isTouched())
    {
        lastChar='\n';
        timer_last_char=millis();
        return '\n';
    }
    if(box_maj->isTouched()) // maj
    {
        switch(mode)
        {
            case 0:
                mode=1;
            break;
            case 1:
                mode=0;
            break;
            case 2:
                mode=3;
            break;
            case 3:
                mode=2;
            break;
        }
        reloadAlone();
        this->renderAll();
    }

    if(box_type->isTouched()) // mode
    {
        switch(mode)
        {
            case 0:
                mode=3;
            break;
            case 1:
                mode=3;
            break;
            case 2:
                mode=0;
            break;
            case 3:
                mode=0;
            break;
        }
        reloadAlone();
        this->renderAll();
    }

    if(box_erase->isTouched())
    {
        if(lastChar==KEY_ERASE)
        {
            delay(100);
        }
        lastChar=KEY_ERASE;
        timer_last_char=millis();
        return KEY_ERASE;
    }

    if(space_label->isTouched())
    {
        return ' ';
    }

    {   // doit être toujours verrifié
        for (uint8_t j = 0; j < 3; j++)
        {
            for (uint8_t i = 0; i < 10; i++)
            {
                if(Touched(getAbsoluteX() + i * 29, getAbsoluteY() + j * 37, 29, 37) && touch.isTouch())
                {
                    if(alphabet[mode][j][i]==' ')
                        break;
                    
                    tft_root.setTextColor(COLOR_PRIMARY);

                    std::string c = "";
                    c.push_back(alphabet[mode][j][i]);

                    tft_root.drawCentreString(c.c_str(), getAbsoluteX() + 15 + i * 29, getAbsoluteY() + 7 + j * 37, &fonts::Font4);
                    
                    while(touch.isTouch())
                    {
                        touch.update();
                    }
                    
                    tft_root.setTextColor(COLOR_BLACK);
                    tft_root.drawCentreString(c.c_str(), getAbsoluteX() + 15 + i * 29, getAbsoluteY() + 7 + j * 37, &fonts::Font4);
                    
                    lastChar=alphabet[mode][j][i];
                    timer_last_char=millis();
                    return alphabet[mode][j][i];
                }
            }
        }
    }
    
    return 0;
}

bool Keyboard::update()
{
    if(label==nullptr || !isEnabled())
        return false;

    char chr = getKey();

    //print(chr);

    if(chr!=0)
    {
        if(chr==KEY_ERASE)
            label->setText(label->getText().substr(0, label->getText().length()-1));
        else
            label->setText(label->getText() + chr);

        delay(10);
        return true;
    }
    return false;
}

void Keyboard::link(Label* label)
{
    Label* oldlabel = this->label;
    if(oldlabel!=nullptr)
        oldlabel->m_linked=false;
    this->label = label;

    if(label != nullptr)
    {
        label->m_linked=true;
        label->reloadAlone();
        enable();
    }else
    {
        disable();
    }
    if(oldlabel!=nullptr)
    {
        oldlabel->reloadAlone();
    }
}

#endif /* Keyboard_CPP */
