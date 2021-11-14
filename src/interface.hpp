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

        auto hold = false;
        auto selected = false;
        auto scroll_up = false;
        auto scroll_down = false;

        auto current = 0;

        auto done = false;

        if (window && renderer && TacticalMap.SizeX > 0 && TacticalMap.SizeY > 0)
        {
            while (!done)
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

                auto num_controls = 0;

                auto controls = std::vector<Button>();

                controls.push_back(Button(num_controls, "icons/back-button.png", 0, 0, 0, 0, lastx, buttony, intGR, Control::Type::BACK));

                // Render Objects within the Map Window
                for (auto y = MapY; y < MapY + SizeY; y++)
                {
                    auto AssetY = DrawY + (y - MapY) * ObjectSize;

                    for (auto x = MapX; x < MapX + SizeX; x++)
                    {
                        auto AssetX = DrawX + (x - MapX) * ObjectSize;

                        auto object = TacticalMap.Objects[y][x];

                        auto objectID = TacticalMap.ObjectID[y][x] - 1;

                        Graphics::StretchImage(renderer, Graphics::GetAsset(Graphics::AssetType::Passable), AssetX, AssetY, ObjectSize, ObjectSize);

                        if (object == TacticalMap::Object::Wall)
                        {
                            Graphics::StretchImage(renderer, Graphics::GetAsset(Graphics::AssetType::Wall), AssetX, AssetY, ObjectSize, ObjectSize);
                        }
                        else if (object == TacticalMap::Object::Player)
                        {
                            if (party.Members[objectID].Class == Character::Class::Warrior)
                            {
                                Graphics::StretchImage(renderer, Graphics::GetAsset(Graphics::AssetType::Warrior), AssetX, AssetY, ObjectSize, ObjectSize);
                            }
                        }
                        else if (object == TacticalMap::Object::HotCoals)
                        {
                            Graphics::StretchImage(renderer, Graphics::GetAsset(Graphics::AssetType::Wall), AssetX, AssetY, ObjectSize, ObjectSize);
                        }
                    }
                }

                Graphics::RenderButtons(renderer, controls, current, border_space, border_pts);

                Input::GetInput(renderer, controls, current, selected, scroll_up, scroll_down, hold);

                if ((selected && current >= 0 && current < controls.size()) || scroll_up || scroll_down || hold)
                {
                    if (controls[current].Type == Control::Type::BACK && !hold)
                    {
                        current = -1;

                        done = true;
                    }
                }
            }
        }

        return false;
    }
}
#endif