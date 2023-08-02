#ifndef HOUR_HPP
#define HOUR_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"

class Hour : public App
{
    public:
    void main()
    {
        Window win("hour");

        while (true)
        {
            win.updateAll();
        }
    }
};

#endif