#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"

class Calcul : public App
{
    public:
    void main()
    {
        launch();
    }

    void launch();

    void buildGui();

    static void addChar(App *app, Gui* objectPrt, void* data)
    {
        std::string chr = reinterpret_cast<Label*>(objectPrt)->getText();
        Label* label = reinterpret_cast<Label*>(data);

        label->setText(label->getText()+chr);
    }
};

#endif