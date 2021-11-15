#ifndef __INTERFACE__HPP__
#define __INTERFACE__HPP__

#include "BloodSword.hpp"

#include "graphics.hpp"

namespace Interface
{
    // (Player/Monster, Id, Awareness)
    typedef std::tuple<TacticalMap::Object, int, int> Combatants;

    bool ValidX(TacticalMap::Base &Map, int x)
    {
        return (x >= 0 && x < Map.SizeX);
    }

    bool ValidY(TacticalMap::Base &Map, int y)
    {
        return (y >= 0 && y < Map.SizeY);
    }

    bool Move(TacticalMap::Base &Map, int srcX, int srcY, int dstX, int dstY)
    {
        auto result = false;

        if (ValidX(Map, srcX) && ValidY(Map, srcY) && ValidX(Map, dstX) && ValidY(Map, dstY))
        {
            if (Map.Objects[srcY][srcX] == TacticalMap::Object::Player)
            {
                if (Map.Objects[dstY][dstX] == TacticalMap::Object::Passable)
                {
                    Map.Objects[dstY][dstX] = Map.Objects[srcY][srcX];
                    Map.ObjectID[dstY][dstX] = Map.ObjectID[srcY][srcX];

                    Map.Objects[srcY][srcX] = TacticalMap::Object::Passable;
                    Map.ObjectID[srcY][srcX] = 0;

                    result = true;
                }
            }
            else if (Map.Objects[srcY][srcX] == TacticalMap::Object::Monster)
            {
                auto HotCoals = false;

                for (auto i = 0; i < Map.HotCoals.size(); i++)
                {
                    if (Map.HotCoals[i].first == srcX && Map.HotCoals[i].second == srcY)
                    {
                        HotCoals = true;

                        break;
                    }
                }

                Map.Objects[dstY][dstX] = Map.Objects[srcY][srcX];
                Map.ObjectID[dstY][dstX] = Map.ObjectID[srcY][srcX];

                if (HotCoals)
                {
                    Map.Objects[srcX][srcY] = TacticalMap::Object::HotCoals;
                }
                else
                {
                    Map.Objects[srcX][srcY] = TacticalMap::Object::Passable;
                }

                Map.ObjectID[srcX][srcY] = 0;

                result = true;
            }
        }

        return result;
    }

    int Find(std::vector<Combatants> &Sequence, TacticalMap::Object object, int id)
    {
        auto found = -1;

        for (auto i = 0; i < Sequence.size(); i++)
        {
            if (std::get<0>(Sequence[i]) == TacticalMap::Object::Player && std::get<1>(Sequence[i]) == id)
            {
                found = i;

                break;
            }
        }

        return found;
    }

    void Find(TacticalMap::Base &Map, TacticalMap::Object object, int id, int &LocationX, int &LocationY)
    {
        LocationX = -1;
        LocationY = -1;

        bool found = false;

        for (auto y = 0; y < Map.SizeY; y++)
        {
            for (auto x = 0; x < Map.SizeX; x++)
            {
                if (Map.Objects[y][x] == object && Map.ObjectID[y][x] == id)
                {
                    LocationX = x;

                    LocationY = y;

                    found = true;

                    break;
                }
            }

            if (found)
            {
                break;
            }
        }
    }

    int Distance(int srcX, int srcY, int dstX, int dstY)
    {
        return std::abs(dstX - srcX) + std::abs(dstY - srcY);
    }

