#include "tinyexpr.h"

#include "../../drivers/drivers.hpp"
#include "../../modules/modules.hpp"

#include "calcul.hpp"

void Calcul::launch()
{
    buildGui();
}

void Calcul::buildGui()
{
    Window window("");
    window.setMarginX(0);
    window.setMarginY(0);
    window.verticalSlide=false;

    std::string keys[] = {"^", "+/-", "%", "/", "7", "8", "9", "*", "4", "5", "6", "-", "1", "2", "3", "+", "0", ".", "AC", "="};

    Box* calculContent = new Box(25, 44, 250, 66);
        Label* oldCalculLabel = new Label(0, 0, 250, 16, "");
            oldCalculLabel->fontHeight=16;
            oldCalculLabel->setTextColor(COLOR_GREY);
            oldCalculLabel->setHorizontalAlignment(CENTER_ALIGNMENT);
            oldCalculLabel->setVerticalAlignment(CENTER_ALIGNMENT);
            oldCalculLabel->setRadius(0);

        Label* actualCalculLabel = new Label(0, 31, 250, 40, "");
            actualCalculLabel->fontHeight=40;
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
                nbt->fontHeight=20;
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
                    invert_btn->fontHeight = 15;
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

        if(home_button.pressed())
        {
            window.removechildren();
            return;
        }

        {
            int oldFontAL = actualCalculLabel->fontHeight;
            long textWidthAL = oldFontAL * actualCalculLabel->getText().size() / 2; // 2 is an approx for the font w-h ratio
            while (textWidthAL < 160 && actualCalculLabel->fontHeight <= 39)
            {
                textWidthAL =  ++actualCalculLabel->fontHeight * actualCalculLabel->getText().size() / 2;
            }
            while (textWidthAL > 160)
            {
                textWidthAL = --actualCalculLabel->fontHeight * actualCalculLabel->getText().size() / 2;
            }
    
            if(actualCalculLabel->fontHeight != oldFontAL)
            {
                actualCalculLabel->rendered = false;
                actualCalculLabel->updateAll();
            }
        }
        
        {
            int oldFontOL = oldCalculLabel->fontHeight;
            long textWidthOL = oldFontOL * oldCalculLabel->getText().size() / 2; // 2 is an approx for the font w-h ratio
            while (textWidthOL < 160 && oldCalculLabel->fontHeight <= 15)
            {
                textWidthOL = ++oldCalculLabel->fontHeight * oldCalculLabel->getText().size() / 2;
            }
            while (textWidthOL > 160)
            {
                textWidthOL = --oldCalculLabel->fontHeight * oldCalculLabel->getText().size() / 2;
            }
            if(oldCalculLabel->fontHeight != oldFontOL)
            {
                oldCalculLabel->rendered = false;
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
        #ifdef BUILD_EMU
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
