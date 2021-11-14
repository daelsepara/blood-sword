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

                auto controls = std::vector<Button>();

                controls.push_back(Button(0, "icons/up.png", 1, 2, 0, 5 + (SizeX / 2), (SCREEN_WIDTH - ObjectSize) / 2, DrawY - ObjectSize - 2 * border_space, MapY > 0 ? intLB : intGR, Control::Type::MAP_UP));
                controls.push_back(Button(1, "icons/left.png", 1, 5 + (SizeY / 2 * SizeX) - SizeX, 0, 3, DrawX - (ObjectSize + 2 * border_space), (SCREEN_HEIGHT - ObjectSize) / 2, MapX > 0 ? intLB : intGR, Control::Type::MAP_LEFT));
                controls.push_back(Button(2, "icons/right.png", 4 + (SizeX * SizeY / 2), 2, 0, 3, DrawX + (SizeX * ObjectSize + 2 * border_space), (SCREEN_HEIGHT - ObjectSize) / 2, (MapX < TacticalMap.SizeX - SizeX) ? intLB : intGR, Control::Type::MAP_RIGHT));
                controls.push_back(Button(3, "icons/down.png", 1, 2, 4 + (SizeX * SizeY) - SizeX / 2, 4, (SCREEN_WIDTH - ObjectSize) / 2, DrawY + (SizeY * ObjectSize + 2 * border_space), (MapY < TacticalMap.SizeY - SizeY) ? intLB : intGR, Control::Type::MAP_DOWN));
                controls.push_back(Button(4, "icons/back-button.png", 3, 4, 3, 4, lastx, buttony, intGR, Control::Type::BACK));

                int num_controls = controls.size();

                // Render Objects within the Map Window
                for (auto y = MapY; y < MapY + SizeY; y++)
                {
                    auto AssetY = DrawY + (y - MapY) * ObjectSize;

                    auto ctrl_y = (y - MapY);

                    auto ctrl_up = num_controls;
                    auto ctrl_dn = num_controls;
                    auto ctrl_lt = num_controls;
                    auto ctrl_rt = num_controls;

                    for (auto x = MapX; x < MapX + SizeX; x++)
                    {
                        auto ctrl_x = (x - MapX);

                        if (ctrl_y > 0)
                        {
                            ctrl_up = num_controls - SizeX;
                        }
                        else
                        {
                            ctrl_up = 0;
                        }

                        if (ctrl_y < SizeY - 1)
                        {
                            ctrl_dn = num_controls + SizeX;
                        }
                        else
                        {
                            ctrl_dn = 3;
                        }

                        if (ctrl_x > 0)
                        {
                            ctrl_lt = num_controls - 1;
                        }
                        else
                        {
                            ctrl_lt = 1;
                        }

                        if (ctrl_x < SizeX - 1)
                        {
                            ctrl_rt = num_controls + 1;
                        }
                        else
                        {
                            ctrl_rt = 2;
                        }

                        auto AssetX = DrawX + (x - MapX) * ObjectSize;

                        auto object = TacticalMap.Objects[y][x];

                        auto objectID = TacticalMap.ObjectID[y][x] - 1;

                        if (object == TacticalMap::Object::Wall)
                        {
                            controls.push_back(Button(num_controls, Graphics::GetAsset(Graphics::AssetType::Wall), ctrl_lt, ctrl_rt, ctrl_up, ctrl_dn, AssetX, AssetY, intDB, Control::Type::NONE));
                        }
                        else if (object == TacticalMap::Object::Player)
                        {
                            if (party.Members[objectID].Class == Character::Class::Warrior)
                            {
                                controls.push_back(Button(num_controls, Graphics::GetAsset(Graphics::AssetType::Warrior), ctrl_lt, ctrl_rt, ctrl_up, ctrl_dn, AssetX, AssetY, intDB, Control::Type::WARRIOR));
                            }
                        }
                        else if (object == TacticalMap::Object::HotCoals)
                        {
                            controls.push_back(Button(num_controls, Graphics::GetAsset(Graphics::AssetType::HotCoals), ctrl_lt, ctrl_rt, ctrl_up, ctrl_dn, AssetX, AssetY, intDB, Control::Type::NONE));
                        }
                        else
                        {
                            controls.push_back(Button(num_controls, Graphics::GetAsset(Graphics::AssetType::Passable), ctrl_lt, ctrl_rt, ctrl_up, ctrl_dn, AssetX, AssetY, intDB, Control::Type::NONE));
                        }

                        num_controls++;
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
                    else if (controls[current].Type == Control::Type::MAP_UP && !hold)
                    {
                        MapY--;
                    }
                    else if (controls[current].Type == Control::Type::MAP_DOWN && !hold)
                    {
                        MapY++;
                    }
                    else if (controls[current].Type == Control::Type::MAP_LEFT && !hold)
                    {
                        MapX--;
                    }
                    else if (controls[current].Type == Control::Type::MAP_RIGHT && !hold)
                    {
                        MapX++;
                    }
                }
            }
        }

        return false;
    }
}
#endif