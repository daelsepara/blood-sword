#ifndef __INTERFACE__HPP__
#define __INTERFACE__HPP__

#include "BloodSword.hpp"

#include "graphics.hpp"

namespace Interface
{
    bool Combat(SDL_Window *window, SDL_Renderer *renderer, std::vector<std::string> &map, Party::Base &party, std::vector<Monster::Base> monsters)
    {
        auto TacticalMap = TacticalMap::Base();

        TacticalMap.Convert(map);

        // Offsets used to display tactical map
        auto MapX = 0;

        auto MapY = 0;

        // Size of viewable grid
        auto SizeX = 10;

        auto SizeY = 10;

        if (window && renderer)
        {
            Graphics::FillRect(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, intBE);

            if (MapX < 0)
            {
                MapX = 0;
            }

            if (MapX > (TacticalMap.SizeX - 1) - SizeX)
            {
                MapX = (TacticalMap.SizeX - 1) - SizeX;
            }

            if (MapY < 0)
            {
                MapY = 0;
            }

            if (MapY > (TacticalMap.SizeY - 1) - SizeX)
            {
                MapY = (TacticalMap.SizeY - 1) - SizeY;
            }
        }

        return false;
    }
}
#endif