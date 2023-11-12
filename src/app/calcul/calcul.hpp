#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <string>
#include <algorithm>
#include "../app.hpp"

class Gui;

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
    static void addChar(App *app, Gui* objectPrt, void* data);
};

#endif
