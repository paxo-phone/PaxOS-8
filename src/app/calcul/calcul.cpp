#include "tinyexpr.h"

#include "calcul.hpp"
#include "../../widgets/gui.hpp"
#include "../../interface/filestream.hpp"
#include "../../interface/interface.hpp"
#include <algorithm>
#include <string>

void Calcul::launch()
{
    buildGui();
}

void Calcul::buildGui()
{
    Window window("");
    window.verticalSlide=false;

    std::string keys[] = {"^", "+/-", "%", "/", "7", "8", "9", "*", "4", "5", "6", "-", "1", "2", "3", "+", "0", ".", "AC", "="};

    Box* calculContent = new Box(25, 44, 250, 66);
        Label* oldCalculLabel = new Label(0, 0, 250, 16, "");
            oldCalculLabel->setFontSize(16);
            oldCalculLabel->setTextColor(COLOR_GREY);
            oldCalculLabel->setHorizontalAlignment(CENTER_ALIGNMENT);
            oldCalculLabel->setVerticalAlignment(CENTER_ALIGNMENT);
            oldCalculLabel->setRadius(0);

        Label* actualCalculLabel = new Label(0, 31, 250, 40, "");
            actualCalculLabel->setFontSize(40);
            actualCalculLabel->setTextColor(COLOR_BLACK);
            actualCalculLabel->setHorizontalAlignment(CENTER_ALIGNMENT);
            actualCalculLabel->setVerticalAlignment(CENTER_ALIGNMENT);
            actualCalculLabel->setRadius(0);
        calculContent->addChild(oldCalculLabel);
        calculContent->addChild(actualCalculLabel);

    Box* keyboard = new Box(20, 125, 280, 340);
        Label* AC_btn;
        Label* invert_btn;
        Label* equals_btn;
        for(int y = 0; y < 5; y++)
            for(int x = 0; x < 4; x++)
            {
                std::string buttonText = keys[x+y*4];
                Label* nbt = new Label(72.5*x, 70*y, 62, 60, keys[x+y*4]);
                nbt->setRadius(30);
                nbt->setFontSize(20);
                nbt->setTextColor(COLOR_BLACK);
                if(x==3 || (x==2 && y==0))
                    nbt->setBackgroundColor(COLOR_BEIGE);
                else
                    nbt->setBackgroundColor(COLOR_LIGHT);

                

                if(buttonText == "AC")
                    AC_btn=nbt;
                else if(buttonText == "+/-")
                {
                    invert_btn=nbt;
                    invert_btn->setFontSize(15);
                }
                else if(buttonText == "=")
                    equals_btn=nbt;
                else
                {
                    nbt->onreleased = addChar;
                    nbt->dataCallback = (void*) actualCalculLabel;
                }
                
                keyboard->addChild(nbt);
            }

    window.addChild(calculContent);
    window.addChild(keyboard);

    while (1)
    {
        window.updateAll();

        if(home_button::isPressed())
        {
            window.removechildren();
            return;
        }

        {
            int oldFontAL = actualCalculLabel->getFontSize();
            long textWidthAL = oldFontAL * actualCalculLabel->getText().size() / 2; // 2 is an approx for the font w-h ratio
            while (textWidthAL < 160 && actualCalculLabel->getFontSize() <= 39)
            {
                actualCalculLabel->setFontSize(actualCalculLabel->getFontSize()+1);
                textWidthAL =  actualCalculLabel->getFontSize() * actualCalculLabel->getText().size() / 2;
            }
            while (textWidthAL > 160)
            {
                actualCalculLabel->setFontSize(actualCalculLabel->getFontSize()-1);
                textWidthAL = actualCalculLabel->getFontSize() * actualCalculLabel->getText().size() / 2;
            }
    
            if(actualCalculLabel->getFontSize() != oldFontAL)
            {
                actualCalculLabel->reloadAlone();
                actualCalculLabel->updateAll();
            }
        }
        
        {
            int oldFontOL = oldCalculLabel->getFontSize();
            long textWidthOL = oldFontOL * oldCalculLabel->getText().size() / 2; // 2 is an approx for the font w-h ratio
            while (textWidthOL < 160 && oldCalculLabel->getFontSize() <= 15)
            {
                oldCalculLabel->setFontSize(oldCalculLabel->getFontSize()+1);
                textWidthOL = oldCalculLabel->getFontSize() * oldCalculLabel->getText().size() / 2;
            }
            while (textWidthOL > 160)
            {
                oldCalculLabel->setFontSize(oldCalculLabel->getFontSize()-1);
                textWidthOL = oldCalculLabel->getFontSize() * oldCalculLabel->getText().size() / 2;
            }
            if(oldCalculLabel->getFontSize() != oldFontOL)
            {
                oldCalculLabel->reloadAlone();
                oldCalculLabel->updateAll();
            }
        }

        if(equals_btn->isTouched())
        {
            std::string currentExpression = actualCalculLabel->getText();
            oldCalculLabel->setText(currentExpression);
            processExpression(currentExpression);
            actualCalculLabel->setText(currentExpression);
        }
        if(AC_btn->isTouched())
        {
            std::string currentExpression = actualCalculLabel->getText();
            processExpression(currentExpression);
            oldCalculLabel->setText(currentExpression);
            actualCalculLabel->setText("");
        }
        if (invert_btn->isTouched())
        {
            std::string currentInput = actualCalculLabel->getText();
            std::replace(currentInput.begin(), currentInput.end(), '-', 'p');
            std::replace(currentInput.begin(), currentInput.end(), '+', 'm');
            
            std::replace(currentInput.begin(), currentInput.end(), 'p', '+');
            std::replace(currentInput.begin(), currentInput.end(), 'm', '-');
            
            if (currentInput[0] != '-' && currentInput[0] != '+' && !currentInput.empty()) {
                currentInput = "-" + currentInput;
            }
            actualCalculLabel->setText(currentInput);
        }
        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            SDL_Delay(20);
        #endif
    }
}

void Calcul::processExpression(std::string &expression)
{
    bool shouldCalculate = false;
    // Check if the expression contains only digits, if yes then it won't process it.
    for (char chr : expression)
    {
        if (!(chr >= 48 && chr <= 57))
        {
            shouldCalculate = true;
        }
    }
    
    if(expression.length() != 0 && shouldCalculate)
    {
        /*char * chr =(char *) malloc(str.length());
        strcpy(chr, str.c_str());
        str = std::to_string(te_interp(chr, 0));*/
        
        parser ob;
        expression=std::to_string(ob.eval_exp((char*) expression.c_str()));
        //print("result: " + expression);
        while(expression[expression.length()-1]=='0')
            expression.erase(expression.length()-1);
        if(expression[expression.length()-1]=='.')
            expression.erase(expression.length()-1);
    }
}

void Calcul::addChar(App *app, Gui* objectPrt, void* data)
{
    std::string chr = reinterpret_cast<Label*>(objectPrt)->getText();
    Label* label = reinterpret_cast<Label*>(data);
    std::string currentInput = label->getText();
    if (currentInput.empty())
    {
        if (chr == "*" || chr == "/" || chr == "%" || chr == "^")
        {
            return;
        }
    } else
    {
        const char lastChar = currentInput[currentInput.size() - 1];
        if (strchr("+-*/%^", lastChar) && (chr == "*" || chr == "/" || chr == "%" || chr == "^"))
        {
            return;
        }
        if (strchr("+-", lastChar) && (chr == "+" || chr == "-"))
        {
            return;
        }
    }
    label->setText(label->getText()+chr);
}