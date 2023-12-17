#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <string>
#include <algorithm>

#include "../CApp.hpp"

class Gui;

class Calcul : public CApp
{

public:
    [[nodiscard]] const char* getAppName() const override { return "calcul"; }
    void main() override;

    void buildGui();
    void processExpression(std::string &equation);
    static void addChar(CApp *app, Gui* objectPrt, void* data);
};

#endif
