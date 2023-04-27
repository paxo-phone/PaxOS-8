#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"

class Snake : public App
{
    public:
    void main()
    {
        Window win("snake");

        while (true)
        {
            win.updateAll();
        }
    }
};

#endif