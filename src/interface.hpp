#ifndef __INTERFACE__HPP__
#define __INTERFACE__HPP__

#include "BloodSword.hpp"

namespace Interface
{
    // (Player/Monster, Id, Awareness)
    typedef std::tuple<TacticalMap::Object, int, int> Combatants;
    typedef std::tuple<int, int, int> Targets;

    bool ValidX(TacticalMap::Base &Map, int x)
    {
        return (x >= 0 && x < Map.Width);
    }

    bool ValidY(TacticalMap::Base &Map, int y)
    {
        return (y >= 0 && y < Map.Height);
    }

    void RenderCombatScreen(SDL_Renderer *Renderer, std::vector<Button> &Controls, int Current, Uint32 bg)
    {
        Graphics::FillWindow(Renderer, bg);

        Graphics::RenderButtons(Renderer, Controls, Current, border_space, border_pts);
    }

    void RenderMessage(SDL_Renderer *Renderer, std::vector<Button> &Controls, TacticalMap::Base &Map, Uint32 bg, std::string Message, Uint32 FlashColor)
    {
        Uint32 Duration = 1500;

        auto MapSizeX = Map.SizeX * Map.ObjectSize;

        auto MapSizeY = Map.SizeY * Map.ObjectSize;

        RenderCombatScreen(Renderer, Controls, -1, bg);

        auto FlashW = 3 * MapSizeX / 5;

        auto FlashH = 2 * infoh;

        Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, infoh * 2, Map.DrawX + (MapSizeX - FlashW) / 2, Map.DrawY + (MapSizeY - FlashH) / 2);

        SDL_RenderPresent(Renderer);

