#ifndef __INTERFACE__HPP__
#define __INTERFACE__HPP__

#include "BloodSword.hpp"

#include "graphics.hpp"

namespace Interface
{
    // (Player/Monster, Id, Awareness)
    typedef std::tuple<TacticalMap::Object, int, int> Combatants;
    typedef std::tuple<int, int, int> Targets;

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

        if (Interface::ValidX(Map, srcX) && Interface::ValidY(Map, srcY) && Interface::ValidX(Map, dstX) && Interface::ValidY(Map, dstY))
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
                    Map.Objects[srcY][srcX] = TacticalMap::Object::HotCoals;
                }
                else
                {
                    Map.Objects[srcY][srcX] = TacticalMap::Object::Passable;
                }

                Map.ObjectID[srcY][srcX] = 0;

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
            if (std::get<0>(Sequence[i]) == object && std::get<1>(Sequence[i]) == id)
            {
                found = i;

                break;
            }
        }

        return found;
    }

    void Find(TacticalMap::Base &Map, TacticalMap::Object object, int id, int &LocationX, int &LocationY)
    {
        bool found = false;

        LocationX = -1;

        LocationY = -1;

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

    bool IsAdjacent(TacticalMap::Base &Map, int PlayerId, int MonsterId)
    {
        auto PlayerX = -1;

        auto PlayerY = -1;

        auto MonsterX = -1;

        auto MonsterY = -1;

        Interface::Find(Map, TacticalMap::Object::Player, PlayerId + 1, PlayerX, PlayerY);

        Interface::Find(Map, TacticalMap::Object::Monster, MonsterId + 1, MonsterX, MonsterY);

        auto IsValidX = Interface::ValidX(Map, PlayerX) && Interface::ValidX(Map, MonsterX);

        auto IsValidY = Interface::ValidY(Map, PlayerY) && Interface::ValidX(Map, MonsterY);

        return IsValidX && IsValidY && Interface::Distance(PlayerX, PlayerY, MonsterX, MonsterY) <= 1;
    }

    bool AttackedUponMoving(TacticalMap::Base &Map, std::vector<Monster::Base> &monsters, Character::Base &character, int PlayerId)
    {
        auto WasAttacked = false;

        for (auto i = 0; i < monsters.size(); i++)
        {
            Monster::Base &monster = monsters[i];

            if (monster.Endurance > 0 && Interface::IsAdjacent(Map, PlayerId, i) && monster.Awareness >= Engine::Score(character, Attributes::Type::Awareness))
            {
                WasAttacked = true;

                // do damages
            }
        }

        return WasAttacked;
    }

    void DrawPath(SDL_Renderer *Renderer, AStar::Path &CurrentPath, int CurrentMove, int SizeX, int SizeY, int MapX, int MapY, int DrawX, int DrawY, int ObjectSize, Uint32 c, Uint8 a)
    {
        if (CurrentMove > 0 && CurrentMove < CurrentPath.Points.size())
        {
            auto TargetX = CurrentPath.Points.back().X - MapX;

            auto TargetY = CurrentPath.Points.back().Y - MapY;

            if (TargetX >= 0 && TargetX < SizeX && TargetY >= 0 && TargetY < SizeY)
            {
                SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);

                Uint32 Color = O(c, a);

                for (auto i = CurrentMove; i < CurrentPath.Points.size() - 1; i++)
                {
                    auto X = CurrentPath.Points[i].X - MapX;

                    auto Y = CurrentPath.Points[i].Y - MapY;

                    if (X >= 0 && X < SizeX && Y >= 0 && Y < SizeY)
                    {
                        Graphics::FillRect(Renderer, ObjectSize, ObjectSize, DrawX + X * ObjectSize, DrawY + Y * ObjectSize, Color);
                    }
                }

                SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_NONE);

                Graphics::ThickRect(Renderer, ObjectSize - 4 * border_pts, ObjectSize - 4 * border_pts, DrawX + TargetX * ObjectSize + 2 * border_pts, DrawY + TargetY * ObjectSize + 2 * border_pts, c, border_pts);
            }
        }
    }

    void SortTargets(std::vector<Targets> &Distances)
    {
        // sort players based on distance
        std::sort(Distances.begin(), Distances.end(), [](Targets &a, Targets &b) -> bool
                  {
                      if (std::get<1>(a) < std::get<1>(b))
                      {
                          return true;
                      }
                      else if (std::get<1>(a) == std::get<1>(b))
                      {
                          // on equidistant targets, attack target with lesser endurance
                          if (std::get<2>(a) < std::get<2>(b))
                          {
                              return true;
                          }
                          else
                          {
                              return false;
                          }
                      }
                      else
                      {
                          return false;
                      }
                  });
    }

    Interface::Targets SelectTarget(TacticalMap::Base &Map, Party::Base &party, std::vector<Monster::Base> &monsters, std::vector<Combatants> &Sequence, int CurrentCombatant)
    {
        Interface::Targets NearestPlayer = {};

        // PlayerId, Distance, Endurance
        std::vector<Targets> Distances = {};

        auto MonsterX = 0;

        auto MonsterY = 0;

        auto MonsterId = std::get<1>(Sequence[CurrentCombatant]);

        Interface::Find(Map, TacticalMap::Object::Monster, MonsterId + 1, MonsterX, MonsterY);

        // Do Monster Actions
        for (auto i = 0; i < party.Members.size(); i++)
        {
            auto Endurance = Engine::Score(party.Members[i], Attributes::Type::Endurance);

            if (Endurance > 0 && !party.Members[i].Escaped)
            {
                auto LocationX = 0;

                auto LocationY = 0;

                Interface::Find(Map, TacticalMap::Object::Player, i + 1, LocationX, LocationY);

                Distances.push_back(std::make_tuple(i, Interface::Distance(MonsterX, MonsterY, LocationX, LocationY), Endurance));
            }
        }

        if (Distances.size() > 0)
        {
            // sort players based on distance and endurance
            SortTargets(Distances);

            NearestPlayer = Distances.front();
        }

        return NearestPlayer;
    }

    void RenderCombatScreen(SDL_Renderer *Renderer, std::vector<Button> &Controls, int Current, Uint32 bg)
    {
        Graphics::FillWindow(Renderer, bg);

        Graphics::RenderButtons(Renderer, Controls, Current, border_space, border_pts);
    }

    void CharacterSheet(SDL_Renderer *Renderer, Party::Base &party, TTF_Font *Font, int PlayerId, int TextW, int TextX, int TextY)
    {
        auto FontSize = TTF_FontHeight(Font);

        auto FightingProwess = Engine::Score(party.Members[PlayerId], Attributes::Type::FightingProwess);
        auto PsychicAbility = Engine::Score(party.Members[PlayerId], Attributes::Type::PsychicAbility);
        auto Awareness = Engine::Score(party.Members[PlayerId], Attributes::Type::Awareness);
        auto Endurance = Engine::Score(party.Members[PlayerId], Attributes::Type::Endurance);
        auto Armour = Engine::Armour(party.Members[PlayerId]);

        Graphics::PutText(Renderer, Character::Description[party.Members[PlayerId].Class], Font, 0, clrLB, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY);
        Graphics::PutText(Renderer, std::string("RANK: " + std::to_string(party.Members[PlayerId].Rank)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + (FontSize + 2));
        Graphics::PutText(Renderer, std::string("FIGHTING PROWESS: " + std::to_string(FightingProwess)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 2 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("PSYCHIC ABILITY: " + std::to_string(PsychicAbility)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 3 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("AWARENESS: " + std::to_string(Awareness)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 4 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ENDURANCE: " + std::to_string(Endurance)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 5 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ARMOUR RATING: " + std::to_string(Armour)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 6 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("DAMAGE: " + std::to_string(party.Members[PlayerId].Damage) + "D+" + std::to_string(party.Members[PlayerId].DamageModifier)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 7 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("EXPERIENCE: " + std::to_string(party.Members[PlayerId].ExperiencePoints)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 8 * (FontSize + 2));

        if (party.Members[PlayerId].IsDefending)
        {
            Graphics::PutText(Renderer, "DEFENDING", Font, 0, clrLG, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 9 * (FontSize + 2));
        }
    }

    void MonsterData(SDL_Renderer *Renderer, std::vector<Monster::Base> &monsters, TTF_Font *Font, int MonsterId, int TextW, int TextX, int TextY)
    {
        auto FontSize = TTF_FontHeight(Font);

        Graphics::PutText(Renderer, monsters[MonsterId].Name.c_str(), Font, 0, clrRD, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY);
        Graphics::PutText(Renderer, std::string("FIGHTING PROWESS: " + std::to_string(monsters[MonsterId].FightingProwess)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + (FontSize + 2));
        Graphics::PutText(Renderer, std::string("PSYCHIC ABILITY: " + std::to_string(monsters[MonsterId].PsychicAbility)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 2 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("AWARENESS: " + std::to_string(monsters[MonsterId].Awareness)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + +3 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ENDURANCE: " + std::to_string(monsters[MonsterId].Endurance)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + +4 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ARMOUR RATING: " + std::to_string(monsters[MonsterId].Armour)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + +5 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("DAMAGE: " + std::to_string(monsters[MonsterId].Damage) + "D+" + std::to_string(monsters[MonsterId].DamageModifier)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 6 * (FontSize + 2));
    }

    void ShowCoordinates(SDL_Renderer *Renderer, TacticalMap::Base &Map, std::vector<Button> &Controls, std::vector<Combatants> &Sequence, int Current, int SelectedCombatant, TTF_Font *Font, int MapX, int MapY, int DrawX, int DrawY, int ObjectSize, int TextW, int TextX)
    {
        auto ControlType = Controls[Current].Type;

        auto FontSize = TTF_FontHeight(Font);

        if ((SelectedCombatant < 0 || SelectedCombatant >= Sequence.size()) && (ControlType == Control::Type::MAP_NONE || ControlType == Control::Type::DESTINATION))
        {
            auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

            auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

            std::string Coordinates = "(" + std::to_string(SelectX) + ", " + std::to_string(SelectY) + ")";

            Graphics::PutText(Renderer, Coordinates.c_str(), Font, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, DrawY);

            Graphics::PutText(Renderer, TacticalMap::Description[Map.Objects[SelectY][SelectX]], Font, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, DrawY + (FontSize + text_space));
        }
    }

    void SortCombatants(std::vector<Combatants> &Sequence)
    {
        std::sort(Sequence.begin(), Sequence.end(), [](Combatants &a, Combatants &b) -> bool
                  {
                      // sort combatants based on Awareness
                      if (std::get<2>(a) > std::get<2>(b))
                      {
                          return true;
                      }
                      else if (std::get<2>(a) == std::get<2>(b))
                      {
                          // give priority to player character
                          if (std::get<0>(a) == TacticalMap::Object::Player && std::get<0>(b) == TacticalMap::Object::Monster)
                          {
                              return true;
                          }
                          else
                          {
                              return false;
                          }
                      }
                      else
                      {
                          return false;
                      }
                  });
    }

    bool Combat(SDL_Window *window, SDL_Renderer *Renderer, std::vector<std::string> &map, Party::Base &party, std::vector<Monster::Base> &monsters)
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

        auto Map = TacticalMap::Base(map, party, monsters);

        // offsets used to display tactical map
        auto MapX = 0;

        auto MapY = 0;

        auto ObjectSize = 64;

        auto PaddingX = 2 * startx + 2 * ObjectSize;

        auto PaddingY = starty + ObjectSize;

        // size of viewable grid
        auto SizeX = (SCREEN_WIDTH - 2 * PaddingX) / ObjectSize;

        auto SizeY = (SCREEN_HEIGHT - 2 * PaddingY) / ObjectSize;

        if (SizeX > Map.SizeX)
        {
            SizeX = Map.SizeX;
        }

        if (SizeY > Map.SizeY)
        {
            SizeY = Map.SizeY;
        }

        // map at the center of the screen
        auto DrawX = PaddingX / 2;

        auto DrawY = ObjectSize;

        // player input
        auto hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        auto CurrentPath = std::vector<AStar::Path>(party.Members.size());

        auto CurrentMove = std::vector<int>(party.Members.size());

        // round sequence
        std::vector<Interface::Combatants> Sequence = {};

        // Sort combatants based on Awareness
        for (auto i = 0; i < party.Members.size(); i++)
        {
            auto Awareness = Engine::Score(party.Members[i], Attributes::Type::Awareness);

            Sequence.push_back(std::make_tuple(TacticalMap::Object::Player, i, Awareness));
        }

        for (auto i = 0; i < monsters.size(); i++)
        {
            auto Awareness = monsters[i].Awareness;

            Sequence.push_back(std::make_tuple(TacticalMap::Object::Monster, i, Awareness));
        }

        SortCombatants(Sequence);

        auto CurrentMode = Combat::Mode::Normal;

        auto SelectedCombatant = -1;

        auto CurrentCombatant = 0;

        // blink
        auto Blink = true;

        auto BlinkStart = SDL_GetTicks();

        auto ResetSelection = [&]()
        {
            SelectedCombatant = -1;
        };

        auto CombatRound = 0;

        auto CycleCombatants = [&]()
        {
            auto active = false;

            while (!active)
            {
                if (!Engine::IsAlive(party) || !Engine::IsAlive(monsters) || Engine::Escaped(party))
                {
                    break;
                }

                CurrentCombatant++;

                if (CurrentCombatant >= Sequence.size())
                {
                    CurrentCombatant = 0;

                    CombatRound++;
                }

                auto obj = std::get<0>(Sequence[CurrentCombatant]);
                auto id = std::get<1>(Sequence[CurrentCombatant]);

                if (obj == TacticalMap::Object::Player)
                {
                    auto character = party.Members[id];

                    active = Engine::IsAlive(character) && !character.Escaped;
                }
                else if (obj == TacticalMap::Object::Monster)
                {
                    active = monsters[id].Endurance > 0;
                }
            }

            CurrentMode = Combat::Mode::Normal;

            ResetSelection();
        };

        auto FontSize = TTF_FontHeight(Fonts::Normal);

        auto TextX = DrawX;

        auto TextR = DrawX + (ObjectSize * SizeX + border_space);

        auto TextWidthR = SCREEN_WIDTH - TextR;

        auto TextY = DrawY - 2 * border_space - FontSize;

        auto TextWidth = Map.SizeX * ObjectSize;

        if (window && Renderer && Map.SizeX > 0 && Map.SizeY > 0)
        {
            auto done = false;

            while (!done)
            {
                // check if out of bounds
                if (MapX < 0)
                {
                    MapX = 0;
                }

                if (MapX > Map.SizeX - SizeX)
                {
                    MapX = Map.SizeX - SizeX;
                }

                if (MapY < 0)
                {
                    MapY = 0;
                }

                if (MapY > Map.SizeY - SizeY)
                {
                    MapY = Map.SizeY - SizeY;
                }

                auto Controls = std::vector<Button>();

                auto MapButtonSize = ObjectSize + 2 * border_space;
                auto MapSizeX = SizeX * ObjectSize;
                auto MapSizeY = SizeY * ObjectSize;
                auto MapButtonsX = DrawX - MapButtonSize;
                auto MapButtonsY = DrawY + border_space;
                auto MapButtonsGridSize = MapSizeY / 4;

                auto ActionsX = DrawX;
                auto ActionsY = DrawY + MapSizeY + 2 * border_space;
                auto ActionsGrid = MapButtonSize;

                auto StartMap = 14;
                auto BottomMapX = StartMap + (SizeX * (SizeY - 1));
                auto MidMapY = StartMap + (SizeY / 2 * SizeX) - SizeX;

                Controls.push_back(Button(0, Graphics::GetAsset(Graphics::AssetType::Up), 0, StartMap, 0, 1, MapButtonsX, MapButtonsY, MapY > 0 ? intLB : intGR, Control::Type::MAP_UP));
                Controls.push_back(Button(1, Graphics::GetAsset(Graphics::AssetType::Left), 1, MidMapY, 0, 2, MapButtonsX, MapButtonsY + (MapButtonsGridSize + 2 * border_space), MapX > 0 ? intLB : intGR, Control::Type::MAP_LEFT));
                Controls.push_back(Button(2, Graphics::GetAsset(Graphics::AssetType::Right), 2, MidMapY + SizeX, 1, 3, MapButtonsX, MapButtonsY + 2 * (MapButtonsGridSize + 2 * border_space), (MapX < Map.SizeX - SizeX) ? intLB : intGR, Control::Type::MAP_RIGHT));
                Controls.push_back(Button(3, Graphics::GetAsset(Graphics::AssetType::Down), 3, BottomMapX, 2, 5, MapButtonsX, MapButtonsY + 3 * (MapButtonsGridSize + 2 * border_space), (MapY < Map.SizeY - SizeY) ? intLB : intGR, Control::Type::MAP_DOWN));
                Controls.push_back(Button(4, Graphics::GetAsset(Graphics::AssetType::Back), StartMap - 1, 4, StartMap - 1, 4, lastx, buttony, intLB, Control::Type::EXIT));
                Controls.push_back(Button(5, Graphics::GetAsset(Graphics::AssetType::Items), 3, 6, BottomMapX, 5, ActionsX, ActionsY, intLB, Control::Type::ITEMS));
                Controls.push_back(Button(6, Graphics::GetAsset(Graphics::AssetType::Move), 5, 7, BottomMapX + 1, 6, ActionsX + ActionsGrid, ActionsY, intLB, Control::Type::MOVE));
                Controls.push_back(Button(7, Graphics::GetAsset(Graphics::AssetType::Heal), 6, 8, BottomMapX + 2, 7, ActionsX + 2 * ActionsGrid, ActionsY, intLB, Control::Type::HEAL));
                Controls.push_back(Button(8, Graphics::GetAsset(Graphics::AssetType::Attack), 7, 9, BottomMapX + 3, 8, ActionsX + 3 * ActionsGrid, ActionsY, intLB, Control::Type::ATTACK));
                Controls.push_back(Button(9, Graphics::GetAsset(Graphics::AssetType::Defend), 8, 10, BottomMapX + 4, 9, ActionsX + 4 * ActionsGrid, ActionsY, intLB, Control::Type::DEFEND));
                Controls.push_back(Button(10, Graphics::GetAsset(Graphics::AssetType::Shoot), 9, 11, BottomMapX + 5, 10, ActionsX + 5 * ActionsGrid, ActionsY, intLB, Control::Type::SHOOT));
                Controls.push_back(Button(11, Graphics::GetAsset(Graphics::AssetType::Memorize), 10, 12, BottomMapX + 6, 11, ActionsX + 6 * ActionsGrid, ActionsY, intLB, Control::Type::MEMORIZE));
                Controls.push_back(Button(12, Graphics::GetAsset(Graphics::AssetType::Magic), 11, 13, BottomMapX + 7, 12, ActionsX + 7 * ActionsGrid, ActionsY, intLB, Control::Type::MAGIC));
                Controls.push_back(Button(13, Graphics::GetAsset(Graphics::AssetType::Flee), 12, 4, BottomMapX + 8, 4, ActionsX + 8 * ActionsGrid, ActionsY, intLB, Control::Type::FLEE));

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
                            if (CtrlX < 9)
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

                        auto Object = Map.Objects[y][x];

                        auto ObjectId = Map.ObjectID[y][x] - 1;

                        if (Object == TacticalMap::Object::Wall)
                        {
                            Controls.push_back(Button(NumControls, Graphics::GetAsset(Graphics::AssetType::Wall), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::MAP_NONE));
                        }
                        else if (Object == TacticalMap::Object::Player)
                        {
                            if (std::get<1>(Sequence[CurrentCombatant]) == ObjectId)
                            {
                                BlinkX = AssetX;

                                BlinkY = AssetY;

                                if (SDL_GetTicks() - BlinkStart > 100)
                                {
                                    Blink = !Blink;

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
                            Controls.push_back(Button(NumControls, Graphics::GetAsset(Graphics::AssetType::HotCoals), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::MAP_NONE));
                        }
                        else if (Object == TacticalMap::Object::Monster)
                        {
                            if (monsters[ObjectId].Type == Monster::Type::Barbarian)
                            {
                                Controls.push_back(Button(NumControls, Graphics::GetAsset(Graphics::AssetType::Barbarian), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::MONSTER));
                            }
                        }
                        else
                        {
                            Controls.push_back(Button(NumControls, Graphics::GetAsset(Graphics::AssetType::Passable), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::DESTINATION));
                        }

                        NumControls++;
                    }
                }

                Interface::RenderCombatScreen(Renderer, Controls, Current, intBK);

                if (Current >= 0 && Current < Controls.size())
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                }

                // Blink current party member
                if (SelectedCombatant != CurrentCombatant && Blink && BlinkX >= 0 && BlinkY >= 0 && (Current == -1 || (Current >= 0 && Current < Controls.size() && (BlinkX != Controls[Current].X || BlinkY != Controls[Current].Y))))
                {
                    Graphics::ThickRect(Renderer, ObjectSize - 4 * border_pts, ObjectSize - 4 * border_pts, BlinkX + 2 * border_pts, BlinkY + 2 * border_pts, intRD, border_pts);
                }

                if (SelectedCombatant >= 0 && SelectedCombatant < Sequence.size())
                {
                    auto Combatant = Sequence[SelectedCombatant];

                    auto SelectedX = 0;

                    auto SelectedY = 0;

                    auto Id = std::get<1>(Combatant);

                    Interface::Find(Map, std::get<0>(Combatant), Id + 1, SelectedX, SelectedY);

                    if ((SelectedX - MapX) >= 0 && (SelectedX - MapX) < SizeX && (SelectedY - MapY) >= 0 && (SelectedY - MapY) < SizeY)
                    {
                        Graphics::ThickRect(Renderer, ObjectSize - 4 * border_pts, ObjectSize - 4 * border_pts, DrawX + (SelectedX - MapX) * ObjectSize + 2 * border_pts, DrawY + (SelectedY - MapY) * ObjectSize + 2 * border_pts, intYW, border_pts);
                    }

                    // Render statistics for currently selected / highlighted player or monster
                    if (std::get<0>(Combatant) == TacticalMap::Object::Player)
                    {
                        Interface::CharacterSheet(Renderer, party, Fonts::Fixed, Id, TextWidthR, TextR, DrawY);
                    }
                    else if (std::get<0>(Combatant) == TacticalMap::Object::Monster)
                    {
                        Interface::MonsterData(Renderer, monsters, Fonts::Fixed, Id, TextWidthR, TextR, DrawY);

                        // Show potential target
                        auto NearestPlayer = Interface::SelectTarget(Map, party, monsters, Sequence, SelectedCombatant);

                        auto PlayerId = std::get<0>(NearestPlayer);

                        if (PlayerId >= 0 && PlayerId < party.Members.size())
                        {
                            auto MonsterX = -1;

                            auto MonsterY = -1;

                            auto LocationX = -1;

                            auto LocationY = -1;

                            Interface::Find(Map, TacticalMap::Object::Monster, Id + 1, MonsterX, MonsterY);

                            Interface::Find(Map, TacticalMap::Object::Player, PlayerId + 1, LocationX, LocationY);

                            if (Interface::ValidX(Map, MonsterX) && Interface::ValidY(Map, MonsterY) && Interface::ValidX(Map, LocationX) && Interface::ValidY(Map, LocationY))
                            {
                                auto MonsterPath = AStar::FindPath(Map, MonsterX, MonsterY, LocationX, LocationY);

                                if (MonsterPath.Points.size() > 2)
                                {
                                    Interface::DrawPath(Renderer, MonsterPath, 1, SizeX, SizeY, MapX, MapY, DrawX, DrawY, ObjectSize, intRD, 0x66);
                                }
                            }
                        }
                    }
                }

                if (Current >= 0 && Current < Controls.size())
                {
                    auto ControlType = Controls[Current].Type;

                    Interface::ShowCoordinates(Renderer, Map, Controls, Sequence, Current, SelectedCombatant, Fonts::Normal, MapX, MapY, DrawX, DrawY, ObjectSize, TextWidth, TextR);

                    if (CurrentMode == Combat::Mode::Normal && ControlType == Control::Type::PLAYER)
                    {
                        Graphics::PutText(Renderer, "View party member", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                        auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                        auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

                        Interface::CharacterSheet(Renderer, party, Fonts::Fixed, Map.ObjectID[SelectY][SelectX] - 1, TextWidthR, TextR, DrawY);
                    }
                    else if (CurrentMode == Combat::Mode::Normal && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "View opponent", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                        auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                        auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

                        Interface::MonsterData(Renderer, monsters, Fonts::Fixed, Map.ObjectID[SelectY][SelectX] - 1, TextWidthR, TextR, DrawY);
                    }
                    else if (CurrentMode == Combat::Mode::Normal)
                    {
                        Graphics::PutText(Renderer, (std::string("Combat Round: " + std::to_string(CombatRound + 1))).c_str(), Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::Move)
                    {
                        auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                        auto TargetX = -1;

                        auto TargetY = -1;

                        if (CurrentMove[PlayerId] > 0 && CurrentMove[PlayerId] < CurrentPath[PlayerId].Points.size())
                        {
                            TargetX = CurrentPath[PlayerId].Points.back().X - MapX;

                            TargetY = CurrentPath[PlayerId].Points.back().Y - MapY;

                            Graphics::PutText(Renderer, "Move to location or continue along current path", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                            Interface::DrawPath(Renderer, CurrentPath[PlayerId], CurrentMove[PlayerId], SizeX, SizeY, MapX, MapY, DrawX, DrawY, ObjectSize, intYW, 0x66);
                        }
                        else
                        {
                            Graphics::PutText(Renderer, "Move to location", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                        }

                        if (ControlType == Control::Type::DESTINATION)
                        {
                            auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                            auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

                            if ((TargetX != SelectX || TargetY != SelectY) && ControlType == Control::Type::DESTINATION)
                            {
                                auto PlayerX = -1;

                                auto PlayerY = -1;

                                Interface::Find(Map, TacticalMap::Object::Player, PlayerId + 1, PlayerX, PlayerY);

                                if (Interface::ValidX(Map, PlayerX) && Interface::ValidY(Map, PlayerY) && Interface::Distance(PlayerX, PlayerY, SelectX, SelectY) > 1)
                                {
                                    auto TempPath = AStar::FindPath(Map, PlayerX, PlayerY, SelectX, SelectY);

                                    if (TempPath.Points.size() > 2)
                                    {
                                        Interface::DrawPath(Renderer, TempPath, 1, SizeX, SizeY, MapX, MapY, DrawX, DrawY, ObjectSize, intMG, 0x66);
                                    }
                                }
                            }
                        }
                    }
                    else if (CurrentMode == Combat::Mode::Heal && ControlType == Control::Type::PLAYER)
                    {
                        Graphics::PutText(Renderer, "Heal target", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::Attack && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "Fight target", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::Attack)
                    {
                        Graphics::PutText(Renderer, "Select a nearby opponent to fight", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::Shoot && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "Shoot at target", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::Magic && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "Cast a spell", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if ((ControlType == Control::Type::MAP_NONE || ControlType == Control::Type::DESTINATION) && (SelectedCombatant < 0 || SelectedCombatant >= Sequence.size()))
                    {
                        auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                        auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

                        std::string Coordinates = "(" + std::to_string(SelectX) + ", " + std::to_string(SelectY) + ")";

                        Graphics::PutText(Renderer, Coordinates.c_str(), Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextR, DrawY);

                        Graphics::PutText(Renderer, TacticalMap::Description[Map.Objects[SelectY][SelectX]], Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextR, DrawY + (FontSize + text_space));
                    }
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        auto FlashW = 3 * MapSizeX / 5;

                        auto FlashH = 2 * infoh;

                        Graphics::PutTextBox(Renderer, message.c_str(), Fonts::Normal, -1, clrWH, flash_color, TTF_STYLE_NORMAL, FlashW, infoh * 2, DrawX + (MapSizeX - FlashW) / 2, DrawY + (MapSizeY - FlashH) / 2);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                // get player input
                if (std::get<0>(Sequence[CurrentCombatant]) == TacticalMap::Object::Player)
                {
                    Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, hold, 50);

                    auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                    if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || hold)
                    {
                        if (Controls[Current].Type == Control::Type::EXIT && !hold)
                        {
                            Current = -1;

                            done = true;
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_UP || ScrollUp)
                        {
                            if (MapY > 0)
                            {
                                MapY--;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_DOWN || ScrollDown)
                        {
                            if (MapY < Map.SizeY - SizeY)
                            {
                                MapY++;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_LEFT)
                        {
                            MapX--;
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_RIGHT)
                        {
                            MapX++;
                        }
                        else if (Controls[Current].Type == Control::Type::PLAYER && !hold)
                        {
                            if (CurrentMode == Combat::Mode::Normal)
                            {
                                auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                                auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

                                if (Interface::ValidX(Map, SelectX) && Interface::ValidY(Map, SelectY))
                                {
                                    if (Map.Objects[SelectY][SelectX] == TacticalMap::Object::Player)
                                    {
                                        auto result = Interface::Find(Sequence, TacticalMap::Object::Player, Map.ObjectID[SelectY][SelectX] - 1);

                                        if (result != SelectedCombatant)
                                        {
                                            SelectedCombatant = result;
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
                                else
                                {
                                    ResetSelection();
                                }
                            }
                            else if (CurrentMode == Combat::Mode::Attack)
                            {
                                DisplayMessage("You cannot fight that!", intRD);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::DESTINATION && !hold)
                        {
                            auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                            auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

                            if (CurrentMode == Combat::Mode::Normal)
                            {
                                ResetSelection();
                            }
                            else if (CurrentMode == Combat::Mode::Attack)
                            {
                                CurrentMode = Combat::Mode::Normal;
                            }
                            else if (CurrentMode == Combat::Mode::Move)
                            {
                                Character::Base &character = party.Members[PlayerId];

                                // Get attacked by a nearby enemy that has a higher awareness
                                auto WasAttacked = AttackedUponMoving(Map, monsters, character, PlayerId);

                                if (WasAttacked)
                                {
                                    DisplayMessage(("The " + std::string(Character::Description[character.Class]) + " was attacked!").c_str(), intRD);
                                }

                                if (Engine::IsAlive(character))
                                {
                                    auto CurrentX = -1;

                                    auto CurrentY = -1;

                                    Interface::Find(Map, TacticalMap::Object::Player, PlayerId + 1, CurrentX, CurrentY);

                                    if (Interface::Distance(CurrentX, CurrentY, SelectX, SelectY) > 1)
                                    {
                                        if (Interface::ValidX(Map, CurrentX) && Interface::ValidY(Map, CurrentY))
                                        {
                                            CurrentPath[PlayerId] = AStar::FindPath(Map, CurrentX, CurrentY, SelectX, SelectY);

                                            CurrentMove[PlayerId] = 1;

                                            if (CurrentPath[PlayerId].Points.size() > 2)
                                            {
                                                if (!Interface::Move(Map, CurrentX, CurrentY, CurrentPath[PlayerId].Points[1].X, CurrentPath[PlayerId].Points[1].Y))
                                                {
                                                    DisplayMessage("Path Blocked!", intRD);
                                                }
                                                else
                                                {
                                                    CurrentMove[PlayerId] = 2;

                                                    CycleCombatants();
                                                }
                                            }
                                            else
                                            {
                                                DisplayMessage("Path Blocked!", intRD);

                                                if (WasAttacked)
                                                {
                                                    CycleCombatants();
                                                }
                                            }
                                        }
                                        else if (WasAttacked)
                                        {
                                            CycleCombatants();
                                        }
                                    }
                                    else
                                    {
                                        CurrentMove[PlayerId] = 0;

                                        CurrentPath[PlayerId].Points.clear();

                                        if (!Interface::Move(Map, CurrentX, CurrentY, SelectX, SelectY))
                                        {
                                            DisplayMessage("Path Blocked!", intRD);

                                            if (WasAttacked)
                                            {
                                                CycleCombatants();
                                            }
                                        }
                                        else
                                        {
                                            CycleCombatants();
                                        }
                                    }
                                }
                                else
                                {
                                    CycleCombatants();
                                }
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MOVE && !hold)
                        {
                            if (CurrentMode == Combat::Mode::Normal)
                            {
                                CurrentMode = Combat::Mode::Move;
                            }
                            else if (CurrentMode == Combat::Mode::Attack)
                            {
                                CurrentMode = Combat::Mode::Normal;
                            }
                            else if (CurrentMode == Combat::Mode::Move)
                            {
                                if (CurrentPath[PlayerId].Points.size() > 1 && CurrentMove[PlayerId] >= 0 && CurrentMove[PlayerId] < CurrentPath[PlayerId].Points.size())
                                {
                                    Character::Base &character = party.Members[PlayerId];

                                    // Get attacked by a nearby enemy that has a higher awareness
                                    auto WasAttacked = AttackedUponMoving(Map, monsters, character, PlayerId);

                                    if (WasAttacked)
                                    {
                                        DisplayMessage(("The " + std::string(Character::Description[character.Class]) + " was attacked!").c_str(), intRD);
                                    }

                                    if (Engine::IsAlive(character))
                                    {
                                        auto CurrentX = -1;

                                        auto CurrentY = -1;

                                        Interface::Find(Map, TacticalMap::Object::Player, PlayerId + 1, CurrentX, CurrentY);

                                        if (!Interface::Move(Map, CurrentX, CurrentY, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].X, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].Y))
                                        {
                                            DisplayMessage("Path Blocked!", intRD);
                                        }
                                        else
                                        {
                                            CurrentMove[PlayerId]++;

                                            CycleCombatants();
                                        }
                                    }
                                    else
                                    {
                                        CycleCombatants();
                                    }
                                }
                                else
                                {
                                    CurrentMode = Combat::Mode::Normal;
                                }
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::ATTACK && !hold)
                        {
                            if (CurrentMode == Combat::Mode::Normal)
                            {
                                CurrentMode = Combat::Mode::Attack;
                            }
                            else
                            {
                                CurrentMode = Combat::Mode::Normal;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MONSTER && !hold)
                        {
                            auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                            auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

                            if (CurrentMode == Combat::Mode::Normal)
                            {
                                if (Interface::ValidX(Map, SelectX) && Interface::ValidY(Map, SelectY))
                                {
                                    if (Map.Objects[SelectY][SelectX] == TacticalMap::Object::Monster)
                                    {
                                        auto result = Interface::Find(Sequence, TacticalMap::Object::Monster, Map.ObjectID[SelectY][SelectX] - 1);

                                        if (result != SelectedCombatant)
                                        {
                                            SelectedCombatant = result;
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
                                else
                                {
                                    ResetSelection();
                                }
                            }
                            else if (CurrentMode == Combat::Mode::Attack)
                            {
                                auto MonsterId = Map.ObjectID[SelectY][SelectX] - 1;

                                Character::Base &character = party.Members[PlayerId];

                                Monster::Base &monster = monsters[MonsterId];

                                if (Interface::IsAdjacent(Map, PlayerId, MonsterId) && monster.Endurance > 0)
                                {
                                    DisplayMessage(("The " + std::string(Character::Description[character.Class]) + " attacks the " + monster.Name + "!").c_str(), intLB);

                                    CycleCombatants();
                                }
                            }
                            else if (CurrentMode == Combat::Mode::Move)
                            {
                                DisplayMessage("You cannot move there!", intRD);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_NONE && !hold)
                        {
                            if (CurrentMode == Combat::Mode::Normal)
                            {
                                ResetSelection();
                            }
                            else if (CurrentMode == Combat::Mode::Attack)
                            {
                                CurrentMode = Combat::Mode::Normal;
                            }
                            else if (CurrentMode == Combat::Mode::Move)
                            {
                                DisplayMessage("You cannot move there!", intRD);
                            }
                        }
                    }
                }
                else if (std::get<0>(Sequence[CurrentCombatant]) == TacticalMap::Object::Monster)
                {
                    // monster attacks / moves

                    auto MonsterX = -1;

                    auto MonsterY = -1;

                    auto MonsterId = std::get<1>(Sequence[CurrentCombatant]);

                    Interface::Find(Map, TacticalMap::Object::Monster, MonsterId + 1, MonsterX, MonsterY);

                    auto NearestPlayer = Interface::SelectTarget(Map, party, monsters, Sequence, CurrentCombatant);

                    auto PlayerId = std::get<0>(NearestPlayer);

                    if (PlayerId >= 0 && PlayerId < party.Members.size())
                    {
                        if (std::get<1>(NearestPlayer) <= 1)
                        {
                            // Do Attack
                            DisplayMessage((monsters[MonsterId].Name + " Attacks!").c_str(), intRD);
                        }
                        else
                        {
                            // Close distance
                            auto LocationX = 0;

                            auto LocationY = 0;

                            Interface::Find(Map, TacticalMap::Object::Player, std::get<0>(NearestPlayer) + 1, LocationX, LocationY);

                            auto MonsterPath = AStar::FindPath(Map, MonsterX, MonsterY, LocationX, LocationY);

                            if (MonsterPath.Points.size() > 2)
                            {
                                Move(Map, MonsterX, MonsterY, MonsterPath.Points[1].X, MonsterPath.Points[1].Y);
                            }
                        }
                    }

                    CycleCombatants();
                }

                if (!Engine::IsAlive(party) || !Engine::IsAlive(monsters) || Engine::Escaped(party))
                {
                    done = true;
                }
            }
        }

        return false;
    }
}
#endif