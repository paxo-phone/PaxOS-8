#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <algorithm>
#include <string>

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"

class Calcul : public CppAppContainer
{
    public:
    void main()
    {
        launch();
    }


    void launch();

    void buildGui();
    
    void processExpression(std::string &equation);

    static void addChar(App *app, Gui* objectPrt, void* data)
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
};

#endif
