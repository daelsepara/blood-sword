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
        return (x >= 0 && x < Map.SizeX);
    }

    bool ValidY(TacticalMap::Base &Map, int y)
    {
        return (y >= 0 && y < Map.SizeY);
    }

    void RenderCombatScreen(SDL_Renderer *Renderer, std::vector<Button> &Controls, int Current, Uint32 bg)
    {
        Graphics::FillWindow(Renderer, bg);

        Graphics::RenderButtons(Renderer, Controls, Current, border_space, border_pts);
    }

    void RenderMessage(SDL_Renderer *Renderer, std::vector<Button> &Controls, Uint32 bg, int SizeX, int SizeY, int DrawX, int DrawY, int ObjectSize, std::string Message, Uint32 FlashColor)
    {
        Uint32 Duration = 1500;

        auto MapSizeX = SizeX * ObjectSize;

        auto MapSizeY = SizeY * ObjectSize;

        RenderCombatScreen(Renderer, Controls, -1, bg);

        auto FlashW = 3 * MapSizeX / 5;

        auto FlashH = 2 * infoh;

        Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, infoh * 2, DrawX + (MapSizeX - FlashW) / 2, DrawY + (MapSizeY - FlashH) / 2);

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

    bool AnimateMove(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, TacticalMap::Base &Map, Party::Base &party, std::vector<Monster::Base> &monsters, int srcX, int srcY, int dstX, int dstY, int ObjectSize, int MapX, int MapY, int DrawX, int DrawY, int SizeX, int SizeY)
    {
        // do not render off screen animations
        if ((srcX < MapX) || (srcX >= SizeX + MapX) || (srcY < MapY) || (srcY >= SizeY + MapY))
        {
            Move(Map, srcX, srcY, dstX, dstY);

            SDL_Delay(300);
        }

        auto Animate = [&](SDL_Surface *passable, SDL_Surface *asset)
        {
            auto DeltaX = (dstX - srcX);

            auto DeltaY = (dstY - srcY);

            for (auto i = 0; i < ObjectSize; i++)
            {
                RenderCombatScreen(Renderer, BattleScreen, -1, bg);

                Graphics::RenderImage(Renderer, passable, DrawX + (srcX - MapX) * ObjectSize, DrawY + (srcY - MapY) * ObjectSize);

                Graphics::RenderImage(Renderer, asset, DrawX + (srcX - MapX) * ObjectSize + DeltaX * i, DrawY + (srcY - MapY) * ObjectSize + DeltaY * i);

                SDL_RenderPresent(Renderer);

                SDL_Delay(3);
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

    bool AttackedUponMoving(TacticalMap::Base &Map, std::vector<Monster::Base> &monsters, Character::Base &character, int PlayerId, int &Damages)
    {
        auto WasAttacked = false;

        Damages = 0;

        for (auto i = 0; i < monsters.size(); i++)
        {
            Monster::Base &monster = monsters[i];

            if (monster.Endurance > 0 && Interface::IsAdjacent(Map, PlayerId, i) && monster.Awareness >= Engine::Score(character, Attributes::Type::Awareness))
            {
                WasAttacked = true;

                Damages -= 2;
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

    // fight encounter between PlayerId and MonsterId
    Combat::Result Fight(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, TacticalMap::Base &Map, Character::Base &Character, Monster::Base &Monster, int SizeX, int SizeY, int ObjectSize, int DrawX, int DrawY, bool Attacked)
    {
        auto Result = Combat::Result::NONE;
        auto MapSizeX = SizeX * ObjectSize;
        auto MapSizeY = SizeY * ObjectSize;
        auto WindowW = 4 * MapSizeX / 5;
        auto WindowH = 4 * MapSizeY / 5;
        auto WindowX = DrawX + (MapSizeX - WindowW) / 2;
        auto WindowY = DrawY + (MapSizeY - WindowH) / 2;
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

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<TextButton> &Controls = (!Attacked) ? FightControls1 : FightControls2;

        auto done = false;

        auto CurrentStage = Combat::Stage::START;

        auto FightingProwess = Engine::Score(Character, Attributes::Type::FightingProwess);
        auto Damage = Character.Damage;
        auto DamageModifier = Character.DamageModifier;
        auto Armour = Engine::Armour(Character);

        if (!Engine::HasWeapon(Character))
        {
            FightingProwess = std::max(0, FightingProwess - 2);

            DamageModifier -= -2;
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

            // Character Stats
            auto Endurance = Engine::Score(Character, Attributes::Type::Endurance);
            Graphics::PutText(Renderer, Character::Description[Character.Class], Fonts::Normal, 0, clrLB, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY);
            Graphics::PutText(Renderer, ("FP: " + std::to_string(FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + RowHeight);
            Graphics::PutText(Renderer, ("EN: " + std::to_string(Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 2 * RowHeight);
            Graphics::PutText(Renderer, ("DMG: " + std::to_string(Damage) + "D+" + std::to_string(DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 3 * RowHeight);
            Graphics::PutText(Renderer, ("ARM: " + std::to_string(Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 4 * RowHeight);

            // MonsterStats
            Graphics::PutText(Renderer, Monster.Name.c_str(), Fonts::Normal, 0, clrRD, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY);
            Graphics::PutText(Renderer, ("FP: " + std::to_string(Monster.FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + RowHeight);
            Graphics::PutText(Renderer, ("EN: " + std::to_string(Monster.Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 2 * RowHeight);
            Graphics::PutText(Renderer, ("DMG: " + std::to_string(Monster.Damage) + "D+" + std::to_string(Monster.DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 3 * RowHeight);
            Graphics::PutText(Renderer, ("ARM: " + std::to_string(Monster.Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 4 * RowHeight);

            if (CurrentStage == Combat::Stage::FIGHT && Result == Combat::Result::NONE)
            {
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
                    Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (ObjectSize + 2 * border_space), TextY + 6 * RowHeight, ObjectSize, ObjectSize);
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
                        Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (ObjectSize + 2 * border_space), TextY + 6 * RowHeight, ObjectSize, ObjectSize);
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
                        Graphics::StretchImage(Renderer, dice[Damages[i] - 1], TextButtonX + i * (ObjectSize + 2 * border_space), TextY + 6 * RowHeight, ObjectSize, ObjectSize);
                    }

                    Graphics::PutText(Renderer, ("Damage Dealt (-Armour): " + std::to_string(DamageSum)).c_str(), Fonts::Normal, 0, clrRD, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

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
        auto Hold = false;
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

            CurrentMode = Combat::Mode::NORMAL;

            ResetSelection();
        };

        auto FontSize = TTF_FontHeight(Fonts::Normal);

        auto TextX = DrawX;

        auto TextR = DrawX + (ObjectSize * SizeX + border_space);

        auto TextWidthR = SCREEN_WIDTH - TextR;

        auto TextY = DrawY - 2 * border_space - FontSize;

        auto TextWidth = Map.SizeX * ObjectSize;

        auto MapButtonSize = ObjectSize + 2 * border_space;
        auto MapSizeX = SizeX * ObjectSize;
        auto MapSizeY = SizeY * ObjectSize;
        auto MapButtonsX = DrawX - MapButtonSize;
        auto MapButtonsY = DrawY + border_space;
        auto MapButtonsGridSize = MapSizeY / 4;

        auto ActionsX = DrawX;
        auto ActionsY = DrawY + MapSizeY + 2 * border_space;
        auto ActionsGrid = MapButtonSize;

        auto RenderFlashMessage = [&]()
        {
            if (FlashMessage)
            {
                if ((SDL_GetTicks() - StartTicks) < Duration)
                {
                    auto FlashW = 3 * MapSizeX / 5;

                    auto FlashH = 2 * infoh;

                    Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, infoh * 2, DrawX + (MapSizeX - FlashW) / 2, DrawY + (MapSizeY - FlashH) / 2);
                }
                else
                {
                    FlashMessage = false;
                }
            }
        };

        if (Window && Renderer && Map.SizeX > 0 && Map.SizeY > 0)
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

                auto StartMap = 14;
                auto BottomMapX = StartMap + (SizeX * (SizeY - 1));
                auto MidMapY = StartMap + (SizeY / 2 * SizeX) - SizeX;

                Controls.push_back(Button(0, Assets::Get(Assets::Type::Up), 0, StartMap, 0, 1, MapButtonsX, MapButtonsY, MapY > 0 ? intLB : intGR, Control::Type::MAP_UP));
                Controls.push_back(Button(1, Assets::Get(Assets::Type::Left), 1, MidMapY, 0, 2, MapButtonsX, MapButtonsY + (MapButtonsGridSize + 2 * border_space), MapX > 0 ? intLB : intGR, Control::Type::MAP_LEFT));
                Controls.push_back(Button(2, Assets::Get(Assets::Type::Right), 2, MidMapY + SizeX, 1, 3, MapButtonsX, MapButtonsY + 2 * (MapButtonsGridSize + 2 * border_space), (MapX < Map.SizeX - SizeX) ? intLB : intGR, Control::Type::MAP_RIGHT));
                Controls.push_back(Button(3, Assets::Get(Assets::Type::Down), 3, BottomMapX, 2, 5, MapButtonsX, MapButtonsY + 3 * (MapButtonsGridSize + 2 * border_space), (MapY < Map.SizeY - SizeY) ? intLB : intGR, Control::Type::MAP_DOWN));
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
                            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Wall), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intYW, Control::Type::MAP_NONE));
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

                    if (CurrentMode == Combat::Mode::NORMAL && ControlType == Control::Type::PLAYER)
                    {
                        Graphics::PutText(Renderer, "View party member", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                        auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                        auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

                        Interface::CharacterSheet(Renderer, party, Fonts::Fixed, Map.ObjectID[SelectY][SelectX] - 1, TextWidthR, TextR, DrawY);
                    }
                    else if (CurrentMode == Combat::Mode::NORMAL && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "View opponent", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);

                        auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                        auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

                        Interface::MonsterData(Renderer, monsters, Fonts::Fixed, Map.ObjectID[SelectY][SelectX] - 1, TextWidthR, TextR, DrawY);
                    }
                    else if (CurrentMode == Combat::Mode::NORMAL)
                    {
                        Graphics::PutText(Renderer, (std::string("Combat Round: " + std::to_string(CombatRound + 1))).c_str(), Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::MOVE)
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
                    else if (CurrentMode == Combat::Mode::HEAL && ControlType == Control::Type::PLAYER)
                    {
                        Graphics::PutText(Renderer, "Heal target", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::ATTACK && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "Fight target", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::ATTACK)
                    {
                        Graphics::PutText(Renderer, "Select a nearby opponent to fight", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::SHOOT && ControlType == Control::Type::MONSTER)
                    {
                        Graphics::PutText(Renderer, "Shoot at target", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, FontSize, TextX, TextY);
                    }
                    else if (CurrentMode == Combat::Mode::MAGIC && ControlType == Control::Type::MONSTER)
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

                RenderFlashMessage();

                // get player input
                if (std::get<0>(Sequence[CurrentCombatant]) == TacticalMap::Object::Player)
                {
                    Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

                    auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                    if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
                    {
                        if (Controls[Current].Type == Control::Type::EXIT && !Hold)
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
                        else if (Controls[Current].Type == Control::Type::PLAYER && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::NORMAL)
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
                            else if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                DisplayMessage("You cannot fight that!", intRD);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::DESTINATION && !Hold)
                        {
                            auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                            auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

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
                                Character::Base &character = party.Members[PlayerId];

                                auto CurrentX = -1;

                                auto CurrentY = -1;

                                Interface::Find(Map, TacticalMap::Object::Player, PlayerId + 1, CurrentX, CurrentY);

                                if (Interface::Distance(CurrentX, CurrentY, SelectX, SelectY) > 1)
                                {
                                    CurrentPath[PlayerId] = AStar::FindPath(Map, CurrentX, CurrentY, SelectX, SelectY);

                                    CurrentMove[PlayerId] = 1;

                                    auto Damages = 0;

                                    // Get attacked by a nearby enemy that has a higher awareness
                                    auto WasAttacked = AttackedUponMoving(Map, monsters, character, PlayerId, Damages);

                                    if (CurrentPath[PlayerId].Points.size() > 2)
                                    {
                                        if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, party, monsters, CurrentX, CurrentY, CurrentPath[PlayerId].Points[1].X, CurrentPath[PlayerId].Points[1].Y, ObjectSize, MapX, MapY, DrawX, DrawY, SizeX, SizeY))
                                        {
                                            DisplayMessage("Path Blocked!", intRD);
                                        }
                                        else
                                        {
                                            if (WasAttacked)
                                            {
                                                RenderMessage(Renderer, Controls, intBK, SizeX, SizeY, DrawX, DrawY, ObjectSize, ("The " + std::string(Character::Description[character.Class]) + " was attacked!"), intRD);

                                                Engine::Gain(character, Attributes::Type::Endurance, Damages);

                                                if (!Engine::IsAlive(character))
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

                                    auto WasAttacked = AttackedUponMoving(Map, monsters, character, PlayerId, Damages);

                                    if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, party, monsters, CurrentX, CurrentY, SelectX, SelectY, ObjectSize, MapX, MapY, DrawX, DrawY, SizeX, SizeY))
                                    {
                                        DisplayMessage("Path Blocked!", intRD);
                                    }
                                    else
                                    {
                                        if (WasAttacked)
                                        {
                                            RenderMessage(Renderer, Controls, intBK, SizeX, SizeY, DrawX, DrawY, ObjectSize, ("The " + std::string(Character::Description[character.Class]) + " was attacked!"), intRD);

                                            Engine::Gain(character, Attributes::Type::Endurance, Damages);

                                            if (!Engine::IsAlive(character))
                                            {
                                                Remove(Map, SelectX, SelectY);
                                            }
                                        }

                                        CycleCombatants();
                                    }
                                }
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MOVE && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                CurrentMode = Combat::Mode::MOVE;
                            }
                            else if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                            else if (CurrentMode == Combat::Mode::MOVE)
                            {
                                if (CurrentPath[PlayerId].Points.size() > 1 && CurrentMove[PlayerId] >= 0 && CurrentMove[PlayerId] < CurrentPath[PlayerId].Points.size())
                                {
                                    Character::Base &character = party.Members[PlayerId];

                                    if (Engine::IsAlive(character))
                                    {
                                        auto CurrentX = -1;

                                        auto CurrentY = -1;

                                        Interface::Find(Map, TacticalMap::Object::Player, PlayerId + 1, CurrentX, CurrentY);

                                        auto Damages = 0;

                                        auto WasAttacked = AttackedUponMoving(Map, monsters, character, PlayerId, Damages);

                                        if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, party, monsters, CurrentX, CurrentY, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].X, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].Y, ObjectSize, MapX, MapY, DrawX, DrawY, SizeX, SizeY))
                                        {
                                            DisplayMessage("Path Blocked!", intRD);
                                        }
                                        else
                                        {
                                            // Get attacked by a nearby enemy that has a higher awareness
                                            if (WasAttacked)
                                            {
                                                RenderMessage(Renderer, Controls, intBK, SizeX, SizeY, DrawX, DrawY, ObjectSize, ("The " + std::string(Character::Description[character.Class]) + " was attacked!"), intRD);

                                                Engine::Gain(character, Attributes::Type::Endurance, Damages);

                                                if (!Engine::IsAlive(character))
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
                                CurrentMode = Combat::Mode::ATTACK;
                            }
                            else
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MONSTER && !Hold)
                        {
                            auto SelectX = MapX + (Controls[Current].X - DrawX) / ObjectSize;

                            auto SelectY = MapY + (Controls[Current].Y - DrawY) / ObjectSize;

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
                                    auto CurrentX = -1;

                                    auto CurrentY = -1;

                                    Interface::Find(Map, TacticalMap::Object::Player, PlayerId + 1, CurrentX, CurrentY);

                                    auto Result = Interface::Fight(Renderer, Controls, intBK, Map, character, monster, SizeX, SizeY, ObjectSize, DrawX, DrawY, false);

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
                            auto CurrentX = -1;

                            auto CurrentY = -1;

                            Interface::Find(Map, TacticalMap::Object::Player, PlayerId + 1, CurrentX, CurrentY);

                            // Do Attack
                            Interface::Fight(Renderer, Controls, intBK, Map, party.Members[PlayerId], monsters[MonsterId], SizeX, SizeY, ObjectSize, DrawX, DrawY, true);

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
                            // Close distance
                            auto LocationX = 0;

                            auto LocationY = 0;

                            Interface::Find(Map, TacticalMap::Object::Player, std::get<0>(NearestPlayer) + 1, LocationX, LocationY);

                            auto MonsterPath = AStar::FindPath(Map, MonsterX, MonsterY, LocationX, LocationY);

                            if (MonsterPath.Points.size() > 2)
                            {
                                Interface::AnimateMove(Renderer, Controls, intBK, Map, party, monsters, MonsterX, MonsterY, MonsterPath.Points[1].X, MonsterPath.Points[1].Y, ObjectSize, MapX, MapY, DrawX, DrawY, SizeX, SizeY);
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

        return Combat::Result::NONE;
    }
}
#endif