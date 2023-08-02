#ifndef MAP_HPP
#define MAP_HPP

#include "../../widgets/gui.hpp"
#include "../../interface/memory.hpp"

class Map : public App
{
    public:
    void main()
    {
        Window win("map");

        while (true)
        {
            win.updateAll();
        }
    }
};

#endif