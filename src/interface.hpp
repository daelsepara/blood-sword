#ifndef __INTERFACE__HPP__
#define __INTERFACE__HPP__

#include "BloodSword.hpp"

#include "graphics.hpp"

namespace Interface
{
    // (Player/Monster, Id, Awareness)
    typedef std::tuple<TacticalMap::Object, int, int> Combatants;

    bool Combat(SDL_Window *window, SDL_Renderer *renderer, std::vector<std::string> &map, Party::Base &party, std::vector<Monster::Base> &monsters)
    {
        auto TacticalMap = TacticalMap::Base(map, party, monsters);

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

        std::vector<Interface::Combatants> Sequence = {};

        // Sort combatants based on Awareness
        for (auto i = 0; i < party.Members.size(); i++)
        {
            auto awareness = Engine::Score(party.Members[i], Attributes::Type::Awareness);

            Sequence.push_back(std::make_tuple(TacticalMap::Object::Player, i, awareness));
        }

        for (auto i = 0; i < monsters.size(); i++)
        {
            auto awareness = monsters[i].Awareness;

            Sequence.push_back(std::make_tuple(TacticalMap::Object::Monster, i, awareness));
        }

        std::sort(Sequence.begin(), Sequence.end(), [](Combatants &a, Combatants &b) -> bool
                  { return std::get<2>(a) > std::get<2>(b); });

        auto current_mode = Combat::Mode::Normal;

        auto combatant_selected = -1;

        auto SelectX = 0;

        auto SelectY = 0;

        auto current_combatant = 0;

        auto BlinkCycle = true;
        auto BlinkStart = SDL_GetTicks();

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

                auto MapButtonsX = DrawX - (ObjectSize + 5 * border_space);
                auto MapButtonsY = DrawY + border_space;
                auto MapButtonsGridSize = (SizeY * ObjectSize - border_space) / 4;

                auto ActionsX = DrawX;
                auto ActionsY = DrawY + SizeY * ObjectSize + 4 * border_space;
                auto ActionsGrid = ObjectSize + 2 * border_space;

                auto StartMap = 12;
                auto BottomMapX = StartMap + (SizeX * (SizeY - 1));
                auto MidMapY = StartMap + (SizeY / 2 * SizeX) - SizeX;

                controls.push_back(Button(0, "icons/up.png", 0, StartMap, 0, 1, MapButtonsX, MapButtonsY, MapY > 0 ? intLB : intGR, Control::Type::MAP_UP));
                controls.push_back(Button(1, "icons/left.png", 1, MidMapY, 0, 2, MapButtonsX, MapButtonsY + MapButtonsGridSize + border_space, MapX > 0 ? intLB : intGR, Control::Type::MAP_LEFT));
                controls.push_back(Button(2, "icons/right.png", 2, MidMapY + SizeX, 1, 3, MapButtonsX, MapButtonsY + 2 * (MapButtonsGridSize + border_space), (MapX < TacticalMap.SizeX - SizeX) ? intLB : intGR, Control::Type::MAP_RIGHT));
                controls.push_back(Button(3, "icons/down.png", 3, BottomMapX, 2, 4, MapButtonsX, MapButtonsY + 3 * (MapButtonsGridSize + border_space), (MapY < TacticalMap.SizeY - SizeY) ? intLB : intGR, Control::Type::MAP_DOWN));
                controls.push_back(Button(4, "icons/back-button.png", 10, 4, 10, 4, lastx, buttony, intLB, Control::Type::BACK));
                controls.push_back(Button(5, "icons/move.png", 3, 6, BottomMapX, 5, ActionsX, ActionsY, intLB, Control::Type::MOVE));
                controls.push_back(Button(6, "icons/heal.png", 5, 7, BottomMapX + 1, 6, ActionsX + ActionsGrid, ActionsY, intLB, Control::Type::HEAL));
                controls.push_back(Button(7, "icons/swords.png", 6, 8, BottomMapX + 2, 7, ActionsX + 2 * ActionsGrid, ActionsY, intLB, Control::Type::ATTACK));
                controls.push_back(Button(8, "icons/shoot.png", 7, 9, BottomMapX + 3, 8, ActionsX + 3 * ActionsGrid, ActionsY, intLB, Control::Type::SHOOT));
                controls.push_back(Button(9, "icons/memorize.png", 8, 10, BottomMapX + 4, 9, ActionsX + 4 * ActionsGrid, ActionsY, intLB, Control::Type::MEMORIZE));
                controls.push_back(Button(10, "icons/spell.png", 9, 11, BottomMapX + 5, 10, ActionsX + 5 * ActionsGrid, ActionsY, intLB, Control::Type::MAGIC));
                controls.push_back(Button(11, "icons/exit.png", 10, 4, BottomMapX + 6, 4, ActionsX + 6 * ActionsGrid, ActionsY, intLB, Control::Type::FLEE));

                int num_controls = controls.size();

                auto BlinkX = -1;

                auto BlinkY = -1;

                // Render Objects within the Map Window
                for (auto y = MapY; y < MapY + SizeY; y++)
                {
                    auto AssetY = DrawY + (y - MapY) * ObjectSize;

                    auto ctrl_y = (y - MapY);

                    for (auto x = MapX; x < MapX + SizeX; x++)
                    {
                        auto ctrl_up = num_controls;
                        auto ctrl_dn = num_controls;
                        auto ctrl_lt = num_controls;
                        auto ctrl_rt = num_controls;

                        auto ctrl_x = (x - MapX);

                        if (ctrl_y > 0)
                        {
                            ctrl_up = num_controls - SizeX;
                        }

                        if (ctrl_y < SizeY - 1)
                        {
                            ctrl_dn = num_controls + SizeX;
                        }
                        else
                        {
                            if (ctrl_x < 7)
                            {
                                ctrl_dn = 5 + ctrl_x;
                            }
                        }

                        if (ctrl_x > 0)
                        {
                            ctrl_lt = num_controls - 1;
                        }
                        else
                        {
                            if (ctrl_y < SizeY / 2)
                            {
                                if (ctrl_y == 0)
                                {
                                    ctrl_lt = 0;
                                }
                                else
                                {
                                    ctrl_lt = 1;
                                }
                            }
                            else
                            {
                                if (ctrl_y == SizeY - 1)
                                {
                                    ctrl_lt = 3;
                                }
                                else
                                {
                                    ctrl_lt = 2;
                                }
                            }
                        }

                        if (ctrl_x < SizeX - 1)
                        {
                            ctrl_rt = num_controls + 1;
                        }

                        auto AssetX = DrawX + (x - MapX) * ObjectSize;

                        auto object = TacticalMap.Objects[y][x];

                        auto objectID = TacticalMap.ObjectID[y][x] - 1;

                        if (object == TacticalMap::Object::Wall)
                        {
                            controls.push_back(Button(num_controls, Graphics::GetAsset(Graphics::AssetType::Wall), ctrl_lt, ctrl_rt, ctrl_up, ctrl_dn, AssetX, AssetY, intYW, Control::Type::NONE));
                        }
                        else if (object == TacticalMap::Object::Player)
                        {
                            if (std::get<1>(Sequence[current_combatant]) == objectID)
                            {
                                BlinkX = AssetX;

                                BlinkY = AssetY;

                                if (SDL_GetTicks() - BlinkStart > 100)
                                {
                                    BlinkCycle = !BlinkCycle;

                                    BlinkStart = SDL_GetTicks();
                                }
                            }

                            if (party.Members[objectID].Class == Character::Class::Warrior)
                            {
                                controls.push_back(Button(num_controls, Graphics::GetAsset(Graphics::AssetType::Warrior), ctrl_lt, ctrl_rt, ctrl_up, ctrl_dn, AssetX, AssetY, intYW, Control::Type::PLAYER));
                            }
                        }
                        else if (object == TacticalMap::Object::HotCoals)
                        {
                            controls.push_back(Button(num_controls, Graphics::GetAsset(Graphics::AssetType::HotCoals), ctrl_lt, ctrl_rt, ctrl_up, ctrl_dn, AssetX, AssetY, intYW, Control::Type::NONE));
                        }
                        else
                        {
                            controls.push_back(Button(num_controls, Graphics::GetAsset(Graphics::AssetType::Passable), ctrl_lt, ctrl_rt, ctrl_up, ctrl_dn, AssetX, AssetY, intYW, Control::Type::DESTINATION));
                        }

                        num_controls++;
                    }
                }

                if (combatant_selected >= 0 && combatant_selected < Sequence.size())
                {
                    // Render statistics for currently selected / highlighted player or monster
                }

                Graphics::FillRect(renderer, SizeX * ObjectSize + 4 * border_space, SizeY * ObjectSize + 4 * border_space, DrawX - 2 * border_space, DrawY - 2 * border_space, intBK);

                if (current >= 0 && current < controls.size())
                {
                    Graphics::RenderCaption(renderer, controls[current]);
                }

                Graphics::RenderButtons(renderer, controls, current, border_space, border_pts);

                if (BlinkCycle && BlinkX >= 0 && BlinkY >= 0 && (current == -1 || (current >= 0 && current < controls.size() && (BlinkX != controls[current].X || BlinkY != controls[current].Y))))
                {
                    Graphics::ThickRect(renderer, ObjectSize, ObjectSize, BlinkX, BlinkY, intRD, border_pts);
                }

                if (std::get<0>(Sequence[current_combatant]) == TacticalMap::Object::Player)
                {
                    Input::GetInput(renderer, controls, current, selected, scroll_up, scroll_down, hold, 50);

                    if ((selected && current >= 0 && current < controls.size()) || scroll_up || scroll_down || hold)
                    {
                        if (controls[current].Type == Control::Type::BACK && !hold)
                        {
                            current = -1;

                            done = true;
                        }
                        else if (controls[current].Type == Control::Type::MAP_UP)
                        {
                            MapY--;
                        }
                        else if (controls[current].Type == Control::Type::MAP_DOWN)
                        {
                            MapY++;
                        }
                        else if (controls[current].Type == Control::Type::MAP_LEFT)
                        {
                            MapX--;
                        }
                        else if (controls[current].Type == Control::Type::MAP_RIGHT)
                        {
                            MapX++;
                        }
                        else if (controls[current].Type == Control::Type::PLAYER && !hold)
                        {
                            if (current_mode == Combat::Mode::Normal)
                            {
                            }
                        }
                        else if (controls[current].Type == Control::Type::DESTINATION && !hold)
                        {
                            if (current_mode == Combat::Mode::Normal)
                            {
                            }
                        }
                    }

                    if (current >= 0 && current < controls.size())
                    {
                        auto control_type = controls[current].Type;

                        if (control_type == Control::Type::PLAYER || control_type == Control::Type::MONSTER || control_type == Control::Type::DESTINATION)
                        {
                            SelectX = MapX + (controls[current].X - DrawX) / ObjectSize;

                            SelectY = MapY + (controls[current].Y - DrawY) / ObjectSize;

                            if (current_mode == Combat::Mode::Move && control_type == Control::Type::DESTINATION)
                            {
                                // Selecting a location to move into
                            }
                            else if (current_mode == Combat::Mode::Heal && control_type == Control::Type::PLAYER)
                            {
                                // Selecting a location to move into
                            }
                            else if (current_mode == Combat::Mode::Attack && control_type == Control::Type::MONSTER)
                            {
                                // Selecting a target for an attack
                            }
                            else if (current_mode == Combat::Mode::Shoot && control_type == Control::Type::MONSTER)
                            {
                                // Selecting a target for a ranged attack
                            }
                            else if (current_mode == Combat::Mode::Magic && control_type == Control::Type::MONSTER)
                            {
                                // Selecting a target for a magic spell
                            }
                        }
                    }
                }
                else if (std::get<0>(Sequence[current_combatant]) == TacticalMap::Object::Monster)
                {
                    // Do Monster Actions
                    current_combatant++;

                    if (current_combatant >= Sequence.size())
                    {
                        current_combatant = 0;
                    }
                }
            }
        }

        return false;
    }
}
#endif