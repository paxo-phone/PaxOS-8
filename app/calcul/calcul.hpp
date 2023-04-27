#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"

class Calcul : public App
{
    public:
    void main()
    {
        Window win("calcul");

        while (true)
        {
            win.updateAll();
        }
    }
};

#endif