        SDL_Delay(Duration);
    }

    void Remove(TacticalMap::Base &Map, int srcX, int srcY)
    {
        if (Interface::ValidX(Map, srcX) && Interface::ValidY(Map, srcY))
        {
            if (Map.Objects[srcY][srcX] == TacticalMap::Object::Player)
            {
                Map.Objects[srcY][srcX] = TacticalMap::Object::Passable;

                Map.ObjectID[srcY][srcX] = 0;
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

                if (HotCoals)
                {
                    Map.Objects[srcY][srcX] = TacticalMap::Object::HotCoals;
                }
                else
                {
                    Map.Objects[srcY][srcX] = TacticalMap::Object::Passable;
                }

                Map.ObjectID[srcY][srcX] = 0;
            }
        }
    }

    bool Move(TacticalMap::Base &Map, int srcX, int srcY, int dstX, int dstY)
    {
        auto result = false;

        if (Interface::ValidX(Map, srcX) && Interface::ValidY(Map, srcY) && Interface::ValidX(Map, dstX) && Interface::ValidY(Map, dstY))
        {
            if (Map.Objects[srcY][srcX] == TacticalMap::Object::Player && Map.Objects[dstY][dstX] == TacticalMap::Object::Passable)
            {
                if (Map.Objects[dstY][dstX] == TacticalMap::Object::Passable)
                {
                    Map.Objects[dstY][dstX] = Map.Objects[srcY][srcX];

                    Map.ObjectID[dstY][dstX] = Map.ObjectID[srcY][srcX];

                    Remove(Map, srcX, srcY);

                    result = true;
                }
            }
            else if (Map.Objects[srcY][srcX] == TacticalMap::Object::Monster && (Map.Objects[dstY][dstX] == TacticalMap::Object::Passable || Map.Objects[dstY][dstX] == TacticalMap::Object::HotCoals))
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

                Remove(Map, srcX, srcY);

                result = true;
            }
        }

        return result;
    }

    bool AnimateMove(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, TacticalMap::Base &Map, Party::Base &party, std::vector<Monster::Base> &monsters, int srcX, int srcY, int dstX, int dstY)
    {
        // do not render off screen animations
        if ((srcX < Map.MapX) || (srcX >= Map.SizeX + Map.MapX) || (srcY < Map.MapY) || (srcY >= Map.SizeY + Map.MapY))
        {
            SDL_Delay(300);

            return Move(Map, srcX, srcY, dstX, dstY);
        }

        auto Animate = [&](SDL_Surface *passable, SDL_Surface *asset)
        {
            auto DeltaX = (dstX - srcX);

            auto DeltaY = (dstY - srcY);

            for (auto i = 0; i < Map.ObjectSize; i += 5)
            {
                RenderCombatScreen(Renderer, BattleScreen, -1, bg);

                Graphics::RenderImage(Renderer, passable, Map.DrawX + (srcX - Map.MapX) * Map.ObjectSize, Map.DrawY + (srcY - Map.MapY) * Map.ObjectSize);

                Graphics::RenderImage(Renderer, asset, Map.DrawX + (srcX - Map.MapX) * Map.ObjectSize + DeltaX * i, Map.DrawY + (srcY - Map.MapY) * Map.ObjectSize + DeltaY * i);

                SDL_RenderPresent(Renderer);

                SDL_Delay(5);
            }
        };

        auto result = false;

        if (Interface::ValidX(Map, srcX) && Interface::ValidY(Map, srcY) && Interface::ValidX(Map, dstX) && Interface::ValidY(Map, dstY))
        {
            if (Map.Objects[srcY][srcX] == TacticalMap::Object::Player && Map.Objects[dstY][dstX] == TacticalMap::Object::Passable)
            {
                auto PlayerId = Map.ObjectID[srcY][srcX] - 1;

                auto passable = Assets::Copy(Assets::Type::Passable);

                SDL_Surface *asset = NULL;

                if (party.Members[PlayerId].Class == Character::Class::Warrior)
                {
                    asset = Assets::Copy(Assets::Type::Warrior);
                }

                Animate(passable, asset);

                SDL_FreeSurface(passable);

                SDL_FreeSurface(asset);

                result = Move(Map, srcX, srcY, dstX, dstY);
            }
            else if (Map.Objects[srcY][srcX] == TacticalMap::Object::Monster && (Map.Objects[dstY][dstX] == TacticalMap::Object::Passable || Map.Objects[dstY][dstX] == TacticalMap::Object::HotCoals))
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

                auto MonsterId = Map.ObjectID[srcY][srcX] - 1;

                auto passable = HotCoals ? Assets::Copy(Assets::Type::HotCoals) : Assets::Copy(Assets::Type::Passable);

                SDL_Surface *asset = NULL;

                if (monsters[MonsterId].Type == Monster::Type::Barbarian)
                {
                    asset = Assets::Copy(Assets::Type::Barbarian);
                }

                Animate(passable, asset);

                SDL_FreeSurface(passable);

                SDL_FreeSurface(asset);

                result = Move(Map, srcX, srcY, dstX, dstY);
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

        for (auto y = 0; y < Map.Height; y++)
        {
            for (auto x = 0; x < Map.Width; x++)
            {
                if (Map.Objects[y][x] == object && Map.ObjectID[y][x] == (id + 1))
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

        Interface::Find(Map, TacticalMap::Object::Player, PlayerId, PlayerX, PlayerY);

        Interface::Find(Map, TacticalMap::Object::Monster, MonsterId, MonsterX, MonsterY);

        auto IsValidX = Interface::ValidX(Map, PlayerX) && Interface::ValidX(Map, MonsterX);

        auto IsValidY = Interface::ValidY(Map, PlayerY) && Interface::ValidX(Map, MonsterY);

        return IsValidX && IsValidY && Interface::Distance(PlayerX, PlayerY, MonsterX, MonsterY) <= 1;
    }

    bool NearbyMonsters(TacticalMap::Base &Map, std::vector<Monster::Base> Monsters, int PlayerId)
    {
        auto result = false;

        for (auto i = 0; i < Monsters.size(); i++)
        {
            result |= (Monsters[i].Endurance > 0 && Interface::IsAdjacent(Map, PlayerId, i));
        }

        return result;
    }

    bool AttackedUponMoving(TacticalMap::Base &Map, std::vector<Monster::Base> &monsters, Character::Base &character, int PlayerId, int &Damages)
    {
        auto WasAttacked = false;

        Damages = 0;

        for (auto i = 0; i < monsters.size(); i++)
        {
            Monster::Base &monster = monsters[i];

            if (monster.Endurance > 0 && Interface::IsAdjacent(Map, PlayerId, i) && monster.Awareness >= Engine::Awareness(character))
            {
                WasAttacked = true;

                Damages -= 1;
            }
        }

        return WasAttacked;
    }

    void DrawPath(SDL_Renderer *Renderer, TacticalMap::Base &Map, AStar::Path &CurrentPath, int CurrentMove, Uint32 color, Uint8 alpha)
    {
        if (CurrentMove > 0 && CurrentMove < CurrentPath.Points.size())
        {
            auto TargetX = CurrentPath.Points.back().X - Map.MapX;

            auto TargetY = CurrentPath.Points.back().Y - Map.MapY;

            if (TargetX >= 0 && TargetX < Map.SizeX && TargetY >= 0 && TargetY < Map.SizeY)
            {
                SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);

                auto Color = O(color, alpha);

                for (auto i = CurrentMove; i < CurrentPath.Points.size() - 1; i++)
                {
                    auto X = CurrentPath.Points[i].X - Map.MapX;

                    auto Y = CurrentPath.Points[i].Y - Map.MapY;

                    if (X >= 0 && X < Map.SizeX && Y >= 0 && Y < Map.SizeY)
                    {
                        Graphics::FillRect(Renderer, Map.ObjectSize, Map.ObjectSize, Map.DrawX + X * Map.ObjectSize, Map.DrawY + Y * Map.ObjectSize, Color);
                    }
                }

                SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_NONE);

                Graphics::ThickRect(Renderer, Map.ObjectSize - 4 * border_pts, Map.ObjectSize - 4 * border_pts, Map.DrawX + TargetX * Map.ObjectSize + 2 * border_pts, Map.DrawY + TargetY * Map.ObjectSize + 2 * border_pts, color, border_pts);
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
                          // on equidistant targets, give priority to the one with less endurance
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

    Interface::Targets SelectTarget(TacticalMap::Base &Map, Party::Base &party, int MonsterId)
    {
        Interface::Targets NearestPlayer = {-1, -1, -1};

        // player id, distance, endurance
        std::vector<Targets> Distances = {};

        auto MonsterX = 0;

        auto MonsterY = 0;

        Interface::Find(Map, TacticalMap::Object::Monster, MonsterId, MonsterX, MonsterY);

        // cycle through the players
        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]) > 0 && !party.Members[i].Escaped)
            {
                auto LocationX = 0;

                auto LocationY = 0;

                Interface::Find(Map, TacticalMap::Object::Player, i, LocationX, LocationY);

                Distances.push_back(std::make_tuple(i, Interface::Distance(MonsterX, MonsterY, LocationX, LocationY), Engine::Endurance(party.Members[i])));
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

    void CharacterSheet(SDL_Renderer *Renderer, Party::Base &party, TTF_Font *Font, int PlayerId, int TextW, int TextX, int TextY)
    {
        auto FontSize = TTF_FontHeight(Font);

        auto FightingProwess = Engine::FightingProwess(party.Members[PlayerId]);
        auto PsychicAbility = Engine::PsychicAbility(party.Members[PlayerId]);
        auto Awareness = Engine::Awareness(party.Members[PlayerId]);
        auto Endurance = Engine::Endurance(party.Members[PlayerId]);
        auto Armour = Engine::Armour(party.Members[PlayerId]);
        auto Damage = party.Members[PlayerId].Damage;
        auto DamageModifier = party.Members[PlayerId].DamageModifier;

        Graphics::PutText(Renderer, Character::Description[party.Members[PlayerId].Class], Font, 0, clrLB, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY);
        Graphics::PutText(Renderer, std::string("RANK: " + std::to_string(party.Members[PlayerId].Rank)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + (FontSize + 2));
        Graphics::PutText(Renderer, std::string("FIGHTING PROWESS: " + std::to_string(FightingProwess)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 2 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("PSYCHIC ABILITY: " + std::to_string(PsychicAbility)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 3 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("AWARENESS: " + std::to_string(Awareness)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 4 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ENDURANCE: " + std::to_string(Endurance)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 5 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ARMOUR RATING: " + std::to_string(Armour)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 6 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("DAMAGE: " + std::to_string(Damage) + (DamageModifier >= 0 ? "D+" : "D") + std::to_string(party.Members[PlayerId].DamageModifier)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 7 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("EXPERIENCE: " + std::to_string(party.Members[PlayerId].ExperiencePoints)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 8 * (FontSize + 2));

        if (party.Members[PlayerId].IsDefending && party.Members[PlayerId].QuickThinking)
        {
            Graphics::PutText(Renderer, "DEFENDING", Font, 0, clrLG, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 9 * (FontSize + 2));
            Graphics::PutText(Renderer, "QUICK THINKING", Font, 0, clrYW, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 10 * (FontSize + 2));
        }
        else if (party.Members[PlayerId].IsDefending)
        {
            Graphics::PutText(Renderer, "DEFENDING", Font, 0, clrLG, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 9 * (FontSize + 2));
        }
        else if (party.Members[PlayerId].QuickThinking)
        {
            Graphics::PutText(Renderer, "QUICK THINKING", Font, 0, clrYW, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 9 * (FontSize + 2));
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
        Graphics::PutText(Renderer, std::string("DAMAGE: " + std::to_string(monsters[MonsterId].Damage) + (monsters[MonsterId].DamageModifier >= 0 ? "D+" : "D") + std::to_string(monsters[MonsterId].DamageModifier)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + 6 * (FontSize + 2));
    }

    void ShowCoordinates(SDL_Renderer *Renderer, TacticalMap::Base &Map, std::vector<Button> &Controls, std::vector<Combatants> &Sequence, int Current, int SelectedCombatant, TTF_Font *Font, int TextW, int TextX)
    {
        auto ControlType = Controls[Current].Type;

        auto FontSize = TTF_FontHeight(Font);

        if ((SelectedCombatant < 0 || SelectedCombatant >= Sequence.size()) && (ControlType == Control::Type::MAP_NONE || ControlType == Control::Type::DESTINATION))
        {
            auto SelectX = Map.MapX + (Controls[Current].X - Map.DrawX) / Map.ObjectSize;

            auto SelectY = Map.MapY + (Controls[Current].Y - Map.DrawY) / Map.ObjectSize;

            std::string Coordinates = "(" + std::to_string(SelectX) + ", " + std::to_string(SelectY) + ")";

            Graphics::PutText(Renderer, Coordinates.c_str(), Font, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, Map.DrawY);

            Graphics::PutText(Renderer, TacticalMap::Description[Map.Objects[SelectY][SelectX]], Font, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, Map.DrawY + (FontSize + text_space));
        }
    }

    void SortCombatants(std::vector<Combatants> &Sequence)
    {
        std::sort(Sequence.begin(), Sequence.end(), [](Combatants &a, Combatants &b) -> bool
                  {
                      // sort combatants based on awareness
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

    // fight encounter between player and monster
    Combat::Result Fight(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, TacticalMap::Base &Map, Character::Base &Character, Monster::Base &Monster, bool Attacked)
    {
        auto Result = Combat::Result::NONE;
        auto MapSizeX = Map.SizeX * Map.ObjectSize;
        auto MapSizeY = Map.SizeY * Map.ObjectSize;
        auto WindowW = 4 * MapSizeX / 5;
        auto WindowH = 4 * MapSizeY / 5;
        auto WindowX = Map.DrawX + (MapSizeX - WindowW) / 2;
        auto WindowY = Map.DrawY + (MapSizeY - WindowH) / 2;
        auto MidWindow = WindowX + (WindowW / 2) + text_space;
        auto ColumnWidth = WindowH / 2 - 4 * text_space;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;

        auto TextButtonX = WindowX + 2 * text_space;
        auto TextButtonY = (WindowY + WindowH) - (text_buttonh + 2 * text_space);
        auto TextWidth = WindowW - 2 * text_space;
        auto ResultsY = 12 * RowHeight + 4 * text_space;

        const char *FightChoices1[2] = {"FIGHT", "CANCEL"}; // player attacks
        auto FightControls1 = Graphics::CreateFixedTextButtons(FightChoices1, 2, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        FightControls1[0].Fg = clrWH;
        FightControls1[0].Highlight = Attacked ? intMG : intLB;
        FightControls1[0].Color = Attacked ? intRD : intDB;
        FightControls1[0].Type = Control::Type::ATTACK;
        FightControls1[1].Fg = clrWH;
        FightControls1[1].Highlight = intLB;
        FightControls1[1].Color = Attacked ? intRD : intDB;
        FightControls1[1].Type = Control::Type::BACK;

        const char *FightChoices2[2] = {"FIGHT"}; // monster attacks
        auto FightControls2 = Graphics::CreateFixedTextButtons(FightChoices2, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        FightControls2[0].Fg = clrWH;
        FightControls2[0].Highlight = Attacked ? intMG : intLB;
        FightControls2[0].Color = Attacked ? intRD : intDB;
        FightControls2[0].Type = Control::Type::ATTACK;

        const char *DoneChoices[1] = {"DONE"}; // end of fighting
        auto DoneControls = Graphics::CreateFixedTextButtons(DoneChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DoneControls[0].Fg = clrWH;
        DoneControls[0].Highlight = Attacked ? intMG : intLB;
        DoneControls[0].Color = Attacked ? intRD : intDB;
        DoneControls[0].Type = Control::Type::BACK;

        const char *DamageChoices[1] = {"DAMAGE"}; // end of fighting
        auto DamageControls = Graphics::CreateFixedTextButtons(DamageChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DamageControls[0].Fg = clrWH;
        DamageControls[0].Highlight = Attacked ? intMG : intLB;
        DamageControls[0].Color = Attacked ? intRD : intDB;
        DamageControls[0].Type = Control::Type::DAMAGE;

        SDL_Surface *dice[6];

        dice[0] = Assets::Copy(Assets::Type::Dice1);
        dice[1] = Assets::Copy(Assets::Type::Dice2);
        dice[2] = Assets::Copy(Assets::Type::Dice3);
        dice[3] = Assets::Copy(Assets::Type::Dice4);
        dice[4] = Assets::Copy(Assets::Type::Dice5);
        dice[5] = Assets::Copy(Assets::Type::Dice6);

        auto swords = Assets::Copy(Assets::Type::Attack);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<TextButton> &Controls = Attacked ? FightControls2 : FightControls1;

        auto done = false;

        auto CurrentStage = Combat::Stage::START;

        auto FightingProwess = Engine::FightingProwess(Character);
        auto Damage = Character.Damage;
        auto DamageModifier = Character.DamageModifier;
        auto Armour = Engine::Armour(Character);

        if (!Engine::HasWeapon(Character))
        {
            FightingProwess = std::max(0, FightingProwess - 2);

            DamageModifier -= 2;
        }

        if (Character.IsDefending && !Attacked)
        {
            Controls = DoneControls;

            CurrentStage = Combat::Stage::END;
        }

        Engine::Randomize();

        auto FightRolls = (Attacked && Character.IsDefending) ? 3 : 2;
        auto DamageRolls = Attacked ? Monster.Damage : Damage;

        std::vector<int> Rolls(FightRolls, 0);
        std::vector<int> Damages(DamageRolls, 0);

        auto DamageSum = 0;
        auto FightingSum = 0;
        bool CalculatedDamage = false;
        bool AssignedDamage = false;

        while (!done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            // character stats
            auto Endurance = Engine::Endurance(Character);
            Graphics::PutText(Renderer, Character::Description[Character.Class], Fonts::Normal, 0, clrLB, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY);
            Graphics::PutText(Renderer, ("FP: " + std::to_string(FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + RowHeight);
            Graphics::PutText(Renderer, ("EN: " + std::to_string(Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 2 * RowHeight);
            Graphics::PutText(Renderer, ("DMG: " + std::to_string(Damage) + "D+" + std::to_string(DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 3 * RowHeight);
            Graphics::PutText(Renderer, ("ARM: " + std::to_string(Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 4 * RowHeight);

            if (Attacked && Character.IsDefending)
            {
                Graphics::PutText(Renderer, "DEFENDING", Fonts::Normal, 0, clrLB, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + 5 * RowHeight);
            }

            // monster stats
            Graphics::PutText(Renderer, Monster.Name.c_str(), Fonts::Normal, 0, clrRD, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY);
            Graphics::PutText(Renderer, ("FP: " + std::to_string(Monster.FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + RowHeight);
            Graphics::PutText(Renderer, ("EN: " + std::to_string(Monster.Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 2 * RowHeight);
            Graphics::PutText(Renderer, ("DMG: " + std::to_string(Monster.Damage) + "D+" + std::to_string(Monster.DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 3 * RowHeight);
            Graphics::PutText(Renderer, ("ARM: " + std::to_string(Monster.Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 4 * RowHeight);

            Graphics::RenderImage(Renderer, swords, MidWindow - (2 * Map.ObjectSize + border_pts), WindowY + Map.ObjectSize);

            if (CurrentStage == Combat::Stage::FIGHT && Result == Combat::Result::NONE)
            {
                FightingSum = 0;

                for (auto i = 0; i < FightRolls; i++)
                {
                    Rolls[i] = Engine::Roll(1, 0);

                    FightingSum += Rolls[i];
                }

                FightingSum = std::max(0, FightingSum);

                // apply trickster's dodging technique
                if (Attacked && Character.Class == Character::Class::Trickster)
                {
                    FightingSum += 1;
                }

                auto FightResult = (!Attacked ? FightingProwess : Monster.FightingProwess) >= FightingSum;

                if (FightResult)
                {
                    Result = Combat::Result::FIGHT;

                    CurrentStage = Combat::Stage::DAMAGE;
                }
                else
                {
                    Result = Combat::Result::UNSUCCESSFUL;

                    CurrentStage = Combat::Stage::END;

                    Controls = DoneControls;
                }
            }
            else if (CurrentStage == Combat::Stage::DAMAGE)
            {
                // show fight results
                for (auto i = 0; i < FightRolls; i++)
                {
                    Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * border_space), TextY + 6 * RowHeight, Map.ObjectSize, Map.ObjectSize);
                }

                Graphics::PutText(Renderer, ("Fight Score: " + std::to_string(FightingSum)).c_str(), Fonts::Normal, 0, clrYW, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                if (Attacked)
                {
                    Graphics::PutText(Renderer, (Monster.Name + " hits the " + std::string(Character::Description[Character.Class]) + "!").c_str(), Fonts::Normal, 0, clrRD, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                }
                else
                {
                    Graphics::PutText(Renderer, ("The " + std::string(Character::Description[Character.Class]) + " hits " + Monster.Name + "!").c_str(), Fonts::Normal, 0, clrLB, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                }

                if (!CalculatedDamage)
                {
                    DamageSum = 0;

                    // compute damage
                    for (auto i = 0; i < DamageRolls; i++)
                    {
                        Damages[i] = Engine::Roll(1, 0);

                        DamageSum += Damages[i];
                    }

                    DamageSum += Attacked ? Monster.DamageModifier : DamageModifier;

                    DamageSum -= Attacked ? Armour : Monster.Armour;

                    DamageSum = std::max(0, DamageSum);

                    CalculatedDamage = true;
                }
            }
            else if (CurrentStage == Combat::Stage::END)
            {
                if (Result == Combat::Result::UNSUCCESSFUL)
                {
                    // show fight results
                    for (auto i = 0; i < FightRolls; i++)
                    {
                        Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * border_space), TextY + 6 * RowHeight, Map.ObjectSize, Map.ObjectSize);
                    }

                    Graphics::PutText(Renderer, ("Fight Score: " + std::to_string(FightingSum)).c_str(), Fonts::Normal, 0, clrYW, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                    if (Attacked)
                    {
                        Graphics::PutText(Renderer, (Monster.Name + "'s attack was unsuccessful!").c_str(), Fonts::Normal, 0, clrLB, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                    }
                    else
                    {
                        Graphics::PutText(Renderer, ("The " + std::string(Character::Description[Character.Class]) + "'s attack was unsuccessful!").c_str(), Fonts::Normal, 0, clrRD, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                    }
                }
                else
                {
                    // show damage results
                    for (auto i = 0; i < DamageRolls; i++)
                    {
                        Graphics::StretchImage(Renderer, dice[Damages[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * border_space), TextY + 6 * RowHeight, Map.ObjectSize, Map.ObjectSize);
                    }

                    Graphics::PutText(Renderer, ("Damage Dealt (-Armour): " + std::to_string(DamageSum)).c_str(), Fonts::Normal, 0, Attacked ? clrRD : clrLG, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                    if (!AssignedDamage)
                    {
                        if (Attacked)
                        {
                            Engine::Gain(Character, Attributes::Type::Endurance, -DamageSum);
                        }
                        else
                        {
                            Engine::Gain(Monster, -DamageSum);
                        }

                        AssignedDamage = true;
                    }
                }
            }

            Graphics::RenderTextButtons(Renderer, Controls, FONT_BOOKMAN, Current, 24, TTF_STYLE_NORMAL);

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::ATTACK && !Hold)
                {
                    CurrentStage = Combat::Stage::FIGHT;

                    Controls = DamageControls;
                }
                else if (Controls[Current].Type == Control::Type::DAMAGE && !Hold)
                {
                    CurrentStage = Combat::Stage::END;

                    Controls = DoneControls;
                }
                else if (Controls[Current].Type == Control::Type::BACK && !Hold)
                {
                    done = true;
                }
            }
        }

        for (auto i = 0; i < 6; i++)
        {
            if (dice[i])
            {
                SDL_FreeSurface(dice[i]);

                dice[i] = NULL;
            }
        }

        SDL_FreeSurface(swords);

        swords = NULL;

        return Result;
    }

    Combat::Result CombatScreen(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<std::string> &map, Party::Base &party, std::vector<Monster::Base> &monsters)
    {
        auto FlashMessage = false;

        auto FlashColor = intRD;

        std::string Message = "";

        Uint32 StartTicks = 0;

        Uint32 Duration = 3000;

        auto DisplayMessage = [&](std::string msg, Uint32 color)
        {
            FlashMessage = true;

            Message = msg;

            FlashColor = color;

            StartTicks = SDL_GetTicks();
        };

        auto Map = TacticalMap::Base(map, party, monsters);

        // offsets used to display tactical map
        Map.MapX = 0;

        Map.MapY = 0;

        Map.ObjectSize = 64;

        auto PaddingX = 2 * startx + 2 * Map.ObjectSize;

        auto PaddingY = starty + Map.ObjectSize;

        // size of viewable grid
        Map.SizeX = (SCREEN_WIDTH - 2 * PaddingX) / Map.ObjectSize;

        Map.SizeY = (SCREEN_HEIGHT - 2 * PaddingY) / Map.ObjectSize;

        if (Map.SizeX > Map.Width)
        {
            Map.SizeX = Map.Width;
        }

        if (Map.SizeY > Map.Height)
        {
            Map.SizeY = Map.Height;
        }

        // map at the center of the screen
        Map.DrawX = PaddingX / 2;

        Map.DrawY = Map.ObjectSize;

        // player input
        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        auto CurrentPath = std::vector<AStar::Path>(party.Members.size());

        auto CurrentMove = std::vector<int>(party.Members.size());

        // round sequence
        std::vector<Interface::Combatants> Sequence = {};

        // sort combatants based on awareness
        for (auto i = 0; i < party.Members.size(); i++)
        {
            auto Awareness = Engine::Awareness(party.Members[i]);

            Sequence.push_back(std::make_tuple(TacticalMap::Object::Player, i, Awareness));
        }

        for (auto i = 0; i < monsters.size(); i++)
        {
            auto Awareness = monsters[i].Awareness;

            Sequence.push_back(std::make_tuple(TacticalMap::Object::Monster, i, Awareness));
        }

        SortCombatants(Sequence);

        auto CurrentMode = Combat::Mode::NORMAL;

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

        auto QuickThinkingRound = false;

        auto IsPlayer = [&](int id)
        {
            return std::get<0>(Sequence[id]) == TacticalMap::Object::Player;
        };

        auto IsMonster = [&](int id)
        {
            return std::get<0>(Sequence[id]) == TacticalMap::Object::Monster;
        };

        auto Type = [&](int id)
        {
            return std::get<0>(Sequence[id]);
        };

        auto GetId = [&](int id)
        {
            return std::get<1>(Sequence[id]);
        };

        auto Target = [&](Interface::Targets target)
        {
            return std::get<0>(target);
        };

        auto TargetDistance = [&](Interface::Targets target)
        {
            return std::get<1>(target);
        };

        auto NextQuickThinker = [&]()
        {
            auto next = 0;

            for (auto i = 0; i < Sequence.size(); i++)
            {
                if (IsPlayer(i))
                {
                    if (party.Members[GetId(i)].QuickThinking)
                    {
                        next = i;

                        break;
                    }
                }
            }

            return next;
        };

        auto ClearDefendingStatus = [&]()
        {
            // clear defensive stance
            for (auto i = 0; i < party.Members.size(); i++)
            {
                party.Members[i].IsDefending = false;
            }
        };

        auto CycleCombatants = [&]()
        {
            if (IsPlayer(CurrentCombatant))
            {
                if (QuickThinkingRound && party.Members[GetId(CurrentCombatant)].QuickThinking)
                {
                    party.Members[GetId(CurrentCombatant)].QuickThinking = false;
                }
            }

            auto active = false;

            while (!active)
            {
                if (!Engine::IsAlive(party) || !Engine::IsAlive(monsters) || Engine::Escaped(party))
                {
                    QuickThinkingRound = false;

                    break;
                }

                if (QuickThinkingRound)
                {
                    if (Engine::QuickThinking(party))
                    {
                        CurrentCombatant = NextQuickThinker();
                    }
                    else
                    {
                        QuickThinkingRound = false;

                        CurrentCombatant = 0;

                        ClearDefendingStatus();

                        CombatRound++;
                    }
                }
                else
                {
                    CurrentCombatant++;

                    if (CurrentCombatant >= Sequence.size())
                    {
                        if (Engine::QuickThinking(party))
                        {
                            QuickThinkingRound = true;

                            CurrentCombatant = NextQuickThinker();
                        }
                        else
                        {
                            QuickThinkingRound = false;

                            CurrentCombatant = 0;

                            CombatRound++;

                            ClearDefendingStatus();
                        }
                    }
                }

                if (IsPlayer(CurrentCombatant))
                {
                    auto character = party.Members[GetId(CurrentCombatant)];

                    active = Engine::IsAlive(character) && !character.Escaped;
                }
                else if (IsMonster(CurrentCombatant))
                {
                    active = monsters[GetId(CurrentCombatant)].Endurance > 0;
                }
            }

            CurrentMode = Combat::Mode::NORMAL;

            ResetSelection();
        };

        auto FontSize = TTF_FontHeight(Fonts::Normal);

        auto TextX = Map.DrawX;

        auto TextR = Map.DrawX + (Map.ObjectSize * Map.SizeX + border_space);

        auto TextWidthR = SCREEN_WIDTH - TextR;

        auto TextY = Map.DrawY - 2 * border_space - FontSize;

        auto TextWidth = Map.Width * Map.ObjectSize;

        auto MapButtonSize = Map.ObjectSize + 2 * border_space;
        auto MapSizeX = Map.SizeX * Map.ObjectSize;
        auto MapSizeY = Map.SizeY * Map.ObjectSize;
        auto MapButtonsX = Map.DrawX - MapButtonSize;
        auto MapButtonsY = Map.DrawY + border_space;
        auto MapButtonsGridSize = MapSizeY / 4;

        auto ActionsX = Map.DrawX;
        auto ActionsY = Map.DrawY + MapSizeY + 2 * border_space;
        auto ActionsGrid = MapButtonSize;

        auto RenderFlashMessage = [&]()
        {
            if (FlashMessage)
            {
                if ((SDL_GetTicks() - StartTicks) < Duration)
                {
                    auto FlashW = 3 * MapSizeX / 5;

                    auto FlashH = 2 * infoh;

                    Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, infoh * 2, Map.DrawX + (MapSizeX - FlashW) / 2, Map.DrawY + (MapSizeY - FlashH) / 2);
                }
                else
                {
                    FlashMessage = false;
                }
            }
        };

        if (Window && Renderer && Map.Width > 0 && Map.Height > 0)
        {
            auto done = false;

            while (!done)
            {
                // check if out of bounds
                if (Map.MapX < 0)
                {
                    Map.MapX = 0;
                }

                if (Map.MapX > Map.Width - Map.SizeX)
                {
                    Map.MapX = Map.Width - Map.SizeX;
                }

                if (Map.MapY < 0)
                {
                    Map.MapY = 0;
                }

                if (Map.MapY > Map.Height - Map.SizeY)
                {
                    Map.MapY = Map.Height - Map.SizeY;
                }

                auto Controls = std::vector<Button>();

                auto StartMap = 14;
                auto BottomMapX = StartMap + (Map.SizeX * (Map.SizeY - 1));
                auto MidMapY = StartMap + (Map.SizeY / 2 * Map.SizeX) - Map.SizeX;

                Controls.push_back(Button(0, Assets::Get(Assets::Type::Up), 0, StartMap, 0, 1, MapButtonsX, MapButtonsY, Map.MapY > 0 ? intLB : intGR, Control::Type::MAP_UP));
                Controls.push_back(Button(1, Assets::Get(Assets::Type::Left), 1, MidMapY, 0, 2, MapButtonsX, MapButtonsY + (MapButtonsGridSize + 2 * border_space), Map.MapX > 0 ? intLB : intGR, Control::Type::MAP_LEFT));
                Controls.push_back(Button(2, Assets::Get(Assets::Type::Right), 2, MidMapY + Map.SizeX, 1, 3, MapButtonsX, MapButtonsY + 2 * (MapButtonsGridSize + 2 * border_space), (Map.MapX < Map.Width - Map.SizeX) ? intLB : intGR, Control::Type::MAP_RIGHT));
                Controls.push_back(Button(3, Assets::Get(Assets::Type::Down), 3, BottomMapX, 2, 5, MapButtonsX, MapButtonsY + 3 * (MapButtonsGridSize + 2 * border_space), (Map.MapY < Map.Height - Map.SizeY) ? intLB : intGR, Control::Type::MAP_DOWN));
                Controls.push_back(Button(4, Assets::Get(Assets::Type::Back), StartMap - 1, 4, StartMap - 1, 4, lastx, buttony, intLB, Control::Type::EXIT));
                Controls.push_back(Button(5, Assets::Get(Assets::Type::Items), 3, 6, BottomMapX, 5, ActionsX, ActionsY, intLB, Control::Type::ITEMS));
                Controls.push_back(Button(6, Assets::Get(Assets::Type::Move), 5, 7, BottomMapX + 1, 6, ActionsX + ActionsGrid, ActionsY, intLB, Control::Type::MOVE));
                Controls.push_back(Button(7, Assets::Get(Assets::Type::Heal), 6, 8, BottomMapX + 2, 7, ActionsX + 2 * ActionsGrid, ActionsY, intLB, Control::Type::HEAL));
                Controls.push_back(Button(8, Assets::Get(Assets::Type::Attack), 7, 9, BottomMapX + 3, 8, ActionsX + 3 * ActionsGrid, ActionsY, intLB, Control::Type::ATTACK));
                Controls.push_back(Button(9, Assets::Get(Assets::Type::Defend), 8, 10, BottomMapX + 4, 9, ActionsX + 4 * ActionsGrid, ActionsY, intLB, Control::Type::DEFEND));
                Controls.push_back(Button(10, Assets::Get(Assets::Type::Shoot), 9, 11, BottomMapX + 5, 10, ActionsX + 5 * ActionsGrid, ActionsY, intLB, Control::Type::SHOOT));
                Controls.push_back(Button(11, Assets::Get(Assets::Type::Memorize), 10, 12, BottomMapX + 6, 11, ActionsX + 6 * ActionsGrid, ActionsY, intLB, Control::Type::MEMORIZE));
                Controls.push_back(Button(12, Assets::Get(Assets::Type::Magic), 11, 13, BottomMapX + 7, 12, ActionsX + 7 * ActionsGrid, ActionsY, intLB, Control::Type::MAGIC));
                Controls.push_back(Button(13, Assets::Get(Assets::Type::Flee), 12, 4, BottomMapX + 8, 4, ActionsX + 8 * ActionsGrid, ActionsY, intLB, Control::Type::FLEE));

                int NumControls = Controls.size();

                // select which object to blink (player/monster)
                auto BlinkX = -1;

                auto BlinkY = -1;

                // render objects within the map window
                for (auto y = Map.MapY; y < Map.MapY + Map.SizeY; y++)
                {
                    auto AssetY = Map.DrawY + (y - Map.MapY) * Map.ObjectSize;

                    auto CtrlY = (y - Map.MapY);

                    for (auto x = Map.MapX; x < Map.MapX + Map.SizeX; x++)
                    {
                        auto CtrlUp = NumControls;
                        auto CtrlDn = NumControls;
                        auto CtrlLt = NumControls;
                        auto CtrlRt = NumControls;

                        auto CtrlX = (x - Map.MapX);

                        if (CtrlY > 0)
                        {
                            CtrlUp = NumControls - Map.SizeX;
                        }

                        if (CtrlY < Map.SizeY - 1)
                        {
                            CtrlDn = NumControls + Map.SizeX;
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
                            if (CtrlY < Map.SizeY / 2)
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
                                if (CtrlY == Map.SizeY - 1)
                                {
                                    CtrlLt = 3;
                                }
                                else
                                {
                                    CtrlLt = 2;
                                }
                            }
                        }

                        if (CtrlX < Map.SizeX - 1)
                        {
                            CtrlRt = NumControls + 1;
                        }

                        auto AssetX = Map.DrawX + (x - Map.MapX) * Map.ObjectSize;

                        auto Object = Map.Objects[y][x];

                        auto ObjectId = Map.ObjectID[y][x] - 1;

                        if (Object == TacticalMap::Object::Wall)
                        {
                            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Wall), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::MAP_NONE));
                        }
                        else if (Object == TacticalMap::Object::Player)
                        {
                            if (GetId(CurrentCombatant) == ObjectId)
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
                                Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Warrior), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::PLAYER));
                            }
                        }
                        else if (Object == TacticalMap::Object::HotCoals)
                        {
                            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::HotCoals), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::MAP_NONE));
                        }
                        else if (Object == TacticalMap::Object::Monster)
                        {
                            if (monsters[ObjectId].Type == Monster::Type::Barbarian)
                            {
                                Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Barbarian), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::MONSTER));
                            }
                        }
                        else
                        {
                            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Passable), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::DESTINATION));
                        }

                        NumControls++;
                    }
                }

                Interface::RenderCombatScreen(Renderer, Controls, Current, intBK);

                if (Current >= 0 && Current < Controls.size())
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                }

                // blink current party member
                if (SelectedCombatant != CurrentCombatant && Blink && BlinkX >= 0 && BlinkY >= 0 && (Current == -1 || (Current >= 0 && Current < Controls.size() && (BlinkX != Controls[Current].X || BlinkY != Controls[Current].Y))))
                {
                    Graphics::ThickRect(Renderer, Map.ObjectSize - 4 * border_pts, Map.ObjectSize - 4 * border_pts, BlinkX + 2 * border_pts, BlinkY + 2 * border_pts, intRD, border_pts);
                }

                if (SelectedCombatant >= 0 && SelectedCombatant < Sequence.size())
                {
                    auto SelectedX = 0;

                    auto SelectedY = 0;

                    auto SelectedId = GetId(SelectedCombatant);

                    Interface::Find(Map, Type(SelectedCombatant), SelectedId, SelectedX, SelectedY);

                    if ((SelectedX - Map.MapX) >= 0 && (SelectedX - Map.MapX) < Map.SizeX && (SelectedY - Map.MapY) >= 0 && (SelectedY - Map.MapY) < Map.SizeY)
                    {
                        Graphics::ThickRect(Renderer, Map.ObjectSize - 4 * border_pts, Map.ObjectSize - 4 * border_pts, Map.DrawX + (SelectedX - Map.MapX) * Map.ObjectSize + 2 * border_pts, Map.DrawY + (SelectedY - Map.MapY) * Map.ObjectSize + 2 * border_pts, intYW, border_pts);
                    }

                    // Render statistics for currently selected / highlighted player or monster
                    if (IsPlayer(SelectedCombatant))
                    {
                        Interface::CharacterSheet(Renderer, party, Fonts::Fixed, SelectedId, TextWidthR, TextR, Map.DrawY);
                    }
                    else if (IsMonster(SelectedCombatant))
                    {
                        Interface::MonsterData(Renderer, monsters, Fonts::Fixed, SelectedId, TextWidthR, TextR, Map.DrawY);

                        // Show potential target
                        auto NearestPlayer = Interface::SelectTarget(Map, party, GetId(SelectedCombatant));

                        auto PlayerId = Target(NearestPlayer);

                        if (PlayerId >= 0 && PlayerId < party.Members.size())
                        {
                            auto MonsterX = -1;

                            auto MonsterY = -1;

                            auto LocationX = -1;

                            auto LocationY = -1;

                            Interface::Find(Map, TacticalMap::Object::Monster, SelectedId, MonsterX, MonsterY);

                            Interface::Find(Map, TacticalMap::Object::Player, PlayerId, LocationX, LocationY);

                            if (Interface::ValidX(Map, MonsterX) && Interface::ValidY(Map, MonsterY) && Interface::ValidX(Map, LocationX) && Interface::ValidY(Map, LocationY))
                            {
                                auto MonsterPath = AStar::FindPath(Map, MonsterX, MonsterY, LocationX, LocationY);

                                if (MonsterPath.Points.size() > 2)
                                {
                                    Interface::DrawPath(Renderer, Map, MonsterPath, 1, intRD, 0x66);
                                }
                            }
                        }
                    }
                }

                if (Current >= 0 && Current < Controls.size())
                {
                    auto ControlType = Controls[Current].Type;

                    auto SelectX = -1;

                    auto SelectY = -1;

                    if (Controls[Current].X >= Map.DrawX && Controls[Current].X <= Map.DrawX + MapSizeX && Controls[Current].Y >= Map.DrawY && Controls[Current].Y <= Map.DrawY + MapSizeY)
                    {
                        SelectX = Map.MapX + (Controls[Current].X - Map.DrawX) / Map.ObjectSize;

                        SelectY = Map.MapY + (Controls[Current].Y - Map.DrawY) / Map.ObjectSize;
                    }

                    Interface::ShowCoordinates(Renderer, Map, Controls, Sequence, Current, SelectedCombatant, Fonts::Normal, TextWidth, TextR);

                    if (CurrentMode == Combat::Mode::NORMAL && ControlType == Control::Type::PLAYER)
                    {
                        Graphics::PutText(Renderer, "View party member", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                        Interface::CharacterSheet(Renderer, party, Fonts::Fixed, Map.ObjectID[SelectY][SelectX] - 1, TextWidthR, TextR, Map.DrawY);
                    }
                    else if (CurrentMode == Combat::Mode::NORMAL && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "View opponent", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                        Interface::MonsterData(Renderer, monsters, Fonts::Fixed, Map.ObjectID[SelectY][SelectX] - 1, TextWidthR, TextR, Map.DrawY);
                    }
                    else if (CurrentMode == Combat::Mode::NORMAL)
                    {
                        Graphics::PutText(Renderer, ("Combat Round: " + std::to_string(CombatRound + 1)).c_str(), Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::MOVE)
                    {
                        auto PlayerId = GetId(CurrentCombatant);

                        auto TargetX = -1;

                        auto TargetY = -1;

                        if (CurrentMove[PlayerId] > 0 && CurrentMove[PlayerId] < CurrentPath[PlayerId].Points.size())
                        {
                            TargetX = CurrentPath[PlayerId].Points.back().X - Map.MapX;

                            TargetY = CurrentPath[PlayerId].Points.back().Y - Map.MapY;

                            Graphics::PutText(Renderer, "Move to location or continue along current path", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                            Interface::DrawPath(Renderer, Map, CurrentPath[PlayerId], CurrentMove[PlayerId], intYW, 0x66);
                        }
                        else
                        {
                            Graphics::PutText(Renderer, "Move to location", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                        }

                        if (ControlType == Control::Type::DESTINATION)
                        {
                            if ((TargetX != SelectX || TargetY != SelectY) && ControlType == Control::Type::DESTINATION)
                            {
                                auto PlayerX = -1;

                                auto PlayerY = -1;

                                Interface::Find(Map, TacticalMap::Object::Player, PlayerId, PlayerX, PlayerY);

                                if (Interface::ValidX(Map, PlayerX) && Interface::ValidY(Map, PlayerY) && Interface::Distance(PlayerX, PlayerY, SelectX, SelectY) > 1)
                                {
                                    auto TempPath = AStar::FindPath(Map, PlayerX, PlayerY, SelectX, SelectY);

                                    if (TempPath.Points.size() > 2)
                                    {
                                        Interface::DrawPath(Renderer, Map, TempPath, 1, intMG, 0x66);
                                    }
                                }
                            }
                        }
                    }
                    else if (CurrentMode == Combat::Mode::HEAL && ControlType == Control::Type::PLAYER)
                    {
                        Graphics::PutText(Renderer, "Heal target", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::ATTACK && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "Fight target", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                        Interface::MonsterData(Renderer, monsters, Fonts::Fixed, Map.ObjectID[SelectY][SelectX] - 1, TextWidthR, TextR, Map.DrawY);
                    }
                    else if (CurrentMode == Combat::Mode::ATTACK)
                    {
                        Graphics::PutText(Renderer, "Select a nearby opponent to fight", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::SHOOT && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "Shoot at target", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                        Interface::MonsterData(Renderer, monsters, Fonts::Fixed, Map.ObjectID[SelectY][SelectX] - 1, TextWidthR, TextR, Map.DrawY);
                    }
                    else if (CurrentMode == Combat::Mode::MAGIC && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "Cast a spell", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                        Interface::MonsterData(Renderer, monsters, Fonts::Fixed, Map.ObjectID[SelectY][SelectX] - 1, TextWidthR, TextR, Map.DrawY);
                    }
                    else if ((ControlType == Control::Type::MAP_NONE || ControlType == Control::Type::DESTINATION) && (SelectedCombatant < 0 || SelectedCombatant >= Sequence.size()))
                    {
                        std::string Coordinates = "(" + std::to_string(SelectX) + ", " + std::to_string(SelectY) + ")";

                        Graphics::PutText(Renderer, Coordinates.c_str(), Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextR, Map.DrawY);

                        Graphics::PutText(Renderer, TacticalMap::Description[Map.Objects[SelectY][SelectX]], Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextR, Map.DrawY + (FontSize + text_space));
                    }
                }

                RenderFlashMessage();

                // get player input
                if (IsPlayer(CurrentCombatant))
                {
                    Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

                    auto PlayerId = GetId(CurrentCombatant);

                    Character::Base &Character = party.Members[PlayerId];

                    auto CurrentX = -1;

                    auto CurrentY = -1;

                    Interface::Find(Map, TacticalMap::Object::Player, PlayerId, CurrentX, CurrentY);

                    if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
                    {
                        auto SelectX = -1;

                        auto SelectY = -1;

                        if (Controls[Current].X >= Map.DrawX && Controls[Current].X <= Map.DrawX + MapSizeX && Controls[Current].Y >= Map.DrawY && Controls[Current].Y <= Map.DrawY + MapSizeY)
                        {
                            SelectX = Map.MapX + (Controls[Current].X - Map.DrawX) / Map.ObjectSize;

                            SelectY = Map.MapY + (Controls[Current].Y - Map.DrawY) / Map.ObjectSize;
                        }

                        if (Controls[Current].Type == Control::Type::EXIT && !Hold)
                        {
                            Current = -1;

                            done = true;
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_UP || ScrollUp)
                        {
                            if (Map.MapY > 0)
                            {
                                Map.MapY--;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_DOWN || ScrollDown)
                        {
                            if (Map.MapY < Map.Height - Map.SizeY)
                            {
                                Map.MapY++;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_LEFT)
                        {
                            Map.MapX--;
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_RIGHT)
                        {
                            Map.MapX++;
                        }
                        else if (Controls[Current].Type == Control::Type::DEFEND)
                        {
                            Character.IsDefending = true;

                            CycleCombatants();

                            Selected = false;

                            Current = -1;
                        }
                        else if (Controls[Current].Type == Control::Type::PLAYER && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
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
                            else if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                DisplayMessage("You cannot fight that!", intRD);
                            }
                            else
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::DESTINATION && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                            else if (CurrentMode == Combat::Mode::MOVE)
                            {
                                if (Interface::Distance(CurrentX, CurrentY, SelectX, SelectY) > 1)
                                {
                                    CurrentPath[PlayerId] = AStar::FindPath(Map, CurrentX, CurrentY, SelectX, SelectY);

                                    CurrentMove[PlayerId] = 1;

                                    auto Damages = 0;

                                    // get attacked by a nearby enemy that has a higher awareness
                                    auto WasAttacked = AttackedUponMoving(Map, monsters, Character, PlayerId, Damages);

                                    if (CurrentPath[PlayerId].Points.size() > 2)
                                    {
                                        if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, party, monsters, CurrentX, CurrentY, CurrentPath[PlayerId].Points[1].X, CurrentPath[PlayerId].Points[1].Y))
                                        {
                                            DisplayMessage("Path Blocked!", intRD);
                                        }
                                        else
                                        {
                                            if (WasAttacked)
                                            {
                                                RenderMessage(Renderer, Controls, Map, intBK, ("The " + std::string(Character::Description[Character.Class]) + " was attacked!"), intRD);

                                                Engine::Gain(Character, Attributes::Type::Endurance, Damages);

                                                if (!Engine::IsAlive(Character))
                                                {
                                                    Remove(Map, CurrentPath[PlayerId].Points[1].X, CurrentPath[PlayerId].Points[1].Y);
                                                }
                                            }

                                            CurrentMove[PlayerId] = 2;

                                            CycleCombatants();
                                        }
                                    }
                                    else
                                    {
                                        DisplayMessage("Path Blocked!", intRD);
                                    }
                                }
                                else
                                {
                                    CurrentMove[PlayerId] = 0;

                                    CurrentPath[PlayerId].Points.clear();

                                    auto Damages = 0;

                                    auto WasAttacked = AttackedUponMoving(Map, monsters, Character, PlayerId, Damages);

                                    if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, party, monsters, CurrentX, CurrentY, SelectX, SelectY))
                                    {
                                        DisplayMessage("Path Blocked!", intRD);
                                    }
                                    else
                                    {
                                        if (WasAttacked)
                                        {
                                            RenderMessage(Renderer, Controls, Map, intBK, ("The " + std::string(Character::Description[Character.Class]) + " was attacked!"), intRD);

                                            Engine::Gain(Character, Attributes::Type::Endurance, Damages);

                                            if (!Engine::IsAlive(Character))
                                            {
                                                Remove(Map, SelectX, SelectY);
                                            }
                                        }

                                        CycleCombatants();
                                    }
                                }
                            }
                            else
                            {
                                CurrentMode = Combat::Mode::NORMAL;

                                ResetSelection();
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MOVE && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                CurrentMode = Combat::Mode::MOVE;
                            }
                            else if (CurrentMode == Combat::Mode::MOVE)
                            {
                                if (CurrentPath[PlayerId].Points.size() > 1 && CurrentMove[PlayerId] >= 0 && CurrentMove[PlayerId] < CurrentPath[PlayerId].Points.size())
                                {
                                    if (Engine::IsAlive(Character))
                                    {
                                        auto Damages = 0;

                                        auto WasAttacked = AttackedUponMoving(Map, monsters, Character, PlayerId, Damages);

                                        if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, party, monsters, CurrentX, CurrentY, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].X, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].Y))
                                        {
                                            DisplayMessage("Path Blocked!", intRD);
                                        }
                                        else
                                        {
                                            // Get attacked by a nearby enemy that has a higher awareness
                                            if (WasAttacked)
                                            {
                                                RenderMessage(Renderer, Controls, Map, intBK, ("The " + std::string(Character::Description[Character.Class]) + " was attacked!"), intRD);

                                                Engine::Gain(Character, Attributes::Type::Endurance, Damages);

                                                if (!Engine::IsAlive(Character))
                                                {
                                                    Remove(Map, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].X, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].Y);
                                                }
                                            }

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
                                    CurrentMode = Combat::Mode::NORMAL;
                                }
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::ATTACK && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                if (Interface::NearbyMonsters(Map, monsters, PlayerId))
                                {
                                    CurrentMode = Combat::Mode::ATTACK;
                                }
                                else
                                {
                                    DisplayMessage("There are no opponents nearby!", intRD);
                                }
                            }
                            else
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MONSTER && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::NORMAL)
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
                            else if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                auto MonsterId = Map.ObjectID[SelectY][SelectX] - 1;

                                Character::Base &character = party.Members[PlayerId];

                                Monster::Base &monster = monsters[MonsterId];

                                if (Interface::IsAdjacent(Map, PlayerId, MonsterId) && monster.Endurance > 0)
                                {
                                    auto Result = Interface::Fight(Renderer, Controls, intBK, Map, character, monster, false);

                                    if (!Engine::IsAlive(character))
                                    {
                                        Remove(Map, CurrentX, CurrentY);
                                    }
                                    else if (monster.Endurance <= 0)
                                    {
                                        Remove(Map, SelectX, SelectY);
                                    }

                                    if (Result != Combat::Result::NONE)
                                    {
                                        CycleCombatants();
                                    }
                                    else
                                    {
                                        DisplayMessage("Attack canceled", intLB);
                                    }

                                    Selected = false;

                                    Current = -1;
                                }
                                else if (monster.Endurance > 0)
                                {
                                    DisplayMessage("You can only attack adjacent targets!", intRD);

                                    CurrentMode = Combat::Mode::NORMAL;
                                }
                            }
                            else if (CurrentMode == Combat::Mode::MOVE)
                            {
                                DisplayMessage("You cannot move there!", intRD);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_NONE && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                ResetSelection();
                            }
                            else if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                            else if (CurrentMode == Combat::Mode::MOVE)
                            {
                                DisplayMessage("You cannot move there!", intRD);
                            }
                        }
                    }

                    Selected = false;
                }
                else if (IsMonster(CurrentCombatant))
                {
                    // monster attacks / moves

                    auto MonsterX = -1;

                    auto MonsterY = -1;

                    auto MonsterId = GetId(CurrentCombatant);

                    Interface::Find(Map, TacticalMap::Object::Monster, MonsterId, MonsterX, MonsterY);

                    auto NearestPlayer = Interface::SelectTarget(Map, party, GetId(CurrentCombatant));

                    auto PlayerId = Target(NearestPlayer);

                    if (PlayerId >= 0 && PlayerId < party.Members.size())
                    {
                        if (TargetDistance(NearestPlayer) <= 1)
                        {
                            auto CurrentX = -1;

                            auto CurrentY = -1;

                            Interface::Find(Map, TacticalMap::Object::Player, PlayerId, CurrentX, CurrentY);

                            // do attack
                            Interface::Fight(Renderer, Controls, intBK, Map, party.Members[PlayerId], monsters[MonsterId], true);

                            if (!Engine::IsAlive(party.Members[PlayerId]))
                            {
                                Remove(Map, CurrentX, CurrentY);
                            }
                            else if (monsters[MonsterId].Endurance <= 0)
                            {
                                Remove(Map, MonsterX, MonsterY);
                            }
                        }
                        else
                        {
                            // close distance
                            auto LocationX = 0;

                            auto LocationY = 0;

                            Interface::Find(Map, TacticalMap::Object::Player, Target(NearestPlayer), LocationX, LocationY);

                            auto MonsterPath = AStar::FindPath(Map, MonsterX, MonsterY, LocationX, LocationY);

                            if (MonsterPath.Points.size() > 2)
                            {
                                Interface::AnimateMove(Renderer, Controls, intBK, Map, party, monsters, MonsterX, MonsterY, MonsterPath.Points[1].X, MonsterPath.Points[1].Y);
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

        if (Engine::Escaped(party))
        {
            return Combat::Result::ESCAPED;
        }
        else if (Engine::IsAlive(party))
        {
            return Combat::Result::VICTORY;
        }

        return Combat::Result::DEFEAT;
    }
}
#endif