#ifndef __INTERFACE__HPP__
#define __INTERFACE__HPP__

#include "BloodSword.hpp"

#include "graphics.hpp"

namespace Interface
{
    bool Combat(SDL_Window *window, SDL_Renderer *renderer, std::vector<std::string> &map, Party::Base &party, std::vector<Monster::Base> &monsters)
    {
        auto TacticalMap = TacticalMap::Base();

        TacticalMap.Convert(map, party, monsters);

        // Offsets used to display tactical map
        auto MapX = 0;

        auto MapY = 0;

        auto ObjectSize = 64;

        auto PaddingX = 2 * startx + ObjectSize;

        auto PaddingY = 3 * starty + ObjectSize;

        // Size of viewable grid
        auto SizeX = (SCREEN_WIDTH - 2 * PaddingX) / ObjectSize;

        auto SizeY = (SCREEN_HEIGHT - 2 * PaddingY) / ObjectSize;

        if (SizeX > TacticalMap.SizeX)
        {
            SizeX = TacticalMap.SizeX;
        }

        if (SizeY > TacticalMap.SizeY)
        {
            SizeY = TacticalMap.SizeY;
        }

        // Draw map at the center of the screen
        auto DrawX = (SCREEN_WIDTH - SizeX * ObjectSize) / 2;

        auto DrawY = (SCREEN_HEIGHT - SizeY * ObjectSize) / 2;

        auto BorderW = SCREEN_WIDTH - 4 * startx;

        auto BorderH = SCREEN_HEIGHT - 2 * PaddingY;

        if (window && renderer && TacticalMap.SizeX > 0 && TacticalMap.SizeY > 0)
        {
            Graphics::FillWindow(renderer, intBE);

            if (MapX < 0)
            {
                MapX = 0;
            }

            if (MapX > TacticalMap.SizeX - SizeX)
            {
                MapX = TacticalMap.SizeX - SizeX;
            }

            if (MapY < 0)
            {
                MapY = 0;
            }

            if (MapY > TacticalMap.SizeY - SizeY)
            {
                MapY = TacticalMap.SizeY - SizeY;
            }

            // Drap Map Window Borders
            Graphics::FillRect(renderer, BorderW, ObjectSize, 2 * startx, 3 * starty, intBK);
            Graphics::FillRect(renderer, BorderW, ObjectSize, 2 * startx, SCREEN_HEIGHT - PaddingY, intBK);
            Graphics::FillRect(renderer, ObjectSize, BorderH, 2 * startx, PaddingY, intBK);
            Graphics::FillRect(renderer, ObjectSize, BorderH, SCREEN_WIDTH - PaddingX, PaddingY, intBK);

            // Render Objects within the Map Window
            for (auto y = MapY; y < MapY + SizeY; y++)
            {
                auto AssetY = DrawY + (y - MapY) * ObjectSize;

                for (auto x = MapX; x < MapX + SizeX; x++)
                {
                    auto AssetX = DrawX + (x - MapX) * ObjectSize;

                    auto object = TacticalMap.Objects[y][x];

                    if (object == TacticalMap::Object::Wall)
                    {
                        Graphics::StretchImage(renderer, Graphics::GetAsset(TacticalMap::Object::Wall), AssetX, AssetY, ObjectSize, ObjectSize);
                    }
                    else if (object == TacticalMap::Object::Warrior)
                    {
                        Graphics::StretchImage(renderer, Graphics::GetAsset(TacticalMap::Object::Warrior), AssetX, AssetY, ObjectSize, ObjectSize);
                    }
                    else if (object == TacticalMap::Object::HotCoals)
                    {
                        Graphics::StretchImage(renderer, Graphics::GetAsset(TacticalMap::Object::HotCoals), AssetX, AssetY, ObjectSize, ObjectSize);
                    }
                    else
                    {
                        Graphics::StretchImage(renderer, Graphics::GetAsset(TacticalMap::Object::Passable), AssetX, AssetY, ObjectSize, ObjectSize);
                    }
                }
            }

            Input::WaitForNext(renderer);
        }

        return false;
    }
}
#endif