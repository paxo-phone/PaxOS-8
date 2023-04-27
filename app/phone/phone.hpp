#ifndef PHONE_HPP
#define PHONE_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"

class Phone : public App
{
    public:
    void main()
    {
        Window win("phone");

        while (true)
        {
            win.updateAll();
        }
    }
};

#endif