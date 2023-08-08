#ifndef PHONE_HPP
#define PHONE_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"
#include "../contact/contact.hpp"

class Phone : public App
{
    public:
    void main()
    {
        Window win("phone");

        Box* numberBox = new Box(AUTO, AUTO, AUTO, 60);
            win.addChild(numberBox);
            numberBox->setBorderSize(1); numberBox->setBorderColor(0);

            Label* numberLabel = new Label(AUTO, AUTO, AUTO, AUTO, "+33");
                numberBox->addChild(numberLabel);
                numberLabel->fontHeight=30;
                numberLabel->setHorizontalAlignment(CENTER_ALIGNMENT);
            Label* clicktogetcontact = new Label(AUTO, 42, AUTO, AUTO, "click to see contacts");
                numberBox->addChild(clicktogetcontact);
                clicktogetcontact->fontHeight=15;
                clicktogetcontact->setHorizontalAlignment(CENTER_ALIGNMENT);

        Box* keyboardnumber = new Box(30, 106, 235, 320);
            win.addChild(keyboardnumber);

            char* keys = "123456789+0 ";

            for (int i=0; i<12; i++)
            {
                string key = "";
                key+=keys[i];
                Button* button = new Button(85*(i%3), 85*int(i/3), 65, 65, key);
                button->setRadius(32);
                button->setBorderSize(0);
                button->setTextColor(COLOR_BLACK);
                button->fontHeight=20;
                button->setBackgroundColor(COLOR_EXTRA_LIGHT);
                button->setHorizontalAlignment(CENTER_ALIGNMENT);
                button->setVerticalAlignment(CENTER_ALIGNMENT);
                button->enabledBackground=true;
                keyboardnumber->addChild(button);

                if(keys[i]==' ')
                {
                    Image* callimg = new Image("apps/contact/con_num.png", 5, 5);
                    callimg->load();
                    callimg->hasEvent=false;
                    button->addChild(callimg);
                }
            }

        while (true)
        {
            win.updateAll();

            for (int i=0; i<11; i++) // check numbers and +
            {
                if(keyboardnumber->children[i]->isTouched())
                {
                    if(reinterpret_cast<Button*>(keyboardnumber->children[i])->getText()=="+")
                        numberLabel->setText("+33");
                    else
                        numberLabel->setText(numberLabel->getText() + reinterpret_cast<Button*>(keyboardnumber->children[i])->getText());
                }
            }

            if (numberLabel->isTouched())
            {
                uint contactIndex = Contact::contactPage();
                if(contactIndex != -1)
                    numberLabel->setText(Contact::contacts[contactIndex].number);
            }

            if(keyboardnumber->children[11]->isTouched()) // check call button
            {
                make_a_call(numberLabel->getText());
            }

            if (home_button.pressed())
            {
                break;
            }
        }
    }

    void make_a_call(std::string number);
    static void during_calling(std::string number);
    static void get_a_call();
};

#endif