    bool Combat(SDL_Window *window, SDL_Renderer *renderer, std::vector<std::string> &map, Party::Base &party, std::vector<Monster::Base> &monsters)
    {
        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        auto DisplayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        auto TacticalMap = TacticalMap::Base(map, party, monsters);

        // offsets used to display tactical map
        auto MapX = 0;

        auto MapY = 0;

        auto ObjectSize = 64;

        auto PaddingX = 2 * startx + ObjectSize;

        auto PaddingY = 3 * starty + ObjectSize;

        // size of viewable grid
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

        // map at the center of the screen
        auto DrawX = (SCREEN_WIDTH - SizeX * ObjectSize) / 2;

        auto DrawY = (SCREEN_HEIGHT - SizeY * ObjectSize) / 2;

        // player input
        auto hold = false;
        auto selected = false;
        auto scroll_up = false;
        auto scroll_down = false;
        auto current = 0;

        auto CurrentPath = std::vector<AStar::Path>(party.Members.size());
        auto CurrentMove = std::vector<int>(party.Members.size());

        // round sequence
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

        auto CurrentMode = Combat::Mode::Normal;

        auto SelectedCombatant = -1;

        auto SelectX = -1;

        auto SelectY = -1;

        auto CurrentCombatant = 0;

        // blink
        auto BlinkCycle = true;

        auto BlinkStart = SDL_GetTicks();

        auto ResetSelection = [&]()
        {
            SelectX = -1;

            SelectY = -1;

            SelectedCombatant = -1;
        };

        auto CurrentX = -1;

        auto CurrentY = -1;

        auto ResetCurrent = [&]()
        {
            CurrentX = -1;

            CurrentY = -1;
        };

        auto CycleCombatants = [&]()
        {
            CurrentCombatant++;

            if (CurrentCombatant >= Sequence.size())
            {
                CurrentCombatant = 0;
            }

            ResetCurrent();
        };

        auto FontSize = 32;

        auto TextX = DrawX;

        auto TextY = DrawY - border_space - TTF_FontHeight(Fonts::Normal);

        auto TextWidth = TacticalMap.SizeX * ObjectSize;

        if (window && renderer && TacticalMap.SizeX > 0 && TacticalMap.SizeY > 0)
        {
            auto done = false;

            while (!done)
            {
                // check if out of bounds
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

                Graphics::FillWindow(renderer, intBK);

                auto Controls = std::vector<Button>();

                auto MapButtonsX = DrawX - (ObjectSize + 5 * border_space);
                auto MapButtonsY = DrawY + border_space;
                auto MapButtonsGridSize = (SizeY * ObjectSize - border_space) / 4;

                auto ActionsX = DrawX;
                auto ActionsY = DrawY + SizeY * ObjectSize + 4 * border_space;
                auto ActionsGrid = ObjectSize + 2 * border_space;

                auto StartMap = 12;
                auto BottomMapX = StartMap + (SizeX * (SizeY - 1));
                auto MidMapY = StartMap + (SizeY / 2 * SizeX) - SizeX;

                Controls.push_back(Button(0, "icons/up.png", 0, StartMap, 0, 1, MapButtonsX, MapButtonsY, MapY > 0 ? intLB : intGR, Control::Type::MAP_UP));
                Controls.push_back(Button(1, "icons/left.png", 1, MidMapY, 0, 2, MapButtonsX, MapButtonsY + MapButtonsGridSize + border_space, MapX > 0 ? intLB : intGR, Control::Type::MAP_LEFT));
                Controls.push_back(Button(2, "icons/right.png", 2, MidMapY + SizeX, 1, 3, MapButtonsX, MapButtonsY + 2 * (MapButtonsGridSize + border_space), (MapX < TacticalMap.SizeX - SizeX) ? intLB : intGR, Control::Type::MAP_RIGHT));
                Controls.push_back(Button(3, "icons/down.png", 3, BottomMapX, 2, 4, MapButtonsX, MapButtonsY + 3 * (MapButtonsGridSize + border_space), (MapY < TacticalMap.SizeY - SizeY) ? intLB : intGR, Control::Type::MAP_DOWN));
                Controls.push_back(Button(4, "icons/back-button-white.png", 10, 4, 10, 4, lastx, buttony, intLB, Control::Type::BACK));
                Controls.push_back(Button(5, "icons/move.png", 3, 6, BottomMapX, 5, ActionsX, ActionsY, intLB, Control::Type::MOVE));
                Controls.push_back(Button(6, "icons/heal.png", 5, 7, BottomMapX + 1, 6, ActionsX + ActionsGrid, ActionsY, intLB, Control::Type::HEAL));
                Controls.push_back(Button(7, "icons/swords.png", 6, 8, BottomMapX + 2, 7, ActionsX + 2 * ActionsGrid, ActionsY, intLB, Control::Type::ATTACK));
                Controls.push_back(Button(8, "icons/shoot.png", 7, 9, BottomMapX + 3, 8, ActionsX + 3 * ActionsGrid, ActionsY, intLB, Control::Type::SHOOT));
                Controls.push_back(Button(9, "icons/memorize.png", 8, 10, BottomMapX + 4, 9, ActionsX + 4 * ActionsGrid, ActionsY, intLB, Control::Type::MEMORIZE));
                Controls.push_back(Button(10, "icons/spell.png", 9, 11, BottomMapX + 5, 10, ActionsX + 5 * ActionsGrid, ActionsY, intLB, Control::Type::MAGIC));
                Controls.push_back(Button(11, "icons/exit-white.png", 10, 4, BottomMapX + 6, 4, ActionsX + 6 * ActionsGrid, ActionsY, intLB, Control::Type::FLEE));

                int NumControls = Controls.size();

                // select which object to blink (player/monster)
                auto BlinkX = -1;

                auto BlinkY = -1;

                // Render Objects within the Map Window
                for (auto y = MapY; y < MapY + SizeY; y++)
                {
                    auto AssetY = DrawY + (y - MapY) * ObjectSize;

                    auto CtrlY = (y - MapY);

                    for (auto x = MapX; x < MapX + SizeX; x++)
                    {
                        auto CtrlUp = NumControls;
                        auto CtrlDn = NumControls;
                        auto CtrlLt = NumControls;
                        auto CtrlRt = NumControls;

                        auto CtrlX = (x - MapX);

                        if (CtrlY > 0)
                        {
                            CtrlUp = NumControls - SizeX;
                        }

                        if (CtrlY < SizeY - 1)
                        {
                            CtrlDn = NumControls + SizeX;
                        }
                        else
                        {
                            if (CtrlX < 7)
                            {
                                CtrlDn = CtrlX + 5;
                            }
                            else
                            {
                                CtrlDn = 5;
                            }
                        }

                        if (CtrlX > 0)
                        {
                            CtrlLt = NumControls - 1;
                        }
                        else
                        {
                            if (CtrlY < SizeY / 2)
                            {
                                if (CtrlY == 0)
                                {
                                    CtrlLt = 0;
                                }
                                else
                                {
                                    CtrlLt = 1;
                                }
                            }
                            else
                            {
                                if (CtrlY == SizeY - 1)
                                {
                                    CtrlLt = 3;
                                }
                                else
                                {
                                    CtrlLt = 2;
                                }
                            }
                        }

                        if (CtrlX < SizeX - 1)
                        {
                            CtrlRt = NumControls + 1;
                        }

                        auto AssetX = DrawX + (x - MapX) * ObjectSize;

                        auto Object = TacticalMap.Objects[y][x];

                        auto ObjectId = TacticalMap.ObjectID[y][x] - 1;

                        if (Object == TacticalMap::Object::Wall)
                        {
                            Controls.push_back(Button(NumControls, Graphics::GetAsset(Graphics::AssetType::Wall), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::NONE));
                        }
                        else if (Object == TacticalMap::Object::Player)
                        {
                            if (std::get<1>(Sequence[CurrentCombatant]) == ObjectId)
                            {
                                BlinkX = AssetX;

                                BlinkY = AssetY;

                                if (SDL_GetTicks() - BlinkStart > 100)
                                {
                                    BlinkCycle = !BlinkCycle;

                                    BlinkStart = SDL_GetTicks();
                                }
                            }

                            if (party.Members[ObjectId].Class == Character::Class::Warrior)
                            {
                                Controls.push_back(Button(NumControls, Graphics::GetAsset(Graphics::AssetType::Warrior), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::PLAYER));
                            }
                        }
                        else if (Object == TacticalMap::Object::HotCoals)
                        {
                            Controls.push_back(Button(NumControls, Graphics::GetAsset(Graphics::AssetType::HotCoals), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::NONE));
                        }
                        else
                        {
                            Controls.push_back(Button(NumControls, Graphics::GetAsset(Graphics::AssetType::Passable), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::DESTINATION));
                        }

                        NumControls++;
                    }
                }

                if (SelectedCombatant >= 0 && SelectedCombatant < Sequence.size())
                {
                    // Render statistics for currently selected / highlighted player or monster
                }

                if (current >= 0 && current < Controls.size())
                {
                    Graphics::RenderCaption(renderer, Controls[current], clrWH, intBK);
                }

                Graphics::RenderButtons(renderer, Controls, current, border_space, border_pts);

                if (CurrentMode == Combat::Mode::Move)
                {
                    auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                    if (CurrentMove[PlayerId] > 0 && CurrentMove[PlayerId] < CurrentPath[PlayerId].Points.size())
                    {
                        Graphics::PutText(renderer, "Select new destination or continue along current path", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                        auto TargetX = CurrentPath[PlayerId].Points.back().X - MapX;

                        auto TargetY = CurrentPath[PlayerId].Points.back().Y - MapY;

                        if (TargetX >= 0 && TargetX < SizeX && TargetY >= 0 && TargetY < SizeY)
                        {
                            Graphics::ThickRect(renderer, ObjectSize - 4 * border_pts, ObjectSize - 4 * border_pts, DrawX + TargetX * ObjectSize + 2 * border_pts, DrawY + TargetY * ObjectSize + 2 * border_pts, intWH, border_pts);
                        }
                    }
                    else
                    {
                        Graphics::PutText(renderer, "Select Destination", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                }

                if (CurrentCombatant == SelectedCombatant)
                {
                    Graphics::ThickRect(renderer, ObjectSize, ObjectSize, BlinkX, BlinkY, intWH, border_pts);
                }
                else if (BlinkCycle && BlinkX >= 0 && BlinkY >= 0 && (current == -1 || (current >= 0 && current < Controls.size() && (BlinkX != Controls[current].X || BlinkY != Controls[current].Y))))
                {
                    Graphics::ThickRect(renderer, ObjectSize, ObjectSize, BlinkX, BlinkY, intRD, border_pts);
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        Graphics::PutTextBox(renderer, message.c_str(), Fonts::Normal, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                if (std::get<0>(Sequence[CurrentCombatant]) == TacticalMap::Object::Player)
                {
                    auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                    Input::GetInput(renderer, Controls, current, selected, scroll_up, scroll_down, hold, 50);

                    if ((selected && current >= 0 && current < Controls.size()) || scroll_up || scroll_down || hold)
                    {
                        if (Controls[current].Type == Control::Type::BACK && !hold)
                        {
                            current = -1;

                            done = true;
                        }
                        else if (Controls[current].Type == Control::Type::MAP_UP)
                        {
                            MapY--;
                        }
                        else if (Controls[current].Type == Control::Type::MAP_DOWN)
                        {
                            MapY++;
                        }
                        else if (Controls[current].Type == Control::Type::MAP_LEFT)
                        {
                            MapX--;
                        }
                        else if (Controls[current].Type == Control::Type::MAP_RIGHT)
                        {
                            MapX++;
                        }
                        else if (Controls[current].Type == Control::Type::PLAYER && !hold)
                        {
                            if (CurrentMode == Combat::Mode::Normal)
                            {
                                SelectX = MapX + (Controls[current].X - DrawX) / ObjectSize;

                                SelectY = MapY + (Controls[current].Y - DrawY) / ObjectSize;

                                if (ValidX(TacticalMap, SelectX) && ValidY(TacticalMap, SelectY))
                                {
                                    if (TacticalMap.Objects[SelectY][SelectX] == TacticalMap::Object::Player)
                                    {
                                        auto id = TacticalMap.ObjectID[SelectY][SelectX] - 1;

                                        SelectedCombatant = Find(Sequence, TacticalMap::Object::Player, id);
                                    }
                                    else
                                    {
                                        ResetSelection();
                                    }
                                }
                                else
                                {
                                    ResetSelection();
                                }
                            }
                        }
                        else if (Controls[current].Type == Control::Type::DESTINATION && !hold)
                        {
                            SelectX = MapX + (Controls[current].X - DrawX) / ObjectSize;

                            SelectY = MapY + (Controls[current].Y - DrawY) / ObjectSize;

                            if (CurrentMode == Combat::Mode::Normal)
                            {
                                ResetSelection();
                            }
                            else if (CurrentMode == Combat::Mode::Move)
                            {
                                Find(TacticalMap, TacticalMap::Object::Player, PlayerId + 1, CurrentX, CurrentY);

                                if (Distance(CurrentX, CurrentY, SelectX, SelectY) > 1)
                                {
                                    if (ValidX(TacticalMap, CurrentX) && ValidY(TacticalMap, CurrentY))
                                    {
                                        CurrentPath[PlayerId] = AStar::FindPath(TacticalMap, CurrentX, CurrentY, SelectX, SelectY);

                                        CurrentMove[PlayerId] = 1;

                                        if (CurrentPath[PlayerId].Points.size() > 2)
                                        {
                                            auto result = Move(TacticalMap, CurrentX, CurrentY, CurrentPath[PlayerId].Points[1].X, CurrentPath[PlayerId].Points[1].Y);

                                            if (!result)
                                            {
                                                DisplayMessage("Movement Blocked!", intRD);
                                            }
                                            else
                                            {
                                                CurrentMove[PlayerId] = 2;

                                                CurrentMode = Combat::Mode::Normal;

                                                CycleCombatants();
                                            }
                                        }
                                        else
                                        {
                                            DisplayMessage("Movement Blocked!", intRD);
                                        }
                                    }
                                }
                                else
                                {
                                    CurrentMove[PlayerId] = 0;

                                    CurrentPath[PlayerId].Points.clear();

                                    auto result = Move(TacticalMap, CurrentX, CurrentY, SelectX, SelectY);

                                    if (!result)
                                    {
                                        DisplayMessage("You cannot move there!", intRD);
                                    }
                                    else
                                    {
                                        CurrentMode = Combat::Mode::Normal;

                                        CycleCombatants();
                                    }
                                }
                            }
                        }
                        else if (Controls[current].Type == Control::Type::MOVE && !hold)
                        {
                            SelectX = MapX + (Controls[current].X - DrawX) / ObjectSize;

                            SelectY = MapY + (Controls[current].Y - DrawY) / ObjectSize;

                            if (CurrentMode == Combat::Mode::Normal)
                            {
                                CurrentMode = Combat::Mode::Move;
                            }
                            else if (CurrentMode == Combat::Mode::Move)
                            {
                                if (CurrentPath[PlayerId].Points.size() > 1 && CurrentMove[PlayerId] >= 0 && CurrentMove[PlayerId] < CurrentPath[PlayerId].Points.size())
                                {
                                    Find(TacticalMap, TacticalMap::Object::Player, PlayerId + 1, CurrentX, CurrentY);

                                    auto result = Move(TacticalMap, CurrentX, CurrentY, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].X, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].Y);

                                    if (!result)
                                    {
                                        DisplayMessage("Movement Blocked!", intRD);
                                    }
                                    else
                                    {
                                        CurrentMode = Combat::Mode::Normal;

                                        CurrentMove[PlayerId]++;

                                        CycleCombatants();
                                    }
                                }
                                else
                                {
                                    CurrentMode = Combat::Mode::Normal;
                                }
                            }
                        }
                        else if (Controls[current].Type == Control::Type::NONE && !hold)
                        {
                            if (CurrentMode == Combat::Mode::Normal)
                            {
                                ResetSelection();
                            }
                            else if (CurrentMode == Combat::Mode::Move)
                            {
                                DisplayMessage("You cannot move there!", intRD);
                            }
                        }
                    }

                    if (current >= 0 && current < Controls.size())
                    {
                        auto control_type = Controls[current].Type;

                        if (control_type == Control::Type::PLAYER || control_type == Control::Type::MONSTER || control_type == Control::Type::DESTINATION)
                        {
                            if (CurrentMode == Combat::Mode::Move && control_type == Control::Type::DESTINATION)
                            {
                                SelectX = MapX + (Controls[current].X - DrawX) / ObjectSize;

                                SelectY = MapY + (Controls[current].Y - DrawY) / ObjectSize;

                                // Selecting a location to move into
                            }
                            else if (CurrentMode == Combat::Mode::Heal && control_type == Control::Type::PLAYER)
                            {
                                // Selecting a location to move into
                            }
                            else if (CurrentMode == Combat::Mode::Attack && control_type == Control::Type::MONSTER)
                            {
                                // Selecting a target for an attack
                            }
                            else if (CurrentMode == Combat::Mode::Shoot && control_type == Control::Type::MONSTER)
                            {
                                // Selecting a target for a ranged attack
                            }
                            else if (CurrentMode == Combat::Mode::Magic && control_type == Control::Type::MONSTER)
                            {
                                // Selecting a target for a magic spell
                            }
                        }
                    }
                }
                else if (std::get<0>(Sequence[CurrentCombatant]) == TacticalMap::Object::Monster)
                {
                    // Do Monster Actions

                    CycleCombatants();
                }
            }
        }

        return false;
    }
}
#endif