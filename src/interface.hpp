#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "graphics.hpp"

#include "book1.hpp"

namespace Interface
{
    // (player/enemy, id, awareness)
    typedef std::tuple<Map::Object, int, int> Combatants;

    // player id, distance, endurance
    typedef std::tuple<int, int, int> Targets;

    // character class to control type mapping
    std::map<Character::Class, Control::Type> PlayerControls = {
        {Character::Class::Warrior, Control::Type::WARRIOR},
        {Character::Class::Trickster, Control::Type::TRICKSTER},
        {Character::Class::Sage, Control::Type::SAGE},
        {Character::Class::Enchanter, Control::Type::ENCHANTER}};

    struct ScreenDimensions
    {
        int ObjectSize = 0;
        int IconSize = 0;
        int BoxWidth = 0;
        int BoxHeight = 0;

        int TextBoxWidth = 0;
        int TextBoxHeight = 0;
        int TextBoxX = 0;
        int TextBoxY = 0;
        int TextWidth = 0;
        int TextBounds = 0;

        int InfoBoxWidth = 0;
        int InfoBoxHeight = 0;
        int InfoBoxX = 0;
        int InfoBoxY = 0;
        int InfoWidth = 0;
    };

    bool ValidX(Map::Base &Map, int x)
    {
        return (x >= 0 && x < Map.Width);
    }

    bool ValidY(Map::Base &Map, int y)
    {
        return (y >= 0 && y < Map.Height);
    }

    int FindControl(std::vector<Button> &controls, Control::Type control)
    {
        auto found = -1;

        for (auto i = 0; i < controls.size(); i++)
        {
            if (controls[i].Type == control)
            {
                found = i;

                break;
            }
        }

        return found;
    }

    void RenderCombatScreen(SDL_Renderer *Renderer, std::vector<Button> &Controls, int Current, Uint32 bg)
    {
        Graphics::FillWindow(Renderer, bg);

        Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);
    }

    void RenderMessage(SDL_Renderer *Renderer, std::vector<Button> &Controls, Map::Base &Map, Uint32 bg, std::string Message, Uint32 FlashColor)
    {
        Uint32 Duration = 1500;

        auto MapSizeX = (Map.SizeX < 13 ? 13 : Map.SizeX) * Map.ObjectSize;

        auto MapSizeY = Map.SizeY * Map.ObjectSize;

        RenderCombatScreen(Renderer, Controls, -1, bg);

        auto FlashW = 4 * MapSizeX / 5;

        auto FlashH = 2 * infoh;

        Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, infoh * 2, Map.DrawX + (MapSizeX - FlashW) / 2, Map.DrawY + (MapSizeY - FlashH) / 2);

        if (FlashColor == intBK)
        {
            Graphics::DrawRect(Renderer, FlashW, infoh * 2, Map.DrawX + (MapSizeX - FlashW) / 2, Map.DrawY + (MapSizeY - FlashH) / 2, intWH);
        }

        SDL_RenderPresent(Renderer);

        SDL_Delay(Duration);
    }

    void Remove(Map::Base &Map, int srcX, int srcY)
    {
        if (Interface::ValidX(Map, srcX) && Interface::ValidY(Map, srcY))
        {
            if (Map.Tiles[srcY][srcX].IsPlayer())
            {
                Map.Tiles[srcY][srcX].Id = 0;
                Map.Tiles[srcY][srcX].Occupant = Map::Object::None;
            }
            else if (Map.Tiles[srcY][srcX].IsEnemy())
            {
                Map.Tiles[srcY][srcX].Id = 0;
                Map.Tiles[srcY][srcX].Occupant = Map::Object::None;
            }
        }
    }

    bool Move(Map::Base &Map, int srcX, int srcY, int dstX, int dstY)
    {
        auto result = false;

        if (Interface::ValidX(Map, srcX) && Interface::ValidY(Map, srcY) && Interface::ValidX(Map, dstX) && Interface::ValidY(Map, dstY))
        {
            if (Map.Tiles[srcY][srcX].IsPlayer() && (Map.Tiles[dstY][dstX].IsPassable || Map.Tiles[dstY][dstX].IsExit()) && Map.Tiles[dstY][dstX].Occupant == Map::Object::None)
            {
                Map.Tiles[dstY][dstX].Id = Map.Tiles[srcY][srcX].Id;
                Map.Tiles[dstY][dstX].Occupant = Map.Tiles[srcY][srcX].Occupant;

                Interface::Remove(Map, srcX, srcY);

                result = true;
            }
            else if (Map.Tiles[srcY][srcX].IsEnemy() && (Map.Tiles[dstY][dstX].IsPassable || Map.Tiles[dstY][dstX].IsPassableToEnemy) && Map.Tiles[dstY][dstX].Occupant == Map::Object::None)
            {
                Map.Tiles[dstY][dstX].Id = Map.Tiles[srcY][srcX].Id;
                Map.Tiles[dstY][dstX].Occupant = Map.Tiles[srcY][srcX].Occupant;

                Interface::Remove(Map, srcX, srcY);

                result = true;
            }
        }

        return result;
    }

    bool IsVisible(Map::Base &Map, int X, int Y)
    {
        auto ValidXY = Interface::ValidX(Map, X) && Interface::ValidY(Map, Y);

        return ValidXY && ((X >= Map.MapX) && (X < Map.SizeX + Map.MapX) && (Y >= Map.MapY) && (Y < Map.SizeY + Map.MapY));
    }

    void GenerateMapControls(Map::Base &Map, std::vector<Button> &Controls, Party::Base &Party, std::vector<Enemy::Base> &Enemies, int NumControls)
    {
        Controls.erase(Controls.begin() + NumControls, Controls.end());

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
                    if (CtrlX < 6)
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

                auto ObjectId = Map.Tiles[y][x].Id - 1;

                if (Map.Tiles[y][x].IsPlayer())
                {
                    Controls.push_back(Button(NumControls, Assets::Get(Party.Members[ObjectId].Asset), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intWH, Control::Type::PLAYER));
                }
                else if (Map.Tiles[y][x].IsEnemy())
                {
                    Controls.push_back(Button(NumControls, Enemies[ObjectId].Enthraled ? Assets::Get(Enemies[ObjectId].Asset, 0x66) : Assets::Get(Enemies[ObjectId].Asset), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intWH, Control::Type::ENEMY));
                }
                else if (Map.Tiles[y][x].IsExit())
                {
                    Controls.push_back(Button(NumControls, Assets::Get(Map.Tiles[y][x].Asset), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intGR, Control::Type::MAP_EXIT));
                }
                else if (Map.Tiles[y][x].IsPassable)
                {
                    Controls.push_back(Button(NumControls, Assets::Get(Map.Tiles[y][x].Asset), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intGR, Control::Type::DESTINATION));
                }
                else if (Map.Tiles[y][x].IsPassableToEnemy)
                {
                    Controls.push_back(Button(NumControls, Assets::Get(Map.Tiles[y][x].Asset, 0x66), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intGR, Control::Type::MAP_NONE));
                }
                else
                {
                    Controls.push_back(Button(NumControls, Assets::Get(Map.Tiles[y][x].Asset), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intGR, Control::Type::MAP_NONE));
                }

                NumControls++;
            }
        }
    }

    bool AnimateMove(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies, int srcX, int srcY, int dstX, int dstY)
    {
        // do not render off screen animations
        if (!Interface::IsVisible(Map, srcX, srcY))
        {
            SDL_Delay(300);

            return Interface::Move(Map, srcX, srcY, dstX, dstY);
        }

        auto Sign = [&](int Value)
        {
            return Value >= 0 ? 1 : -1;
        };

        auto Draw = [&](SDL_Surface *passable, SDL_Surface *asset, int X, int Y, int Delay)
        {
            RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::RenderImage(Renderer, passable, Map.DrawX + (srcX - Map.MapX) * Map.ObjectSize, Map.DrawY + (srcY - Map.MapY) * Map.ObjectSize);

            Graphics::RenderImage(Renderer, asset, X, Y);

            SDL_RenderPresent(Renderer);

            SDL_Delay(Delay);
        };

        auto Animate = [&](SDL_Surface *passable, SDL_Surface *asset)
        {
            auto DeltaX = (dstX - srcX);

            auto DeltaY = (dstY - srcY);

            // move along x
            for (auto i = 0; i < std::abs(DeltaX) * Map.ObjectSize; i += 5)
            {
                Draw(passable, asset, Map.DrawX + (srcX - Map.MapX) * Map.ObjectSize + Sign(DeltaX) * i, Map.DrawY + (srcY - Map.MapY) * Map.ObjectSize, 5);
            }

            // move along y, assumes movement along x was successful
            for (auto i = 0; i < std::abs(DeltaY) * Map.ObjectSize; i += 5)
            {
                Draw(passable, asset, Map.DrawX + (dstX - Map.MapX) * Map.ObjectSize, Map.DrawY + (srcY - Map.MapY) * Map.ObjectSize + Sign(DeltaY) * i, 5);
            }
        };

        auto result = false;

        if (Interface::ValidX(Map, srcX) && Interface::ValidY(Map, srcY) && Interface::ValidX(Map, dstX) && Interface::ValidY(Map, dstY))
        {
            if (Map.Tiles[srcY][srcX].IsPlayer() && (Map.Tiles[dstY][dstX].IsPassable || Map.Tiles[dstY][dstX].IsExit()) && !Map.Tiles[dstY][dstX].IsOccupied())
            {
                auto PlayerId = Map.Tiles[srcY][srcX].Id - 1;

                auto passable = Assets::Copy(Map.Tiles[srcY][srcX].Asset);

                auto asset = Assets::Copy(Party.Members[PlayerId].Asset);

                Animate(passable, asset);

                SDL_FreeSurface(passable);

                SDL_FreeSurface(asset);

                result = Interface::Move(Map, srcX, srcY, dstX, dstY);
            }
            else if (Map.Tiles[srcY][srcX].IsEnemy() && (Map.Tiles[dstY][dstX].IsPassable || Map.Tiles[dstY][dstX].IsPassableToEnemy) && !Map.Tiles[dstY][dstX].IsOccupied())
            {
                auto EnemyId = Map.Tiles[srcY][srcX].Id - 1;

                auto passable = Map.Tiles[srcY][srcX].IsPassableToEnemy ? Assets::Copy(Map.Tiles[srcY][srcX].Asset, 0x66) : Assets::Copy(Map.Tiles[srcY][srcX].Asset);

                auto asset = Enemies[EnemyId].Enthraled ? Assets::Copy(Enemies[EnemyId].Asset, 0x66) : Assets::Copy(Enemies[EnemyId].Asset);

                Animate(passable, asset);

                SDL_FreeSurface(passable);

                SDL_FreeSurface(asset);

                result = Interface::Move(Map, srcX, srcY, dstX, dstY);
            }
        }

        return result;
    }

    int Find(std::vector<Combatants> &Sequence, Map::Object object, int id)
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

    void Find(Map::Base &Map, Map::Object object, int id, int &LocationX, int &LocationY)
    {
        bool found = false;

        LocationX = -1;

        LocationY = -1;

        for (auto y = 0; y < Map.Height; y++)
        {
            for (auto x = 0; x < Map.Width; x++)
            {
                if (Map.Tiles[y][x].Occupant == object && Map.Tiles[y][x].Id == (id + 1))
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

    bool IsAdjacent(Map::Base &Map, int AttackerId, Map::Object AttackerType, int DefenderId, Map::Object DefenderType)
    {
        auto AttackerX = -1;

        auto AttackerY = -1;

        auto DefenderX = -1;

        auto DefenderY = -1;

        Interface::Find(Map, AttackerType, AttackerId, AttackerX, AttackerY);

        Interface::Find(Map, DefenderType, DefenderId, DefenderX, DefenderY);

        auto IsValidX = Interface::ValidX(Map, AttackerX) && Interface::ValidX(Map, DefenderX);

        auto IsValidY = Interface::ValidY(Map, AttackerY) && Interface::ValidX(Map, DefenderY);

        return IsValidX && IsValidY && Interface::Distance(AttackerX, AttackerY, DefenderX, DefenderY) <= 1;
    }

    bool IsAdjacent(Map::Base &Map, int PlayerId, int EnemyId)
    {
        return Interface::IsAdjacent(Map, PlayerId, Map::Object::Player, EnemyId, Map::Object::Enemy);
    }

    bool NearbyEnemies(Map::Base &Map, std::vector<Enemy::Base> &Enemies, int PlayerId, bool ShootMode)
    {
        auto result = false;

        for (auto i = 0; i < Enemies.size(); i++)
        {
            result |= (Engine::IsAlive(Enemies[i]) && Interface::IsAdjacent(Map, PlayerId, i) && ((ShootMode && !Enemies[i].Enthraled) || !ShootMode));
        }

        return result;
    }

    bool NearbyOpponents(Map::Base &Map, std::vector<Enemy::Base> &Enemies, int EnemyId, bool ShootMode)
    {
        auto result = false;

        for (auto i = 0; i < Enemies.size(); i++)
        {
            result |= (i != EnemyId && Engine::IsAlive(Enemies[i]) && Interface::IsAdjacent(Map, EnemyId, Map::Object::Enemy, i, Map::Object::Enemy) && ((ShootMode && !Enemies[i].Enthraled) || !ShootMode));
        }

        return result;
    }

    bool AttackedWhileMoving(Map::Base &Map, std::vector<Enemy::Base> &Enemies, Character::Base &character, int PlayerId, int &Damages)
    {
        auto WasAttacked = false;

        Damages = 0;

        for (auto i = 0; i < Enemies.size(); i++)
        {
            Enemy::Base &Enemy = Enemies[i];

            if (Engine::IsAlive(Enemy) && Interface::IsAdjacent(Map, PlayerId, i) && Enemy.Awareness >= Engine::Awareness(character))
            {
                WasAttacked = true;

                Damages -= 1;
            }
        }

        return WasAttacked;
    }

    void DrawPath(SDL_Renderer *Renderer, Map::Base &Map, AStar::Path &CurrentPath, int CurrentMove, Uint32 color, Uint8 alpha)
    {
        if (CurrentMove > 0 && CurrentMove < CurrentPath.Points.size())
        {
            auto TargetX = CurrentPath.Points.back().X - Map.MapX;

            auto TargetY = CurrentPath.Points.back().Y - Map.MapY;

            SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);

            auto Color = O(color, alpha);

            for (auto i = CurrentMove; i < CurrentPath.Points.size() - 1; i++)
            {
                auto X = CurrentPath.Points[i].X - Map.MapX;

                auto Y = CurrentPath.Points[i].Y - Map.MapY;

                if (Interface::IsVisible(Map, X + Map.MapX, Y + Map.MapY))
                {
                    Graphics::FillRect(Renderer, Map.ObjectSize, Map.ObjectSize, Map.DrawX + X * Map.ObjectSize, Map.DrawY + Y * Map.ObjectSize, Color);
                }
            }

            SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_NONE);

            if (Interface::IsVisible(Map, TargetX + Map.MapX, TargetY + Map.MapY))
            {
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

    std::vector<Targets> CycleTargets(Map::Base &Map, Party::Base &Party, int EnemyX, int EnemyY, bool ignore)
    {
        auto Distances = std::vector<Targets>();

        // cycle through the players
        for (auto i = 0; i < Party.Members.size(); i++)
        {
            if (Engine::IsAlive(Party.Members[i]) > 0 && !Party.Members[i].Escaped)
            {
                auto LocationX = 0;

                auto LocationY = 0;

                Interface::Find(Map, Map::Object::Player, i, LocationX, LocationY);

                auto TempPath = AStar::FindPath(Map, EnemyX, EnemyY, LocationX, LocationY, ignore);

                if (TempPath.Points.size() > 0)
                {
                    Distances.push_back({i, Interface::Distance(EnemyX, EnemyY, LocationX, LocationY), Engine::Endurance(Party.Members[i])});
                }
                else
                {
                    Distances.push_back({i, (Map.SizeX * Map.SizeY), Engine::Endurance(Party.Members[i])});
                }
            }
        }

        return Distances;
    }

    Interface::Targets SelectTarget(Map::Base &Map, Party::Base &Party, int EnemyId)
    {
        Interface::Targets NearestPlayer = {-1, -1, -1};

        // player id, distance, endurance
        std::vector<Targets> Distances = {};

        auto EnemyX = 0;

        auto EnemyY = 0;

        Interface::Find(Map, Map::Object::Enemy, EnemyId, EnemyX, EnemyY);

        Distances = Interface::CycleTargets(Map, Party, EnemyX, EnemyY, false);

        if (Distances.size() > 0)
        {
            // sort players based on distance and endurance
            SortTargets(Distances);

            NearestPlayer = Distances.front();
        }
        else
        {
            Distances = Interface::CycleTargets(Map, Party, EnemyX, EnemyY, true);

            if (Distances.size() > 0)
            {
                // sort players based on distance and endurance
                SortTargets(Distances);

                NearestPlayer = Distances.front();
            }
        }

        return NearestPlayer;
    }

    void CharacterSheet(SDL_Renderer *Renderer, Character::Base &Character, TTF_Font *Font, int TextWidth, int X, int Y, Uint32 Bg, bool FlipColors)
    {
        auto FontSize = TTF_FontHeight(Font);

        // add equipment bonuses
        auto EquipmentFPR = Engine::Equipment(Character, Attributes::Type::FightingProwess, false);
        auto EquipmentPSY = Engine::Equipment(Character, Attributes::Type::PsychicAbility, false);
        auto EquipmentAWR = Engine::Equipment(Character, Attributes::Type::Awareness, false);
        auto Weapons = Engine::Equipment(Character, Attributes::Type::FightingProwess, true);

        auto FightingProwess = Engine::FightingProwess(Character) + (EquipmentFPR.size() > 0 ? EquipmentFPR[0].Score : 0) + (Weapons.size() > 0 ? Weapons[0].Score : 0);
        auto PsychicAbility = Engine::PsychicAbility(Character) + (EquipmentPSY.size() > 0 ? EquipmentPSY[0].Score : 0);
        auto Awareness = Engine::Awareness(Character) + (EquipmentAWR.size() > 0 ? EquipmentAWR[0].Score : 0);
        auto Endurance = Engine::Endurance(Character);
        auto Armour = Engine::Armour(Character);
        auto Damage = Character.Damage;
        auto DamageModifier = Character.DamageModifier + (Weapons.size() > 0 ? Weapons[0].Damage : 0);

        Graphics::PutText(Renderer, Character::ClassName[Character.Class], Font, 0, FlipColors ? clrBK : clrGR, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y);
        Graphics::PutText(Renderer, std::string("RANK: " + std::to_string(Character.Rank)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + (FontSize + 2));
        Graphics::PutText(Renderer, std::string("FIGHTING PROWESS: " + std::to_string(FightingProwess)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 2 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("PSYCHIC ABILITY: " + std::to_string(PsychicAbility)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 3 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("AWARENESS: " + std::to_string(Awareness)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 4 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ENDURANCE: " + std::to_string(Endurance)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 5 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ARMOUR RATING: " + std::to_string(Armour)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 6 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("DAMAGE: " + std::to_string(Damage) + (DamageModifier >= 0 ? "D+" : "D") + std::to_string(DamageModifier)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 7 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("EXPERIENCE: " + std::to_string(Character.ExperiencePoints)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 8 * (FontSize + 2));
    }

    void CharacterSheet(SDL_Renderer *Renderer, Map::Base &Map, Party::Base &Party, TTF_Font *Font, int PlayerId)
    {
        Interface::CharacterSheet(Renderer, Party.Members[PlayerId], Font, Map.TextRightWidth, Map.TextRightX, Map.DrawY, intBK, false);

        auto FontSize = TTF_FontHeight(Font);

        auto TextOffset = 9;

        if (Engine::HaveMoney(Party.Members[PlayerId]))
        {
            Graphics::PutText(Renderer, ("GOLD: " + std::to_string(Engine::CountMoney(Party.Members[PlayerId]))).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + TextOffset * (FontSize + 2));

            TextOffset++;
        }

        if (Engine::HasArrows(Party.Members[PlayerId]))
        {
            Graphics::PutText(Renderer, ("ARROWS: " + std::to_string(Engine::CountArrows(Party.Members[PlayerId]))).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + TextOffset * (FontSize + 2));

            TextOffset++;
        }

        if (Party.Members[PlayerId].IsDefending)
        {
            Graphics::PutText(Renderer, "DEFENDING", Font, 0, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + TextOffset * (FontSize + 2));

            TextOffset++;
        }

        if (Party.Members[PlayerId].QuickThinking)
        {
            Graphics::PutText(Renderer, "QUICK THINKING", Font, 0, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + TextOffset * (FontSize + 2));

            TextOffset++;
        }

        if (Engine::HasStatus(Party.Members[PlayerId], Spell::Type::EyeOfTheTiger))
        {
            Graphics::PutText(Renderer, "EYE OF THE TIGER", Font, 0, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + TextOffset * (FontSize + 2));

            TextOffset++;
        }
    }

    void EnemyData(SDL_Renderer *Renderer, Map::Base &Map, std::vector<Enemy::Base> &Enemies, TTF_Font *Font, int EnemyId)
    {
        auto FontSize = TTF_FontHeight(Font);

        Graphics::PutText(Renderer, Enemies[EnemyId].Name.c_str(), Font, 0, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY);
        Graphics::PutText(Renderer, std::string("FIGHTING PROWESS: " + std::to_string(Enemies[EnemyId].FightingProwess)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + (FontSize + 2));
        Graphics::PutText(Renderer, std::string("PSYCHIC ABILITY: " + std::to_string(Enemies[EnemyId].PsychicAbility)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + 2 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("AWARENESS: " + std::to_string(Enemies[EnemyId].Awareness)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + 3 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ENDURANCE: " + std::to_string(Enemies[EnemyId].Endurance)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + 4 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ARMOUR RATING: " + std::to_string(Enemies[EnemyId].Armour)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + 5 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("DAMAGE: " + std::to_string(Enemies[EnemyId].Damage) + (Enemies[EnemyId].DamageModifier >= 0 ? "D+" : "D") + std::to_string(Enemies[EnemyId].DamageModifier)).c_str(), Font, 0, clrWH, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + 6 * (FontSize + 2));

        auto RowOffset = 7;

        if (Enemies[EnemyId].KnockedOff)
        {
            Graphics::PutText(Renderer, "KNOCKED OFF", Font, 0, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + RowOffset * (FontSize + 2));

            RowOffset++;
        }

        if (Engine::HasStatus(Enemies[EnemyId], Spell::Type::Nighthowl))
        {
            Graphics::PutText(Renderer, "NIGHTHOWL", Font, 0, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + RowOffset * (FontSize + 2));

            RowOffset++;
        }

        if (Enemies[EnemyId].Enthraled)
        {
            Graphics::PutText(Renderer, "ENTHRALED", Font, 0, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextRightWidth, FontSize, Map.TextRightX, Map.DrawY + RowOffset * (FontSize + 2));

            RowOffset++;
        }
    }

    void ShowCoordinates(SDL_Renderer *Renderer, Map::Base &Map, std::vector<Button> &Controls, std::vector<Combatants> &Sequence, int Current, int SelectedCombatant, TTF_Font *Font, int TextW, int TextX)
    {
        auto ControlType = Controls[Current].Type;

        auto FontSize = TTF_FontHeight(Font);

        if ((SelectedCombatant < 0 || SelectedCombatant >= Sequence.size()) && (ControlType == Control::Type::MAP_NONE || ControlType == Control::Type::DESTINATION || ControlType == Control::Type::MAP_EXIT))
        {
            auto SelectX = Map.MapX + (Controls[Current].X - Map.DrawX) / Map.ObjectSize;

            auto SelectY = Map.MapY + (Controls[Current].Y - Map.DrawY) / Map.ObjectSize;

            std::string Coordinates = "(" + std::to_string(SelectX) + ", " + std::to_string(SelectY) + ")";

            Graphics::PutText(Renderer, Coordinates.c_str(), Font, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, Map.DrawY);

            Graphics::PutText(Renderer, Map::Description[Map.Tiles[SelectY][SelectX].Type], Font, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, Map.DrawY + (FontSize + text_space));
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
                          if (std::get<0>(a) == Map::Object::Player && std::get<0>(b) == Map::Object::Enemy)
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

    void KnockedOffSequence(std::vector<Combatants> &Sequence, std::vector<Enemy::Base> &Enemies)
    {
        auto NewSequence = std::vector<Combatants>();
        auto KnockedOff = std::vector<Combatants>();

        for (auto i = 0; i < Sequence.size(); i++)
        {
            if (std::get<0>(Sequence[i]) == Map::Object::Enemy)
            {
                if (Enemies[std::get<1>(Sequence[i])].KnockedOff)
                {
                    KnockedOff.push_back(Sequence[i]);
                }
                else
                {
                    NewSequence.push_back(Sequence[i]);
                }
            }
            else
            {
                NewSequence.push_back(Sequence[i]);
            }
        }

        if (KnockedOff.size() > 0)
        {
            NewSequence.insert(NewSequence.end(), KnockedOff.begin(), KnockedOff.end());
        }

        Sequence = NewSequence;
    }

    void DealDamage(Map::Base &Map, std::vector<Enemy::Base> &Enemies, int EnemyId, int Damage, bool UseArmour)
    {
        auto EnemyX = -1;

        auto EnemyY = -1;

        Find(Map, Map::Object::Enemy, EnemyId, EnemyX, EnemyY);

        auto TotalDamage = std::max(0, Damage - (UseArmour ? Enemies[EnemyId].Armour : 0));

        Engine::Gain(Enemies[EnemyId], -TotalDamage);

        if (!Engine::IsAlive(Enemies[EnemyId]))
        {
            Interface::Remove(Map, EnemyX, EnemyY);
        }
    }

    Attributes::Result Test(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Character::Base &Character, Enemy::Base &Enemy, Attributes::Type Attribute, bool IsEnemy)
    {
        auto Result = Attributes::Result::NONE;
        auto MapSizeX = (Map.SizeX < 15 ? 15 : Map.SizeX) * Map.ObjectSize;
        auto MapSizeY = (Map.SizeY < 8 ? 8 : Map.SizeY) * Map.ObjectSize;
        auto WindowW = 4 * MapSizeX / 5;
        auto WindowH = 4 * MapSizeY / 5;
        auto WindowX = Map.DrawX + (MapSizeX - WindowW) / 2;
        auto WindowY = Map.DrawY + (MapSizeY - WindowH) / 2;
        auto ColumnWidth = WindowW - 4 * text_space;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;

        auto TextButtonX = WindowX + 2 * text_space;
        auto TextButtonY = (WindowY + WindowH) - (text_buttonh + 2 * text_space);
        auto TextWidth = WindowW - 3 * text_space;
        auto ResultsY = 12 * RowHeight + 4 * text_space;

        const char *ResistChoices[2] = {"TEST"};

        auto ResistControls = Graphics::CreateFixedTextButtons(ResistChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        ResistControls[0].Fg = clrWH;
        ResistControls[0].Highlight = intGR;
        ResistControls[0].Color = intBK;
        ResistControls[0].Type = Control::Type::TEST;

        const char *DoneChoices[1] = {"DONE"}; // end of psychic resistance check
        auto DoneControls = Graphics::CreateFixedTextButtons(DoneChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DoneControls[0].Fg = clrWH;
        DoneControls[0].Highlight = intGR;
        DoneControls[0].Color = intBK;
        DoneControls[0].Type = Control::Type::BACK;

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

        std::vector<TextButton> &Controls = ResistControls;

        auto done = false;

        auto CurrentStage = Attributes::Stage::START;

        Engine::Randomize();

        auto TestRolls = 2;

        std::vector<int> Rolls(TestRolls, 0);

        auto TestSum = 0;

        auto AttributeValue = 0;

        auto Equipment = Engine::Equipment(Character, Attribute, false);
        auto Weapons = Engine::Equipment(Character, Attribute, true);

        if (Attribute == Attributes::Type::FightingProwess)
        {
            AttributeValue = IsEnemy ? Enemy.FightingProwess : (Engine::FightingProwess(Character) + (Equipment.size() > 0 ? Equipment[0].Score : 0) + (Weapons.size() > 0 ? Weapons[0].Score : 0));
        }
        else if (Attribute == Attributes::Type::PsychicAbility)
        {
            AttributeValue = IsEnemy ? Enemy.PsychicAbility : (Engine::PsychicAbility(Character) + (Equipment.size() > 0 ? Equipment[0].Score : 0));
        }
        else if (Attribute == Attributes::Type::Awareness)
        {
            AttributeValue = IsEnemy ? Enemy.Awareness : (Engine::Awareness(Character) + (Equipment.size() > 0 ? Equipment[0].Score : 0));
        }

        while (!done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + " TEST").c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY);

            if (IsEnemy)
            {
                Graphics::PutText(Renderer, Enemy.Name.c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 2 * RowHeight);
            }
            else
            {
                Graphics::PutText(Renderer, Character::ClassName[Character.Class], Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 2 * RowHeight);
            }

            Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + ": " + std::to_string(AttributeValue)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 3 * RowHeight);

            if (CurrentStage == Attributes::Stage::TEST && Result == Attributes::Result::NONE)
            {
                TestSum = 0;

                for (auto i = 0; i < TestRolls; i++)
                {
                    Rolls[i] = Engine::Roll(1, 0);

                    TestSum += Rolls[i];
                }

                TestSum = std::max(0, TestSum);

                auto TestResult = AttributeValue >= TestSum;

                if (TestResult)
                {
                    Result = Attributes::Result::SUCCESS;
                }
                else
                {
                    Result = Attributes::Result::FAILURE;
                }

                CurrentStage = Attributes::Stage::END;
            }
            else if (CurrentStage == Attributes::Stage::END)
            {
                // show casting results
                for (auto i = 0; i < TestRolls; i++)
                {
                    Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + 6 * RowHeight, Map.ObjectSize, Map.ObjectSize);
                }

                Graphics::PutText(Renderer, ("Test Score: " + std::to_string(TestSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                if (Result == Attributes::Result::SUCCESS)
                {
                    Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + " test passed!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                }
                else
                {
                    Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + " test failed!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                }
            }

            Graphics::RenderTextButtons(Renderer, Controls, FONT_BOOKMAN, Current, 24, TTF_STYLE_NORMAL);

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::TEST && !Hold)
                {
                    CurrentStage = Attributes::Stage::TEST;

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

    bool BreakControl(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Enemy::Base &Enemy, int Rolls, int Threshold)
    {
        auto Result = false;
        auto MapSizeX = (Map.SizeX < 15 ? 15 : Map.SizeX) * Map.ObjectSize;
        auto MapSizeY = (Map.SizeY < 8 ? 8 : Map.SizeY) * Map.ObjectSize;
        auto WindowW = 4 * MapSizeX / 5;
        auto WindowH = 4 * MapSizeY / 5;
        auto WindowX = Map.DrawX + (MapSizeX - WindowW) / 2;
        auto WindowY = Map.DrawY + (MapSizeY - WindowH) / 2;
        auto ColumnWidth = WindowW - 4 * text_space;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;

        auto TextButtonX = WindowX + 2 * text_space;
        auto TextButtonY = (WindowY + WindowH) - (text_buttonh + 2 * text_space);
        auto TextWidth = WindowW - 3 * text_space;
        auto ResultsY = 12 * RowHeight + 4 * text_space;

        const char *ResistChoices[2] = {"BREAK"};

        auto ResistControls = Graphics::CreateFixedTextButtons(ResistChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        ResistControls[0].Fg = clrWH;
        ResistControls[0].Highlight = intGR;
        ResistControls[0].Color = intBK;
        ResistControls[0].Type = Control::Type::TEST;

        const char *DoneChoices[1] = {"DONE"}; // end of psychic resistance check
        auto DoneControls = Graphics::CreateFixedTextButtons(DoneChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DoneControls[0].Fg = clrWH;
        DoneControls[0].Highlight = intGR;
        DoneControls[0].Color = intBK;
        DoneControls[0].Type = Control::Type::BACK;

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

        std::vector<TextButton> &Controls = ResistControls;

        auto done = false;

        auto CurrentStage = Attributes::Stage::START;

        Engine::Randomize();

        std::vector<int> TestRolls(Rolls, 0);

        auto TestSum = 0;

        while (!done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, "Break Service Enthralment", Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY);

            Graphics::PutText(Renderer, Enemy.Name.c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + RowHeight);

            if (CurrentStage == Attributes::Stage::TEST && Result == false)
            {
                TestSum = 0;

                for (auto i = 0; i < Rolls; i++)
                {
                    TestRolls[i] = Engine::Roll(1, 0);

                    TestSum += TestRolls[i];
                }

                TestSum = std::max(0, TestSum);

                auto TestResult = TestSum >= Threshold;

                if (TestResult)
                {
                    Result = true;
                }

                CurrentStage = Attributes::Stage::END;
            }
            else if (CurrentStage == Attributes::Stage::END)
            {
                // show casting results
                for (auto i = 0; i < Rolls; i++)
                {
                    Graphics::StretchImage(Renderer, dice[TestRolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + 6 * RowHeight, Map.ObjectSize, Map.ObjectSize);
                }

                Graphics::PutText(Renderer, ("Score: " + std::to_string(TestSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                if (Result)
                {
                    Graphics::PutText(Renderer, "Servile Enthralment Broken!", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                }
                else
                {
                    Graphics::PutText(Renderer, "Servile Enthralment Remains!", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                }
            }

            Graphics::RenderTextButtons(Renderer, Controls, FONT_BOOKMAN, Current, 24, TTF_STYLE_NORMAL);

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::TEST && !Hold)
                {
                    CurrentStage = Attributes::Stage::TEST;

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

    void RenderChoiceCaption(SDL_Renderer *renderer, Button &control, std::string caption, SDL_Color color, Uint32 bg)
    {
        auto captionx = control.X - text_space;
        auto captiony = control.Y + control.H + text_space;

        if (caption.length() > 0)
        {
            auto captionw = 0;

            auto captionh = 0;

            TTF_SizeText(Fonts::Caption, caption.c_str(), &captionw, &captionh);

            Graphics::PutText(renderer, caption.c_str(), Fonts::Caption, border_pts, color, bg, TTF_STYLE_NORMAL, captionw + 2 * text_space, captionh, captionx, captiony);
        }
    }

    int Choose(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, std::vector<Assets::Type> Assets, std::vector<std::string> Captions, const char *Message)
    {
        auto Result = -1;

        auto MapSizeX = (Map.SizeX < 12 ? 12 : Map.SizeX) * Map.ObjectSize;
        auto MapSizeY = (Map.SizeY < 8 ? 8 : Map.SizeY) * Map.ObjectSize;
        auto WindowW = 10 * Map.ObjectSize;
        auto WindowH = 3 * Map.ObjectSize;
        auto WindowX = Map.DrawX + (MapSizeX - WindowW) / 2;
        auto WindowY = Map.DrawY + (MapSizeY - WindowH) / 2;
        auto WindowButtonX = WindowX + 4 * text_space;

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<Button> Controls = {};

        auto NumControls = 0;

        for (auto i = 0; i < Assets.size(); i++)
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets[NumControls]), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::CHOICE));

            NumControls++;
        }

        Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Back), NumControls > 0 ? NumControls - 1 : 0, NumControls, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::BACK));

        auto done = false;

        while (!done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, Message, Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, WindowW - 4 * text_space, TTF_FontHeight(Fonts::Normal), WindowButtonX - text_space, WindowY + text_space);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                if (Controls[Current].Type != Control::Type::BACK)
                {
                    Interface::RenderChoiceCaption(Renderer, Controls[Current], Captions[Current], clrWH, intBK);
                }
                else
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                }
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK)
                {
                    Result = -1;

                    done = true;
                }
                else
                {
                    Result = Current;

                    done = true;
                }
            }
        }

        return Result;
    }

    void TeleportToExits(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies, int StartMap)
    {
        if (Map.Exits.empty())
        {
            Interface::RenderMessage(Renderer, BattleScreen, Map, bg, "You cannot flee from this combat!", intBK);
        }
        else
        {
            for (auto i = 0; i < Party.Members.size(); i++)
            {
                if (Engine::IsAlive(Party.Members[i]) && !Party.Members[i].Escaped)
                {
                    auto teleported = false;

                    while (!teleported)
                    {
                        auto PlayerX = -1;
                        auto PlayerY = -1;

                        Interface::Find(Map, Map::Object::Player, i, PlayerX, PlayerY);

                        for (auto j = 0; j < Map.Exits.size(); j++)
                        {
                            if (Interface::AnimateMove(Renderer, BattleScreen, bg, Map, Party, Enemies, PlayerX, PlayerY, Map.Exits[j].first, Map.Exits[j].second))
                            {
                                teleported = true;

                                break;
                            }
                        }

                        if (!teleported)
                        {
                            std::vector<std::pair<int, int>> neighbors = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

                            for (auto j = 0; j < Map.Exits.size(); j++)
                            {
                                for (auto k = 0; k < neighbors.size(); k++)
                                {
                                    auto X = Map.Exits[j].first + neighbors[k].first;
                                    auto Y = Map.Exits[j].second + neighbors[k].second;

                                    if (Interface::ValidX(Map, X) && Interface::ValidY(Map, Y) && Map.Tiles[Y][X].Occupant == Map::Object::None)
                                    {
                                        teleported = Interface::AnimateMove(Renderer, BattleScreen, bg, Map, Party, Enemies, PlayerX, PlayerY, X, Y);
                                    }

                                    if (teleported)
                                    {
                                        break;
                                    }
                                }

                                if (teleported)
                                {
                                    break;
                                }
                            }
                        }
                    }
                }

                Interface::GenerateMapControls(Map, BattleScreen, Party, Enemies, StartMap);
            }

            Interface::RenderMessage(Renderer, BattleScreen, Map, bg, "Party teleported near the exits!", intGR);
        }
    }

    void ApplySpellEffects(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies, int PlayerId, int EnemyId, Spell::Type Spell, int CombatRound, int StartMap)
    {
        if (Spell == Spell::Type::VolcanoSpray || Spell == Spell::Type::SheetLightning)
        {
            auto DamageRolls = Spell == Spell::Type::VolcanoSpray ? 1 : 2;
            auto DamageModifier = Spell == Spell::Type::VolcanoSpray ? 1 : 2;
            auto Damage = Engine::Roll(DamageRolls, DamageModifier);

            Interface::RenderMessage(Renderer, BattleScreen, Map, bg, ("All enemies suffer " + std::to_string(Damage) + " damage!").c_str(), intGR);

            for (auto i = 0; i < Enemies.size(); i++)
            {
                if (Engine::IsAlive(Enemies[i]))
                {
                    Interface::DealDamage(Map, Enemies, i, Damage, true);

                    if (!Engine::IsAlive(Enemies[i]))
                    {
                        Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[i].Name + " killed!", intGR);
                    }
                }
            }
        }
        else if (Spell == Spell::Type::WhiteFire || Spell == Spell::Type::Swordthrust || Spell == Spell::Type::NemesisBolt)
        {
            auto DamageRolls = (Spell == Spell::Type::WhiteFire ? 2 : (Spell == Spell::Type::Swordthrust ? 3 : 7));
            auto DamageModifier = (Spell == Spell::Type::WhiteFire ? 2 : (Spell == Spell::Type::Swordthrust ? 3 : 7));
            auto Damage = Engine::Roll(DamageRolls, DamageModifier);

            Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[EnemyId].Name + " suffers " + std::to_string(Damage) + " damage!").c_str(), intGR);

            Interface::DealDamage(Map, Enemies, EnemyId, Damage, true);

            if (!Engine::IsAlive(Enemies[EnemyId]))
            {
                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[EnemyId].Name + " killed!", intGR);
            }
        }
        else if (Spell == Spell::Type::MistsOfDeath)
        {
            auto Damage = Engine::Roll(2, 0);

            for (auto i = 0; i < Enemies.size(); i++)
            {
                if (Engine::IsAlive(Enemies[i]))
                {
                    auto Result = Interface::Test(Renderer, BattleScreen, bg, Map, Party.Members[PlayerId], Enemies[i], Attributes::Type::PsychicAbility, true);

                    if (Result == Attributes::Result::FAILURE)
                    {
                        Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[i].Name + " suffers " + std::to_string(Damage) + " damage!").c_str(), intGR);

                        Interface::DealDamage(Map, Enemies, i, Damage, true);

                        if (!Engine::IsAlive(Enemies[i]))
                        {
                            Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[i].Name + " killed!", intGR);
                        }
                    }
                    else
                    {
                        Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[i].Name + " resists the Mists of Death spell!").c_str(), intBK);
                    }
                }
            }
        }
        else if (Spell == Spell::Type::TheVampireSpell)
        {
            if (Engine::IsAlive(Enemies[EnemyId]))
            {
                auto Result = Interface::Test(Renderer, BattleScreen, bg, Map, Party.Members[PlayerId], Enemies[EnemyId], Attributes::Type::PsychicAbility, true);

                if (Result == Attributes::Result::FAILURE)
                {
                    auto Damage = Engine::Roll(4, 0);

                    Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[EnemyId].Name + " suffers " + std::to_string(Damage) + " damage!").c_str(), intGR);

                    Interface::DealDamage(Map, Enemies, EnemyId, Damage, false);

                    if (!Engine::IsAlive(Enemies[EnemyId]))
                    {
                        Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[EnemyId].Name + " killed!", intGR);
                    }

                    auto Endurance = Damage / 2;

                    Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (std::string(Character::ClassName[Party.Members[PlayerId].Class]) + " gains " + std::to_string(Endurance) + " endurance!").c_str(), intGR);

                    Engine::Gain(Party.Members[PlayerId], Attributes::Type::Endurance, Endurance);
                }
                else
                {
                    Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[EnemyId].Name + " resists The Vampire Spell!").c_str(), intBK);
                }
            }
        }
        else if (Spell == Spell::Type::GhastlyTouch)
        {
            if (Engine::IsAlive(Enemies[EnemyId]))
            {
                auto Result = Interface::Test(Renderer, BattleScreen, bg, Map, Party.Members[PlayerId], Enemies[EnemyId], Attributes::Type::PsychicAbility, true);

                auto Damage = 0;

                if (Result == Attributes::Result::FAILURE)
                {
                    Damage = Engine::Roll(7, 0);
                }
                else
                {
                    Damage = Engine::Roll(2, 0);
                }

                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[EnemyId].Name + " suffers " + std::to_string(Damage) + " damage!").c_str(), intGR);

                Interface::DealDamage(Map, Enemies, EnemyId, Damage, false);

                if (!Engine::IsAlive(Enemies[EnemyId]))
                {
                    Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[EnemyId].Name + " killed!", intGR);
                }
            }
        }
        else if (Spell == Spell::Type::Nighthowl)
        {
            if (!Engine::HasStatus(Enemies[EnemyId], Spell::Type::Nighthowl))
            {
                Enemies[EnemyId].SpellStatus.push_back({Spell::Type::Nighthowl, CombatRound, 4, 0});

                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[EnemyId].Name + " afflicted with Nighthowl!", intGR);
            }
            else
            {
                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[EnemyId].Name + " already disadvantaged during combat!", intBK);
            }
        }
        else if (Spell == Spell::Type::EyeOfTheTiger)
        {
            auto Result = -1;

            while (Result == -1)
            {
                Result = Interface::Choose(Renderer, BattleScreen, bg, Map, {Assets::Type::EyeOfTheTigerIndividual, Assets::Type::EyeOfTheTigerParty}, {"YOU: +2 FP +2 DMG", "PARTY: +1 FP +1 DMG"}, "Select which effect to apply");
            }

            if (Result == 0)
            {
                if (Engine::HasStatus(Party.Members[PlayerId], Spell::Type::EyeOfTheTiger))
                {
                    Interface::RenderMessage(Renderer, BattleScreen, Map, bg, "Eye of the Tiger already in effect!", intBK);
                }
                else
                {
                    Interface::RenderMessage(Renderer, BattleScreen, Map, bg, "You gain +2 Fighting Prowess and +2 to Damage!", intGR);

                    Party.Members[PlayerId].SpellStatus.push_back({Spell::Type::EyeOfTheTiger, CombatRound, 4, 2});
                }
            }
            else if (Result == 0)
            {
                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, "Party gains +1 Fighting Prowess and +1 to Damage!", intGR);

                for (auto i = 0; i < Party.Members.size(); i++)
                {
                    if (Engine::IsAlive(Party.Members[i]))
                    {
                        if (!Engine::HasStatus(Party.Members[PlayerId], Spell::Type::EyeOfTheTiger))
                        {
                            Party.Members[PlayerId].SpellStatus.push_back({Spell::Type::EyeOfTheTiger, CombatRound, i == PlayerId ? 5 : 4, 1});
                        }
                    }
                }
            }
        }
        else if (Spell == Spell::Type::ServileEnthralment)
        {
            if (!Enemies[EnemyId].Enthraled)
            {
                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[EnemyId].Name + " is now under your control!", intGR);

                Enemies[EnemyId].Enthraled = true;
            }
            else
            {
                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[EnemyId].Name + " is already under your control!", intBK);
            }
        }
        else if (Spell == Spell::Type::ImmediateDeliverance)
        {
            Interface::TeleportToExits(Renderer, BattleScreen, bg, Map, Party, Enemies, StartMap);
        }
    }

    Spell::Result CastSpell(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Character::Base &Character, int SelectedSpell)
    {
        auto Result = Spell::Result::NONE;
        auto MapSizeX = (Map.SizeX < 15 ? 15 : Map.SizeX) * Map.ObjectSize;
        auto MapSizeY = (Map.SizeY < 8 ? 8 : Map.SizeY) * Map.ObjectSize;
        auto WindowW = 4 * MapSizeX / 5;
        auto WindowH = 4 * MapSizeY / 5;
        auto WindowX = Map.DrawX + (MapSizeX - WindowW) / 2;
        auto WindowY = Map.DrawY + (MapSizeY - WindowH) / 2;
        auto ColumnWidth = WindowW - 4 * text_space;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;

        auto TextButtonX = WindowX + 2 * text_space;
        auto TextButtonY = (WindowY + WindowH) - (text_buttonh + 2 * text_space);
        auto TextWidth = WindowW - 3 * text_space;
        auto ResultsY = 12 * RowHeight + 4 * text_space;

        const char *SpellChoices[2] = {"CAST", "CANCEL"};

        auto SpellControls = Graphics::CreateFixedTextButtons(SpellChoices, 2, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        SpellControls[0].Fg = clrWH;
        SpellControls[0].Highlight = intGR;
        SpellControls[0].Color = intBK;
        SpellControls[0].Type = Control::Type::CAST;
        SpellControls[1].Fg = clrWH;
        SpellControls[1].Highlight = intGR;
        SpellControls[1].Color = intBK;
        SpellControls[1].Type = Control::Type::BACK;

        const char *DoneChoices[1] = {"DONE"}; // end of casting
        auto DoneControls = Graphics::CreateFixedTextButtons(DoneChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DoneControls[0].Fg = clrWH;
        DoneControls[0].Highlight = intGR;
        DoneControls[0].Color = intBK;
        DoneControls[0].Type = Control::Type::BACK;

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

        std::vector<TextButton> &Controls = SpellControls;

        auto done = false;

        auto CurrentStage = Spell::Stage::START;

        if (Character.IsDefending)
        {
            Controls = DoneControls;

            CurrentStage = Spell::Stage::END;
        }

        Engine::Randomize();

        auto CastingRolls = 2;

        std::vector<int> Rolls(CastingRolls, 0);

        auto CastingSum = 0;

        Spell::Base &Spell = Character.Spells[SelectedSpell];

        auto Equipment = Engine::Equipment(Character, Attributes::Type::PsychicAbility, false);

        // compute effective Psychic Ability including bonus from equipment
        auto PsychicAbility = (Engine::PsychicAbility(Character) - Character.Spells.size()) + (Equipment.size() > 0 ? Equipment[0].Score : 0);

        while (!done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            // character stats
            Graphics::PutText(Renderer, Character::ClassName[Character.Class], Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY);
            Graphics::PutText(Renderer, ("PSYCHIC ABILITY: " + std::to_string(PsychicAbility) + " (-" + std::to_string(Character.Spells.size()) + ")").c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + RowHeight);
            Graphics::PutText(Renderer, ("SPELL: " + Spell.Name).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 2 * RowHeight);
            Graphics::PutText(Renderer, ("DIFFICULTY: " + std::to_string(CastingRolls) + "D" + (Spell.Difficulty < 0 ? "" : "+") + std::to_string(Spell.Difficulty)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 3 * RowHeight);

            if (CurrentStage == Spell::Stage::CAST && Result == Spell::Result::NONE)
            {
                CastingSum = 0;

                for (auto i = 0; i < CastingRolls; i++)
                {
                    Rolls[i] = Engine::Roll(1, 0);

                    CastingSum += Rolls[i];
                }

                CastingSum = std::max(0, CastingSum);

                CastingSum += Spell.Difficulty;

                auto CastingResult = Engine::PsychicAbility(Character) >= CastingSum;

                if (CastingResult)
                {
                    Result = Spell::Result::SUCCESS;
                }
                else
                {
                    Result = Spell::Result::FAILURE;
                }

                CurrentStage = Spell::Stage::END;
            }
            else if (CurrentStage == Spell::Stage::END)
            {
                // show casting results
                for (auto i = 0; i < CastingRolls; i++)
                {
                    Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + 6 * RowHeight, Map.ObjectSize, Map.ObjectSize);
                }

                Graphics::PutText(Renderer, ("Score: " + std::to_string(CastingSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                if (Result == Spell::Result::SUCCESS)
                {
                    Graphics::PutText(Renderer, (Spell.Name + " successfully cast!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                }
                else
                {
                    Graphics::PutText(Renderer, ("Casting " + Spell.Name + " fails!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                }
            }

            Graphics::RenderTextButtons(Renderer, Controls, FONT_BOOKMAN, Current, 24, TTF_STYLE_NORMAL);

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::CAST && !Hold)
                {
                    CurrentStage = Spell::Stage::CAST;

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

        if (Result == Spell::Result::FAILURE)
        {
            Spell.Difficulty--;
        }

        return Result;
    }

    // fight/shoot encounter between player and Enemy
    Combat::Result Fight(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Character::Base &Character, Enemy::Base &Enemy, Combat::FightMode FightMode, bool Attacked)
    {
        auto Result = Combat::Result::NONE;
        auto MapSizeX = (Map.SizeX < 15 ? 15 : Map.SizeX) * Map.ObjectSize;
        auto MapSizeY = (Map.SizeY < 8 ? 8 : Map.SizeY) * Map.ObjectSize;
        auto WindowW = 12 * Map.ObjectSize;
        auto WindowH = 7 * Map.ObjectSize;
        auto WindowX = Map.DrawX + (MapSizeX - WindowW) / 2;
        auto WindowY = Map.DrawY + (MapSizeY - WindowH) / 2;
        auto MidWindow = WindowX + (WindowW / 2) + text_space;
        auto ColumnWidth = WindowW / 2 - 4 * text_space;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;

        auto TextButtonX = WindowX + 2 * text_space;
        auto TextButtonY = (WindowY + WindowH) - (text_buttonh + 2 * text_space);
        auto TextWidth = WindowW - 3 * text_space;

        const char *FightChoices1[2] = {(FightMode == Combat::FightMode::FIGHT ? "FIGHT" : "SHOOT"), "CANCEL"}; // player attacks

        auto FightControls1 = Graphics::CreateFixedTextButtons(FightChoices1, 2, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        FightControls1[0].Fg = clrWH;
        FightControls1[0].Highlight = intGR;
        FightControls1[0].Color = intBK;
        FightControls1[0].Type = Control::Type::ATTACK;
        FightControls1[1].Fg = clrWH;
        FightControls1[1].Highlight = intGR;
        FightControls1[1].Color = intBK;
        FightControls1[1].Type = Control::Type::BACK;

        const char *FightChoices2[2] = {FightMode == Combat::FightMode::FIGHT ? "FIGHT" : "SHOOT"}; // Enemy attacks
        auto FightControls2 = Graphics::CreateFixedTextButtons(FightChoices2, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        FightControls2[0].Fg = clrWH;
        FightControls2[0].Highlight = intGR;
        FightControls2[0].Color = intBK;
        FightControls2[0].Type = Control::Type::ATTACK;

        const char *FightChoices3[3] = {"FIGHT", "QUARTERSTAFF", "CANCEL"}; // player attacks
        auto FightControls3 = Graphics::CreateFixedTextButtons(FightChoices3, 3, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        FightControls3[0].Fg = clrWH;
        FightControls3[0].Highlight = intGR;
        FightControls3[0].Color = intBK;
        FightControls3[0].Type = Control::Type::ATTACK;
        FightControls3[1].Fg = clrWH;
        FightControls3[1].Highlight = intGR;
        FightControls3[1].Color = intBK;
        FightControls3[1].Type = Control::Type::QUARTERSTAFF;
        FightControls3[2].Fg = clrWH;
        FightControls3[2].Highlight = intGR;
        FightControls3[2].Color = intBK;
        FightControls3[2].Type = Control::Type::BACK;

        const char *FightChoices4[2] = {"FIGHT", "QUARTERSTAFF"}; // player attacks
        auto FightControls4 = Graphics::CreateFixedTextButtons(FightChoices4, 2, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        FightControls4[0].Fg = clrWH;
        FightControls4[0].Highlight = intGR;
        FightControls4[0].Color = intBK;
        FightControls4[0].Type = Control::Type::ATTACK;
        FightControls4[1].Fg = clrWH;
        FightControls4[1].Highlight = intGR;
        FightControls4[1].Color = intBK;
        FightControls4[1].Type = Control::Type::QUARTERSTAFF;

        const char *DoneChoices[1] = {"DONE"}; // end of fighting
        auto DoneControls = Graphics::CreateFixedTextButtons(DoneChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DoneControls[0].Fg = clrWH;
        DoneControls[0].Highlight = intGR;
        DoneControls[0].Color = intBK;
        DoneControls[0].Type = Control::Type::BACK;

        const char *DamageChoices[1] = {"DAMAGE"}; // end of fighting
        auto DamageControls = Graphics::CreateFixedTextButtons(DamageChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DamageControls[0].Fg = clrWH;
        DamageControls[0].Highlight = intGR;
        DamageControls[0].Color = intBK;
        DamageControls[0].Type = Control::Type::DAMAGE;

        SDL_Surface *dice[6];

        dice[0] = Assets::Copy(Assets::Type::Dice1);
        dice[1] = Assets::Copy(Assets::Type::Dice2);
        dice[2] = Assets::Copy(Assets::Type::Dice3);
        dice[3] = Assets::Copy(Assets::Type::Dice4);
        dice[4] = Assets::Copy(Assets::Type::Dice5);
        dice[5] = Assets::Copy(Assets::Type::Dice6);

        auto swords = Assets::Copy(Assets::Type::Fight);

        auto shoot = Assets::Copy(Assets::Type::Shoot);

        auto HasQuarterstaff = Engine::HasAbility(Character, Abilities::Type::Quarterstaff) && Engine::HasWeapon(Character, Equipment::Weapon::Quarterstaff);

        auto Ambidextrous = !Attacked && Engine::HasAbility(Character, Abilities::Type::Ambidextrousness);

        std::vector<TextButton> &Controls = Attacked ? FightControls2 : (HasQuarterstaff && FightMode != Combat::FightMode::SHOOT ? FightControls3 : FightControls1);

        auto Rounds = Ambidextrous ? 2 : 1;

        Engine::Randomize();

        for (auto Round = 0; Round < Rounds; Round++)
        {
            auto Hold = false;
            auto Selected = false;
            auto ScrollUp = false;
            auto ScrollDown = false;
            auto Current = 0;
            auto Quarterstaff = false;

            auto CurrentStage = Combat::Stage::START;
            auto Weapons = Engine::Weapons(Character);
            auto Equipment = Engine::Equipment(Character, Attributes::Type::FightingProwess, false);
            auto FightingProwess = Engine::FightingProwess(Character) + (Weapons.size() > Round ? Weapons[Round].Score : 0) + (Equipment.size() > 0 ? Equipment[0].Score : 0);
            auto Damage = Character.Damage;
            auto DamageModifier = Character.DamageModifier + (Weapons.size() > Round ? Weapons[Round].Damage : 0) + (Equipment.size() > 0 ? Equipment[0].Damage : 0);
            auto Armour = Engine::Armour(Character);

            if (!Attacked && Engine::HasStatus(Character, Spell::Type::EyeOfTheTiger))
            {
                auto EyeResult = Engine::GetStatus(Character, Spell::Type::EyeOfTheTiger);

                if (EyeResult >= 0 && EyeResult < Character.SpellStatus.size())
                {
                    FightingProwess += std::get<3>(Character.SpellStatus[EyeResult]);

                    DamageModifier += std::get<3>(Character.SpellStatus[EyeResult]);
                }
            }

            if (Weapons.size() < Round && !Engine::HasAbility(Character, Abilities::Type::UnarmedMartialArts))
            {
                FightingProwess = std::max(0, FightingProwess - 2);

                DamageModifier -= 2;
            }

            if (Character.IsDefending && !Attacked)
            {
                Controls = DoneControls;

                CurrentStage = Combat::Stage::END;
            }

            auto FightRolls = 2;

            FightRolls += (Attacked && Character.IsDefending) ? 1 : 0;
            FightRolls += (Attacked && Engine::HasAbility(Character, Abilities::Type::Dodging)) ? 1 : 0;
            FightRolls += (Attacked && Engine::HasStatus(Enemy, Spell::Type::Nighthowl)) ? 1 : 0;

            auto DamageRolls = FightMode == Combat::FightMode::SHOOT ? 1 : (Attacked ? Enemy.Damage : Damage);

            std::vector<int> Rolls(FightRolls, 0);
            std::vector<int> Damages(DamageRolls, 0);

            auto DamageSum = 0;
            auto FightingSum = 0;
            bool CalculatedDamage = false;
            bool AssignedDamage = false;

            Result = Combat::Result::NONE;

            auto done = false;

            while (!done)
            {
                // render current combat screen
                Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

                Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

                Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

                // character stats
                auto Endurance = Engine::Endurance(Character);
                Graphics::PutText(Renderer, Character::ClassName[Character.Class], Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY);
                Graphics::PutText(Renderer, ("FPR: " + std::to_string(FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + RowHeight);
                Graphics::PutText(Renderer, ("END: " + std::to_string(Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 2 * RowHeight);
                Graphics::PutText(Renderer, ("DMG: " + (FightMode == Combat::FightMode::SHOOT ? "1D" : (std::to_string(Damage) + "D" + (DamageModifier < 0 ? "" : "+") + std::to_string(DamageModifier)))).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 3 * RowHeight);
                Graphics::PutText(Renderer, ("ARM: " + std::to_string(Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 4 * RowHeight);

                auto StatusOffset = 5;

                if (Attacked && Character.IsDefending)
                {
                    Graphics::PutText(Renderer, "DEFENDING", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + StatusOffset * RowHeight);

                    StatusOffset++;
                }

                if (Attacked && Engine::HasAbility(Character, Abilities::Type::Dodging))
                {
                    Graphics::PutText(Renderer, "DODGING", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + StatusOffset * RowHeight);

                    StatusOffset++;
                }

                auto RowOffset = 5;

                if (Attacked && Engine::HasStatus(Enemy, Spell::Type::Nighthowl))
                {
                    Graphics::PutText(Renderer, "NIGHTHOWL", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + RowOffset * RowHeight);

                    RowOffset++;
                }

                if (!Attacked && Engine::HasStatus(Character, Spell::Type::EyeOfTheTiger))
                {
                    Graphics::PutText(Renderer, "EYE OF THE TIGER", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + RowOffset * RowHeight);

                    RowOffset++;
                }

                auto ResultsY = (RowOffset + 6) * RowHeight + 4 * text_space;

                // Enemy stats
                Graphics::PutText(Renderer, Enemy.Name.c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY);
                Graphics::PutText(Renderer, ("FPR: " + std::to_string(Enemy.FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + RowHeight);
                Graphics::PutText(Renderer, ("END: " + std::to_string(Enemy.Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 2 * RowHeight);
                Graphics::PutText(Renderer, ("DMG: " + std::to_string(Enemy.Damage) + "D" + (Enemy.DamageModifier < 0 ? "" : "+") + std::to_string(Enemy.DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 3 * RowHeight);
                Graphics::PutText(Renderer, ("ARM: " + std::to_string(Enemy.Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 4 * RowHeight);

                if (FightMode == Combat::FightMode::FIGHT)
                {
                    Graphics::RenderImage(Renderer, swords, MidWindow - (3 * Map.ObjectSize / 2 + border_pts), WindowY + Map.ObjectSize);
                }
                else if (FightMode == Combat::FightMode::SHOOT)
                {
                    Graphics::RenderImage(Renderer, shoot, MidWindow - (3 * Map.ObjectSize / 2 + border_pts), WindowY + Map.ObjectSize);
                }

                if (CurrentStage == Combat::Stage::FIGHT && Result == Combat::Result::NONE)
                {
                    FightingSum = 0;

                    if (Quarterstaff && !Attacked)
                    {
                        FightRolls += 1;

                        Rolls.resize(FightRolls, 0);
                    }

                    for (auto i = 0; i < FightRolls; i++)
                    {
                        Rolls[i] = Engine::Roll(1, 0);

                        FightingSum += Rolls[i];
                    }

                    FightingSum = std::max(0, FightingSum);

                    auto FightResult = (!Attacked ? FightingProwess : Enemy.FightingProwess) >= FightingSum;

                    if (FightMode == Combat::FightMode::SHOOT && !Attacked)
                    {
                        Engine::ShootArrow(Character);
                    }

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
                        Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (RowOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                    }

                    Graphics::PutText(Renderer, ((FightMode != Combat::FightMode::SHOOT ? "Fight Score: " : "Shooting Score: ") + std::to_string(FightingSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                    if (Attacked)
                    {
                        Graphics::PutText(Renderer, (Enemy.Name + " hits the " + std::string(Character::ClassName[Character.Class]) + "!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                    }
                    else
                    {
                        Graphics::PutText(Renderer, ("The " + std::string(Character::ClassName[Character.Class]) + " hits " + Enemy.Name + "!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                    }

                    if (!CalculatedDamage)
                    {
                        DamageSum = 0;

                        if (Quarterstaff && !Attacked)
                        {
                            DamageRolls += 1;

                            Rolls.resize(DamageRolls, 0);
                        }

                        // compute damage
                        for (auto i = 0; i < DamageRolls; i++)
                        {
                            Damages[i] = Engine::Roll(1, 0);

                            DamageSum += Damages[i];
                        }

                        DamageSum += FightMode == Combat::FightMode::SHOOT ? 0 : (Attacked ? Enemy.DamageModifier : DamageModifier);

                        DamageSum -= Attacked ? Armour : Enemy.Armour;

                        DamageSum = std::max(0, DamageSum);

                        CalculatedDamage = true;

                        if (!Attacked && Quarterstaff && DamageSum > 0)
                        {
                            Enemy.KnockedOff = true;

                            Result = Combat::Result::KNOCKED_OFF;
                        }
                    }
                }
                else if (CurrentStage == Combat::Stage::END)
                {
                    if (Result == Combat::Result::UNSUCCESSFUL)
                    {
                        // show fight results
                        for (auto i = 0; i < FightRolls; i++)
                        {
                            Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (RowOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                        }

                        Graphics::PutText(Renderer, ("Fight Score: " + std::to_string(FightingSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                        if (Attacked)
                        {
                            Graphics::PutText(Renderer, (Enemy.Name + "'s attack was unsuccessful!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                        }
                        else
                        {
                            Graphics::PutText(Renderer, ("The " + std::string(Character::ClassName[Character.Class]) + "'s attack was unsuccessful!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                        }
                    }
                    else
                    {
                        // show damage results
                        for (auto i = 0; i < DamageRolls; i++)
                        {
                            Graphics::StretchImage(Renderer, dice[Damages[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (RowOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                        }

                        Graphics::PutText(Renderer, ("Damage Dealt (-Armour): " + std::to_string(DamageSum)).c_str(), Fonts::Normal, 0, Attacked ? clrGR : clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                        if (!AssignedDamage)
                        {
                            if (Attacked)
                            {
                                Engine::Gain(Character, Attributes::Type::Endurance, -DamageSum);
                            }
                            else
                            {
                                Engine::Gain(Enemy, -DamageSum);
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
                    else if (Controls[Current].Type == Control::Type::QUARTERSTAFF && !Hold)
                    {
                        Quarterstaff = true;

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

            if (Ambidextrous && Engine::IsAlive(Enemy) && Round < Rounds - 1 && Result != Combat::Result::NONE)
            {
                Controls = (HasQuarterstaff && FightMode != Combat::FightMode::SHOOT) ? FightControls4 : FightControls2;

                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (std::string("Strike ") + std::to_string(Round + 2)), intGR);
            }
            else
            {
                break;
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

        SDL_FreeSurface(shoot);

        shoot = NULL;

        return Result;
    }

    // fight encounter between Enemies
    Combat::Result Fight(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Enemy::Base &Enemy, Enemy::Base &Target, Combat::FightMode FightMode)
    {
        auto Result = Combat::Result::NONE;
        auto MapSizeX = (Map.SizeX < 15 ? 15 : Map.SizeX) * Map.ObjectSize;
        auto MapSizeY = (Map.SizeY < 8 ? 8 : Map.SizeY) * Map.ObjectSize;
        auto WindowW = 12 * Map.ObjectSize;
        auto WindowH = 7 * Map.ObjectSize;
        auto WindowX = Map.DrawX + (MapSizeX - WindowW) / 2;
        auto WindowY = Map.DrawY + (MapSizeY - WindowH) / 2;
        auto MidWindow = WindowX + (WindowW / 2) + text_space;
        auto ColumnWidth = WindowW / 2 - 4 * text_space;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;

        auto TextButtonX = WindowX + 2 * text_space;
        auto TextButtonY = (WindowY + WindowH) - (text_buttonh + 2 * text_space);
        auto TextWidth = WindowW - 3 * text_space;

        const char *FightChoices1[2] = {(FightMode != Combat::FightMode::SHOOT ? "FIGHT" : "SHOOT"), "CANCEL"}; // player attacks

        auto FightControls1 = Graphics::CreateFixedTextButtons(FightChoices1, 2, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        FightControls1[0].Fg = clrWH;
        FightControls1[0].Highlight = intGR;
        FightControls1[0].Color = intBK;
        FightControls1[0].Type = Control::Type::ATTACK;
        FightControls1[1].Fg = clrWH;
        FightControls1[1].Highlight = intGR;
        FightControls1[1].Color = intBK;
        FightControls1[1].Type = Control::Type::BACK;

        const char *FightChoices2[2] = {(FightMode != Combat::FightMode::SHOOT ? "FIGHT" : "SHOOT")}; // Enemy attacks
        auto FightControls2 = Graphics::CreateFixedTextButtons(FightChoices2, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        FightControls2[0].Fg = clrWH;
        FightControls2[0].Highlight = intGR;
        FightControls2[0].Color = intBK;
        FightControls2[0].Type = Control::Type::ATTACK;

        const char *DoneChoices[1] = {"DONE"}; // end of fighting
        auto DoneControls = Graphics::CreateFixedTextButtons(DoneChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DoneControls[0].Fg = clrWH;
        DoneControls[0].Highlight = intGR;
        DoneControls[0].Color = intBK;
        DoneControls[0].Type = Control::Type::BACK;

        const char *DamageChoices[1] = {"DAMAGE"}; // end of fighting
        auto DamageControls = Graphics::CreateFixedTextButtons(DamageChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DamageControls[0].Fg = clrWH;
        DamageControls[0].Highlight = intGR;
        DamageControls[0].Color = intBK;
        DamageControls[0].Type = Control::Type::DAMAGE;

        SDL_Surface *dice[6];

        dice[0] = Assets::Copy(Assets::Type::Dice1);
        dice[1] = Assets::Copy(Assets::Type::Dice2);
        dice[2] = Assets::Copy(Assets::Type::Dice3);
        dice[3] = Assets::Copy(Assets::Type::Dice4);
        dice[4] = Assets::Copy(Assets::Type::Dice5);
        dice[5] = Assets::Copy(Assets::Type::Dice6);

        auto swords = Assets::Copy(Assets::Type::Fight);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;
        auto QuarterStaff = false;

        std::vector<TextButton> &Controls = Enemy.Enthraled ? FightControls1 : FightControls2;

        auto done = false;

        auto CurrentStage = Combat::Stage::START;

        Engine::Randomize();

        auto FightRolls = 2;
        FightRolls += Engine::HasStatus(Enemy, Spell::Type::Nighthowl) ? 1 : 0;

        auto DamageRolls = FightMode != Combat::FightMode::SHOOT ? Enemy.Damage : 1;

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
            Graphics::PutText(Renderer, Enemy.Name.c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY);
            Graphics::PutText(Renderer, ("FPR: " + std::to_string(Enemy.FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + RowHeight);
            Graphics::PutText(Renderer, ("END: " + std::to_string(Enemy.Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 2 * RowHeight);
            Graphics::PutText(Renderer, ("DMG: " + std::to_string(Enemy.Damage) + "D" + (Enemy.DamageModifier < 0 ? "" : "+") + std::to_string(Enemy.DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 3 * RowHeight);
            Graphics::PutText(Renderer, ("ARM: " + std::to_string(Enemy.Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 4 * RowHeight);

            auto EnemyOffset = 5;

            if (Enemy.Enthraled)
            {
                Graphics::PutText(Renderer, "ENTHRALED", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + EnemyOffset * RowHeight);

                EnemyOffset++;
            }

            if (Engine::HasStatus(Enemy, Spell::Type::Nighthowl))
            {
                Graphics::PutText(Renderer, "NIGHTHOWL", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + EnemyOffset * RowHeight);

                EnemyOffset++;
            }

            auto ResultsY = (EnemyOffset + 6) * RowHeight + 4 * text_space;

            // Enemy stats
            Graphics::PutText(Renderer, Target.Name.c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY);
            Graphics::PutText(Renderer, ("FPR: " + std::to_string(Target.FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + RowHeight);
            Graphics::PutText(Renderer, ("END: " + std::to_string(Target.Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + 2 * RowHeight);
            Graphics::PutText(Renderer, ("DMG: " + (FightMode != Combat::FightMode::SHOOT ? std::to_string(Target.Damage) : "1") + "D" + (FightMode != Combat::FightMode::SHOOT ? ((Target.DamageModifier < 0 ? "" : "+") + std::to_string(Target.DamageModifier)) : "")).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + 3 * RowHeight);
            Graphics::PutText(Renderer, ("ARM: " + std::to_string(Target.Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + 4 * RowHeight);

            auto TargetOffset = 5;

            if (Target.Enthraled)
            {
                Graphics::PutText(Renderer, "ENTHRALED", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + TargetOffset * RowHeight);

                TargetOffset++;
            }

            if (Engine::HasStatus(Target, Spell::Type::Nighthowl))
            {
                Graphics::PutText(Renderer, "NIGHTHOWL", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + TargetOffset * RowHeight);

                TargetOffset++;
            }

            Graphics::RenderImage(Renderer, swords, MidWindow - (3 * Map.ObjectSize / 2 + border_pts), WindowY + Map.ObjectSize);

            if (CurrentStage == Combat::Stage::FIGHT && Result == Combat::Result::NONE)
            {
                FightingSum = 0;

                for (auto i = 0; i < FightRolls; i++)
                {
                    Rolls[i] = Engine::Roll(1, 0);

                    FightingSum += Rolls[i];
                }

                FightingSum = std::max(0, FightingSum);

                auto FightResult = Enemy.FightingProwess >= FightingSum;

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
                    Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (EnemyOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                }

                Graphics::PutText(Renderer, ((FightMode != Combat::FightMode::SHOOT ? "Fight Score: " : "Shooting Score: ") + std::to_string(FightingSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                Graphics::PutText(Renderer, (Enemy.Name + " hits " + Target.Name + "!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);

                if (!CalculatedDamage)
                {
                    DamageSum = 0;

                    // compute damage
                    for (auto i = 0; i < DamageRolls; i++)
                    {
                        Damages[i] = Engine::Roll(1, 0);

                        DamageSum += Damages[i];
                    }

                    DamageSum += Enemy.DamageModifier;

                    DamageSum -= Target.Armour;

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
                        Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (EnemyOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                    }

                    Graphics::PutText(Renderer, ((FightMode != Combat::FightMode::SHOOT ? "Fight Score: " : "Shooting Score: ") + std::to_string(FightingSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                    Graphics::PutText(Renderer, (Enemy.Name + "'s attack was unsuccessful!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY + RowHeight);
                }
                else
                {
                    // show damage results
                    for (auto i = 0; i < DamageRolls; i++)
                    {
                        Graphics::StretchImage(Renderer, dice[Damages[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (EnemyOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                    }

                    Graphics::PutText(Renderer, ("Damage Dealt (-Armour): " + std::to_string(DamageSum)).c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, ResultsY);

                    if (!AssignedDamage)
                    {
                        Engine::Gain(Target, -DamageSum);

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
                else if (Controls[Current].Type == Control::Type::QUARTERSTAFF && !Hold)
                {
                    QuarterStaff = true;

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

    int CallToMind(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Character::Base &Character, Control::Type Mode)
    {
        auto FlashMessage = false;

        auto FlashColor = intGR;

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

        auto Result = -1;

        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 12 * Map.ObjectSize;
        auto WindowH = 9 * Map.ObjectSize;
        auto WindowX = Map.DrawX + Map.ObjectSize;
        auto WindowY = (SCREEN_HEIGHT - WindowH) / 2;
        auto WindowButtonX = WindowX + 4 * text_space;
        auto WindowButtonY = WindowY + FontSize + 4 * text_space;
        auto WindowButtonGridX = Map.ObjectSize + 2 * text_space;
        auto WindowButtonGridY = Map.ObjectSize + 2 * text_space;

        auto RenderFlashMessage = [&]()
        {
            if (FlashMessage)
            {
                if ((SDL_GetTicks() - StartTicks) < Duration)
                {
                    auto FlashW = Map.ObjectSize * 8;

                    auto FlashH = Map.ObjectSize * 2;

                    Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, FlashH, WindowX + (WindowW - FlashW) / 2, WindowY + (WindowH - FlashH) / 2);

                    if (FlashColor == intBK)
                    {
                        Graphics::DrawRect(Renderer, FlashW, FlashH, WindowX + (WindowW - FlashW) / 2, WindowY + (WindowH - FlashH) / 2, intWH);
                    }
                }
                else
                {
                    FlashMessage = false;
                }
            }
        };

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<Button> Controls = {};

        Controls.push_back(Button(0, Assets::Get(Assets::Type::VolcanoSpray, Engine::WasCalledToMind(Character, Spell::Type::VolcanoSpray) ? 0xFF : 0x66), 0, 1, 0, 7, WindowButtonX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(1, Assets::Get(Assets::Type::Nighthowl, Engine::WasCalledToMind(Character, Spell::Type::Nighthowl) ? 0xFF : 0x66), 0, 2, 1, 8, WindowButtonX + WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(2, Assets::Get(Assets::Type::WhiteFire, Engine::WasCalledToMind(Character, Spell::Type::WhiteFire) ? 0xFF : 0x66), 1, 3, 2, 9, WindowButtonX + 2 * WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(3, Assets::Get(Assets::Type::Swordthrust, Engine::WasCalledToMind(Character, Spell::Type::Swordthrust) ? 0xFF : 0x66), 2, 4, 3, 10, WindowButtonX + 3 * WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(4, Assets::Get(Assets::Type::EyeOfTheTiger, Engine::WasCalledToMind(Character, Spell::Type::EyeOfTheTiger) ? 0xFF : 0x66), 3, 5, 4, 11, WindowButtonX + 4 * WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(5, Assets::Get(Assets::Type::ImmediateDeliverance, Engine::WasCalledToMind(Character, Spell::Type::ImmediateDeliverance) ? 0xFF : 0x66), 4, 6, 5, 12, WindowButtonX + 5 * WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(6, Assets::Get(Assets::Type::MistsOfDeath, Engine::WasCalledToMind(Character, Spell::Type::MistsOfDeath) ? 0xFF : 0x66), 5, 6, 6, 6, WindowButtonX + 6 * WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(7, Assets::Get(Assets::Type::TheVampireSpell, Engine::WasCalledToMind(Character, Spell::Type::TheVampireSpell) ? 0xFF : 0x66), 7, 8, 0, 7, WindowButtonX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::CAST));
        Controls.push_back(Button(8, Assets::Get(Assets::Type::SheetLightning, Engine::WasCalledToMind(Character, Spell::Type::SheetLightning) ? 0xFF : 0x66), 7, 9, 1, 8, WindowButtonX + WindowButtonGridX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::CAST));
        Controls.push_back(Button(9, Assets::Get(Assets::Type::GhastlyTouch, Engine::WasCalledToMind(Character, Spell::Type::GhastlyTouch) ? 0xFF : 0x66), 8, 10, 2, 9, WindowButtonX + 2 * WindowButtonGridX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::CAST));
        Controls.push_back(Button(10, Assets::Get(Assets::Type::NemesisBolt, Engine::WasCalledToMind(Character, Spell::Type::NemesisBolt) ? 0xFF : 0x66), 9, 11, 3, 10, WindowButtonX + 3 * WindowButtonGridX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::CAST));
        Controls.push_back(Button(11, Assets::Get(Assets::Type::ServileEnthralment, Engine::WasCalledToMind(Character, Spell::Type::ServileEnthralment) ? 0xFF : 0x66), 10, 12, 4, 11, WindowButtonX + 4 * WindowButtonGridX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::CAST));
        Controls.push_back(Button(12, Assets::Get(Assets::Type::Back), 11, 12, 5, 12, WindowButtonX + 5 * WindowButtonGridX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::BACK));

        auto done = false;

        while (!done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, (std::string("Select a spell to ") + std::string(Mode == Control::Type::CALL ? "call to mind" : "forget")).c_str(), Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, WindowW - 5 * text_space, TTF_FontHeight(Fonts::Normal), WindowButtonX - text_space, WindowY + text_space);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                // render spell details
                if (Current == Spell::All.size())
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                }
                else
                {
                    Spell::Base &Spell = Spell::All[Current];

                    Graphics::PutText(Renderer, ("(" + std::string(Spell::ClassDescription[Spell.Class]) + ") " + Spell.Name + " - Complexity Level: " + std::to_string(Spell.Complexity) + "\n\n" + Spell.Description).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, WindowW - 5 * text_space, WindowH - (WindowButtonY + 2 * WindowButtonGridY), WindowButtonX - text_space, WindowButtonY + 2 * WindowButtonGridY);
                }
            }

            RenderFlashMessage();

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK)
                {
                    done = true;
                }
                else if (Controls[Current].Type == Control::Type::CAST)
                {
                    if (Mode == Control::Type::CALL)
                    {
                        if (Engine::WasCalledToMind(Character, Spell::All[Current].Type))
                        {
                            DisplayMessage((Spell::All[Current].Name + " already called to mind!").c_str(), intBK);
                        }
                        else
                        {
                            Result = Current;

                            done = true;
                        }
                    }
                    else if (Mode == Control::Type::FORGET)
                    {
                        if (!Engine::WasCalledToMind(Character, Spell::All[Current].Type))
                        {
                            DisplayMessage((Spell::All[Current].Name + " not called to mind!").c_str(), intBK);
                        }
                        else
                        {
                            Result = Current;

                            done = true;
                        }
                    }
                }
            }
        }

        return Result;
    }

    int SelectSpell(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Character::Base &Character)
    {
        auto Result = -1;

        auto MapSizeX = (Map.SizeX < 12 ? 12 : Map.SizeX) * Map.ObjectSize;
        auto MapSizeY = (Map.SizeY < 8 ? 8 : Map.SizeY) * Map.ObjectSize;
        auto WindowW = 10 * Map.ObjectSize;
        auto WindowH = 3 * Map.ObjectSize;
        auto WindowX = Map.DrawX + (MapSizeX - WindowW) / 2;

        auto WindowY = Map.DrawY + (MapSizeY - WindowH) / 2;
        auto WindowButtonX = WindowX + 4 * text_space;

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<Button> Controls = {};

        auto NumControls = 0;

        for (auto i = 0; i < Character.Spells.size(); i++)
        {
            Controls.push_back(Button(Controls.size(), Assets::Get(Character.Spells[i].Asset), i > 0 ? i - 1 : 0, i < Character.Spells.size() - 1 ? i + 1 : i, i, i, WindowButtonX + i * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::CAST));

            NumControls++;
        }

        Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Back), NumControls > 0 ? NumControls - 1 : NumControls, NumControls, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::BACK));

        auto done = false;

        while (!done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, "Select a spell to cast", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, WindowW - 4 * text_space, TTF_FontHeight(Fonts::Normal), WindowButtonX - text_space, WindowY + text_space);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Character.Spells.size())
            {
                // render spell names
                auto captionx = Controls[Current].X - text_space;

                auto captiony = Controls[Current].Y + Controls[Current].H + text_space;

                std::string caption = "Cast " + Character.Spells[Current].Name + " (" + std::string(Spell::ClassDescription[Character.Spells[Current].Class]) + ")";

                auto captionw = 0;

                auto captionh = 0;

                TTF_SizeText(Fonts::Caption, caption.c_str(), &captionw, &captionh);

                Graphics::PutText(Renderer, caption.c_str(), Fonts::Caption, border_pts, clrWH, bg, TTF_STYLE_NORMAL, captionw + 2 * text_space, captionh, captionx, captiony);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK)
                {
                    done = true;
                }
                else if (Controls[Current].Type == Control::Type::CAST)
                {
                    Result = Current;

                    done = true;
                }
            }
        }

        return Result;
    }

    Abilities::Type Abilities(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Character::Base &Character)
    {
        auto Result = Abilities::Type::None;

        auto MapSizeX = (Map.SizeX < 12 ? 12 : Map.SizeX) * Map.ObjectSize;
        auto MapSizeY = (Map.SizeY < 8 ? 8 : Map.SizeY) * Map.ObjectSize;
        auto WindowW = 11 * Map.ObjectSize;
        auto WindowH = 3 * Map.ObjectSize;
        auto WindowX = Map.DrawX + (MapSizeX - WindowW) / 2;
        auto WindowY = Map.DrawY + (MapSizeY - WindowH) / 2;
        auto WindowButtonX = WindowX + 4 * text_space;

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<Button> Controls = {};
        std::vector<Abilities::Type> Abilities = {};

        auto NumControls = 0;

        if (Engine::HasAbility(Character, Abilities::Type::Ambidextrousness))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Ambidextrousness, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::AMBIDEXTROUSNESS));

            Abilities.push_back(Abilities::Type::Ambidextrousness);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::UnarmedMartialArts))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::UnarmedMartialArts, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::UNARMED_MARTIAL_ARTS));

            Abilities.push_back(Abilities::Type::UnarmedMartialArts);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Archery))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Archery, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::ARCHERY));

            Abilities.push_back(Abilities::Type::Archery);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Dodging))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Dodging, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::DODGING));

            Abilities.push_back(Abilities::Type::Dodging);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::QuickThinking))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::QuickThinking), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::QUICKTHINKING));

            Abilities.push_back(Abilities::Type::QuickThinking);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Quarterstaff))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Quarterstaff, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::QUARTERSTAFF));

            Abilities.push_back(Abilities::Type::Quarterstaff);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Healing))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Healing, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::HEAL));

            Abilities.push_back(Abilities::Type::Healing);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Exorcism))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Exorcism, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::EXORCISM));

            Abilities.push_back(Abilities::Type::Exorcism);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::ESP))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::ESP, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::ESP));

            Abilities.push_back(Abilities::Type::ESP);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::ParanormalSight))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::ParanormalSight, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::PARANORMAL_SIGHT));

            Abilities.push_back(Abilities::Type::ParanormalSight);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Levitation))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Levitation, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::LEVITATION));

            Abilities.push_back(Abilities::Type::Levitation);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::CallToMind))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::CallToMind), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::CALL));

            Abilities.push_back(Abilities::Type::CallToMind);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::CastSpell))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::CastSpell), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::CAST));

            Abilities.push_back(Abilities::Type::CastSpell);

            NumControls++;
        }

        Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Back), NumControls > 0 ? NumControls - 1 : 0, NumControls, NumControls, NumControls, WindowButtonX + NumControls * (Map.ObjectSize + 2 * text_space), WindowY + Map.ObjectSize, intWH, Control::Type::BACK));

        auto done = false;

        while (!done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, "Use Ability", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, WindowW - 4 * text_space, TTF_FontHeight(Fonts::Normal), WindowButtonX - text_space, WindowY + text_space);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK)
                {
                    done = true;
                }
                else if (Current >= 0 && Current < Abilities.size())
                {
                    Result = Abilities[Current];

                    done = true;
                }
            }
        }

        return Result;
    }

    void RenderMapInfo(SDL_Renderer *Renderer, Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies, std::vector<Button> &Controls, std::vector<Combatants> &Sequence, std::vector<AStar::Path> &CurrentPath, std::vector<int> &CurrentMove, Combat::Mode CurrentMode, int CombatRound, int Current, int CurrentCombatant, int SelectedCombatant, int SelectedSpell)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);

        auto MapSizeX = Map.SizeX * Map.ObjectSize;

        auto MapSizeY = Map.SizeY * Map.ObjectSize;

        auto IsPlayer = std::get<0>(Sequence[CurrentCombatant]) == Map::Object::Player;

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

            Interface::ShowCoordinates(Renderer, Map, Controls, Sequence, Current, SelectedCombatant, Fonts::Normal, Map.TextWidth, Map.TextRightX);

            if (CurrentMode == Combat::Mode::NORMAL)
            {
                if (ControlType == Control::Type::PLAYER)
                {
                    Graphics::PutText(Renderer, "View Party member", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);

                    Interface::CharacterSheet(Renderer, Map, Party, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                }
                else if (ControlType == Control::Type::ENEMY)
                {
                    Graphics::PutText(Renderer, "View opponent", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);

                    Interface::EnemyData(Renderer, Map, Enemies, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                }
                else
                {
                    Graphics::PutText(Renderer, ("Combat Round: " + std::to_string(CombatRound + 1)).c_str(), Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
                }
            }
            else if (CurrentMode == Combat::Mode::MOVE)
            {
                auto TargetX = -1;

                auto TargetY = -1;

                auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                if (IsPlayer)
                {
                    if (CurrentMove[PlayerId] > 0 && CurrentMove[PlayerId] < CurrentPath[PlayerId].Points.size())
                    {
                        TargetX = CurrentPath[PlayerId].Points.back().X - Map.MapX;

                        TargetY = CurrentPath[PlayerId].Points.back().Y - Map.MapY;

                        Graphics::PutText(Renderer, "Move to location or continue along current path", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);

                        Interface::DrawPath(Renderer, Map, CurrentPath[PlayerId], CurrentMove[PlayerId], intGR, 0x66);
                    }
                    else
                    {
                        Graphics::PutText(Renderer, "Move to location", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
                    }
                }
                else
                {
                    Graphics::PutText(Renderer, "Move to location", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
                }

                if ((TargetX != SelectX || TargetY != SelectY) && ((ControlType == Control::Type::DESTINATION) || (IsPlayer && ControlType == Control::Type::MAP_EXIT) || (!IsPlayer && ControlType == Control::Type::MAP_NONE && Map.Tiles[SelectY][SelectX].IsPassableToEnemy)))
                {
                    auto PlayerX = -1;

                    auto PlayerY = -1;

                    Interface::Find(Map, std::get<0>(Sequence[CurrentCombatant]), PlayerId, PlayerX, PlayerY);

                    if (Interface::ValidX(Map, PlayerX) && Interface::ValidY(Map, PlayerY) && Interface::Distance(PlayerX, PlayerY, SelectX, SelectY) > 1)
                    {
                        auto TempPath = AStar::FindPath(Map, PlayerX, PlayerY, SelectX, SelectY, IsPlayer ? false : true);

                        if (TempPath.Points.size() > 2)
                        {
                            Interface::DrawPath(Renderer, Map, TempPath, 1, intGR, 0x66);
                        }
                    }
                }
                else if (ControlType == Control::Type::PLAYER)
                {
                    Interface::CharacterSheet(Renderer, Map, Party, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                }
                else if (ControlType == Control::Type::ENEMY)
                {
                    Interface::EnemyData(Renderer, Map, Enemies, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                }
            }
            else if (CurrentMode == Combat::Mode::ATTACK)
            {
                if (ControlType == Control::Type::PLAYER || ControlType == Control::Type::ENEMY)
                {
                    Graphics::PutText(Renderer, "Fight target", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);

                    if (ControlType == Control::Type::PLAYER)
                    {
                        Interface::CharacterSheet(Renderer, Map, Party, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                    }
                    else if (ControlType == Control::Type::ENEMY)
                    {
                        Interface::EnemyData(Renderer, Map, Enemies, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                    }
                }
                else
                {
                    Graphics::PutText(Renderer, "Select a nearby opponent to fight", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
                }
            }
            else if (CurrentMode == Combat::Mode::SHOOT)
            {
                if (ControlType == Control::Type::PLAYER || ControlType == Control::Type::ENEMY)
                {
                    Graphics::PutText(Renderer, "Shoot at target", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);

                    if (ControlType == Control::Type::PLAYER)
                    {
                        Interface::CharacterSheet(Renderer, Map, Party, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                    }
                    else if (ControlType == Control::Type::ENEMY)
                    {
                        Interface::EnemyData(Renderer, Map, Enemies, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                    }
                }
                else
                {
                    Graphics::PutText(Renderer, "Shoot at target from range", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
                }
            }
            else if (CurrentMode == Combat::Mode::CAST && ControlType == Control::Type::ENEMY)
            {
                if (IsPlayer)
                {
                    auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                    std::string cast = "Cast " + Party.Members[PlayerId].Spells[SelectedSpell].Name + " on target";

                    Graphics::PutText(Renderer, cast.c_str(), Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);

                    Interface::EnemyData(Renderer, Map, Enemies, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                }
            }
            else if (CurrentMode == Combat::Mode::CAST && ControlType == Control::Type::PLAYER)
            {
                if (IsPlayer)
                {
                    auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                    std::string cast = "Cast " + Party.Members[PlayerId].Spells[SelectedSpell].Name + " on target";

                    Graphics::PutText(Renderer, cast.c_str(), Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);

                    Interface::CharacterSheet(Renderer, Map, Party, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                }
            }
            else if (CurrentMode == Combat::Mode::CAST)
            {
                if (IsPlayer)
                {
                    auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                    std::string cast = "Cast " + Party.Members[PlayerId].Spells[SelectedSpell].Name;

                    Graphics::PutText(Renderer, cast.c_str(), Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
                }
            }
        }
    }

    void RenderSelection(SDL_Renderer *Renderer, Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies, std::vector<Combatants> &Sequence, int SelectedCombatant)
    {
        if (SelectedCombatant >= 0 && SelectedCombatant < Sequence.size())
        {
            auto SelectedX = 0;

            auto SelectedY = 0;

            auto SelectedId = std::get<1>(Sequence[SelectedCombatant]);

            Interface::Find(Map, std::get<0>(Sequence[SelectedCombatant]), SelectedId, SelectedX, SelectedY);

            auto IsPlayer = std::get<0>(Sequence[SelectedCombatant]) == Map::Object::Player;

            auto IsEnemy = std::get<0>(Sequence[SelectedCombatant]) == Map::Object::Enemy;

            if ((SelectedX - Map.MapX) >= 0 && (SelectedX - Map.MapX) < Map.SizeX && (SelectedY - Map.MapY) >= 0 && (SelectedY - Map.MapY) < Map.SizeY)
            {
                Graphics::ThickRect(Renderer, Map.ObjectSize - 4 * border_pts, Map.ObjectSize - 4 * border_pts, Map.DrawX + (SelectedX - Map.MapX) * Map.ObjectSize + 2 * border_pts, Map.DrawY + (SelectedY - Map.MapY) * Map.ObjectSize + 2 * border_pts, intWH, border_pts);
            }

            // render statistics for currently selected / highlighted player or Enemy
            if (IsPlayer)
            {
                Interface::CharacterSheet(Renderer, Map, Party, Fonts::Fixed, SelectedId);
            }
            else if (IsEnemy)
            {
                Interface::EnemyData(Renderer, Map, Enemies, Fonts::Fixed, SelectedId);

                if (!Enemies[SelectedId].Enthraled)
                {
                    // Show potential target
                    auto NearestPlayer = Interface::SelectTarget(Map, Party, SelectedId);

                    auto PlayerId = std::get<0>(NearestPlayer);

                    if (PlayerId >= 0 && PlayerId < Party.Members.size())
                    {
                        auto EnemyX = -1;

                        auto EnemyY = -1;

                        auto LocationX = -1;

                        auto LocationY = -1;

                        Interface::Find(Map, Map::Object::Enemy, SelectedId, EnemyX, EnemyY);

                        Interface::Find(Map, Map::Object::Player, PlayerId, LocationX, LocationY);

                        if (Interface::ValidX(Map, EnemyX) && Interface::ValidY(Map, EnemyY) && Interface::ValidX(Map, LocationX) && Interface::ValidY(Map, LocationY))
                        {
                            auto EnemyPath = AStar::FindPath(Map, EnemyX, EnemyY, LocationX, LocationY, true);

                            if (EnemyPath.Points.size() > 2)
                            {
                                Interface::DrawPath(Renderer, Map, EnemyPath, 1, intGR, 0x66);
                            }
                        }
                    }
                }
            }
        }
    }

    Combat::Result CombatScreen(SDL_Window *Window, SDL_Renderer *Renderer, Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies)
    {
        auto FlashMessage = false;

        auto FlashColor = intGR;

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

        auto CurrentPath = std::vector<AStar::Path>(Party.Members.size());

        auto CurrentMove = std::vector<int>(Party.Members.size());

        // round sequence
        std::vector<Interface::Combatants> Sequence = {};

        // sort combatants based on awareness
        for (auto i = 0; i < Party.Members.size(); i++)
        {
            auto Awareness = Engine::Awareness(Party.Members[i]);

            Sequence.push_back({Map::Object::Player, i, Awareness});
        }

        for (auto i = 0; i < Enemies.size(); i++)
        {
            auto Awareness = Enemies[i].Awareness;

            Sequence.push_back({Map::Object::Enemy, i, Awareness});
        }

        SortCombatants(Sequence);

        auto CurrentMode = Combat::Mode::NORMAL;

        auto SelectedCombatant = -1;

        auto CurrentCombatant = 0;

        auto SelectedSpell = -1;

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
            return std::get<0>(Sequence[id]) == Map::Object::Player;
        };

        auto IsEnemy = [&](int id)
        {
            return std::get<0>(Sequence[id]) == Map::Object::Enemy;
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
                    if (Party.Members[GetId(i)].QuickThinking)
                    {
                        next = i;

                        break;
                    }
                }
            }

            return next;
        };

        auto Controls = std::vector<Button>();

        auto CycleCombatants = [&]()
        {
            if (IsPlayer(CurrentCombatant))
            {
                if (QuickThinkingRound && Party.Members[GetId(CurrentCombatant)].QuickThinking)
                {
                    Party.Members[GetId(CurrentCombatant)].QuickThinking = false;

                    Party.Members[GetId(CurrentCombatant)].UsedQuickThinking = true;
                }

                if (!QuickThinkingRound)
                {
                    Engine::UpdateSpellStatus(Party.Members[GetId(CurrentCombatant)], CombatRound);
                }
            }

            if (IsEnemy(CurrentCombatant))
            {
                if (Enemies[GetId(CurrentCombatant)].KnockedOff)
                {
                    Enemies[GetId(CurrentCombatant)].KnockedOff = false;
                }

                if (!QuickThinkingRound)
                {
                    Engine::UpdateSpellStatus(Enemies[GetId(CurrentCombatant)], CombatRound);
                }
            }

            auto active = false;

            while (!active)
            {
                if (!Engine::IsAlive(Party) || !Engine::IsAlive(Enemies) || Engine::Escaped(Party))
                {
                    break;
                }

                if (QuickThinkingRound)
                {
                    if (Engine::QuickThinking(Party))
                    {
                        CurrentCombatant = NextQuickThinker();
                    }
                    else
                    {
                        Interface::RenderMessage(Renderer, Controls, Map, intBK, "Quick thinking round ends!", intGR);

                        QuickThinkingRound = false;

                        CurrentCombatant = 0;

                        CombatRound++;

                        Current = 0;
                    }
                }
                else
                {
                    CurrentCombatant++;

                    if (CurrentCombatant >= Sequence.size())
                    {
                        if (Engine::KnockedOff(Enemies))
                        {
                            KnockedOffSequence(Sequence, Enemies);

                            CurrentCombatant = 0;
                        }
                        else if (Engine::QuickThinking(Party))
                        {
                            Interface::RenderMessage(Renderer, Controls, Map, intBK, "Quick thinking round begins!", intGR);

                            QuickThinkingRound = true;

                            CurrentCombatant = NextQuickThinker();

                            Current = 0;
                        }
                        else
                        {
                            SortCombatants(Sequence);

                            QuickThinkingRound = false;

                            CurrentCombatant = 0;

                            CombatRound++;
                        }
                    }
                }

                if (IsPlayer(CurrentCombatant))
                {
                    auto character = Party.Members[GetId(CurrentCombatant)];

                    active = Engine::IsAlive(character) && !character.Escaped;

                    if (Party.Members[GetId(CurrentCombatant)].IsDefending)
                    {
                        Party.Members[GetId(CurrentCombatant)].IsDefending = false;
                    }
                }
                else if (IsEnemy(CurrentCombatant))
                {
                    active = Engine::IsAlive(Enemies[GetId(CurrentCombatant)]);
                }
            }

            CurrentMode = Combat::Mode::NORMAL;

            ResetSelection();
        };

        auto FontSize = TTF_FontHeight(Fonts::Normal);

        Map.TextX = Map.DrawX;

        Map.TextY = Map.DrawY - 2 * text_space - FontSize;

        Map.TextWidth = (Map.Width < 13 ? 13 : Map.Width) * Map.ObjectSize;

        Map.TextRightX = Map.DrawX + (Map.ObjectSize * Map.SizeX + text_space);

        Map.TextRightWidth = SCREEN_WIDTH - Map.TextRightX;

        auto MapButtonSize = Map.ObjectSize + 2 * text_space;
        auto MapSizeX = Map.SizeX * Map.ObjectSize;
        auto MapSizeY = Map.SizeY * Map.ObjectSize;
        auto MapButtonsX = Map.DrawX - MapButtonSize;
        auto MapButtonsY = Map.DrawY + text_space;
        auto MapButtonsGridSize = MapSizeY / 4;

        auto ActionsX = Map.DrawX;
        auto ActionsY = Map.DrawY + MapSizeY + 2 * text_space;
        auto ActionsGrid = MapButtonSize;

        auto RenderFlashMessage = [&]()
        {
            if (FlashMessage)
            {
                if ((SDL_GetTicks() - StartTicks) < Duration)
                {
                    auto FlashW = 4 * (Map.SizeX < 13 ? 13 * Map.ObjectSize : MapSizeX) / 5;

                    auto FlashH = 2 * infoh;

                    Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, infoh * 2, Map.DrawX + ((Map.SizeX < 13 ? 13 * Map.ObjectSize : MapSizeX) - FlashW) / 2, Map.DrawY + (MapSizeY - FlashH) / 2);

                    if (FlashColor == intBK)
                    {
                        Graphics::DrawRect(Renderer, FlashW, infoh * 2, Map.DrawX + ((Map.SizeX < 13 ? 13 * Map.ObjectSize : MapSizeX) - FlashW) / 2, Map.DrawY + (MapSizeY - FlashH) / 2, intWH);
                    }
                }
                else
                {
                    FlashMessage = false;
                }
            }
        };

        Engine::ClearDefendingStatus(Party);

        Engine::ResetSpellDifficulty(Party);

        auto StartMap = 11;
        auto BottomMapX = StartMap + (Map.SizeX * (Map.SizeY - 1));
        auto MidMapY = StartMap + (Map.SizeY / 2 * Map.SizeX) - Map.SizeX;

        Controls.push_back(Button(0, Assets::Get(Assets::Type::Up), 0, StartMap, 0, 1, MapButtonsX, MapButtonsY, Map.MapY > 0 ? intWH : intGR, Control::Type::MAP_UP));
        Controls.push_back(Button(1, Assets::Get(Assets::Type::Left), 1, MidMapY, 0, 2, MapButtonsX, MapButtonsY + (MapButtonsGridSize + 2 * text_space), Map.MapX > 0 ? intWH : intGR, Control::Type::MAP_LEFT));
        Controls.push_back(Button(2, Assets::Get(Assets::Type::Right), 2, MidMapY + Map.SizeX, 1, 3, MapButtonsX, MapButtonsY + 2 * (MapButtonsGridSize + 2 * text_space), (Map.MapX < Map.Width - Map.SizeX) ? intWH : intGR, Control::Type::MAP_RIGHT));
        Controls.push_back(Button(3, Assets::Get(Assets::Type::Down), 3, BottomMapX, 2, 5, MapButtonsX, MapButtonsY + 3 * (MapButtonsGridSize + 2 * text_space), (Map.MapY < Map.Height - Map.SizeY) ? intWH : intGR, Control::Type::MAP_DOWN));
        Controls.push_back(Button(4, Assets::Get(Assets::Type::Exit), StartMap - 1, 4, StartMap - 1, 4, lastx, buttony, intWH, Control::Type::EXIT));
        Controls.push_back(Button(5, Assets::Get(Assets::Type::Move), 4, 6, BottomMapX, 5, ActionsX, ActionsY, intWH, Control::Type::MOVE));
        Controls.push_back(Button(6, Assets::Get(Assets::Type::Fight), 5, 7, Map.SizeX > 1 ? BottomMapX + 1 : 6, 6, ActionsX + ActionsGrid, ActionsY, intWH, Control::Type::ATTACK));
        Controls.push_back(Button(7, Assets::Get(Assets::Type::Defend), 6, 8, Map.SizeX > 2 ? BottomMapX + 2 : 7, 7, ActionsX + 2 * ActionsGrid, ActionsY, intWH, Control::Type::DEFEND));
        Controls.push_back(Button(8, Assets::Get(Assets::Type::Shoot), 7, 9, Map.SizeX > 3 ? BottomMapX + 3 : 8, 8, ActionsX + 3 * ActionsGrid, ActionsY, intWH, Control::Type::SHOOT));
        Controls.push_back(Button(9, Assets::Get(Assets::Type::Ability), 8, 10, Map.SizeX > 4 ? BottomMapX + 4 : 9, 9, ActionsX + 4 * ActionsGrid, ActionsY, intWH, Control::Type::ABILITY));
        Controls.push_back(Button(10, Assets::Get(Assets::Type::Flee), 10, 4, Map.SizeX > 5 ? BottomMapX + 5 : 10, 4, ActionsX + 5 * ActionsGrid, ActionsY, intWH, Control::Type::FLEE));

        // generate controls within the map window
        Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

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

                // select which object to blink (player/Enemy)
                auto BlinkX = -1;

                auto BlinkY = -1;

                if (IsPlayer(CurrentCombatant))
                {
                    auto PlayerX = -1;

                    auto PlayerY = -1;

                    Interface::Find(Map, Map::Object::Player, GetId(CurrentCombatant), PlayerX, PlayerY);

                    if (Interface::IsVisible(Map, PlayerX, PlayerY))
                    {
                        BlinkX = Map.DrawX + (PlayerX - Map.MapX) * Map.ObjectSize;

                        BlinkY = Map.DrawY + (PlayerY - Map.MapY) * Map.ObjectSize;

                        if (SDL_GetTicks() - BlinkStart > 100)
                        {
                            Blink = !Blink;

                            BlinkStart = SDL_GetTicks();
                        }
                    }
                }
                else if (IsEnemy(CurrentCombatant))
                {
                    if (Enemies[GetId(CurrentCombatant)].Enthraled)
                    {
                        auto EnemyX = -1;

                        auto EnemyY = -1;

                        Interface::Find(Map, Map::Object::Enemy, GetId(CurrentCombatant), EnemyX, EnemyY);

                        if (Interface::IsVisible(Map, EnemyX, EnemyY))
                        {
                            BlinkX = Map.DrawX + (EnemyX - Map.MapX) * Map.ObjectSize;

                            BlinkY = Map.DrawY + (EnemyY - Map.MapY) * Map.ObjectSize;

                            if (SDL_GetTicks() - BlinkStart > 100)
                            {
                                Blink = !Blink;

                                BlinkStart = SDL_GetTicks();
                            }
                        }
                    }
                }

                Interface::RenderCombatScreen(Renderer, Controls, Current, intBK);

                if (Current >= 0 && Current < Controls.size())
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                }

                // blink current Party member
                if (SelectedCombatant != CurrentCombatant && Blink && BlinkX >= 0 && BlinkY >= 0 && (Current == -1 || (Current >= 0 && Current < Controls.size() && (BlinkX != Controls[Current].X || BlinkY != Controls[Current].Y))))
                {
                    Graphics::ThickRect(Renderer, Map.ObjectSize - 4 * border_pts, Map.ObjectSize - 4 * border_pts, BlinkX + 2 * border_pts, BlinkY + 2 * border_pts, intGR, border_pts);
                }

                Interface::RenderSelection(Renderer, Map, Party, Enemies, Sequence, SelectedCombatant);

                Interface::RenderMapInfo(Renderer, Map, Party, Enemies, Controls, Sequence, CurrentPath, CurrentMove, CurrentMode, CombatRound, Current, CurrentCombatant, SelectedCombatant, SelectedSpell);

                RenderFlashMessage();

                auto Enthraled = false;

                if (IsEnemy(CurrentCombatant))
                {
                    Enthraled = Enemies[GetId(CurrentCombatant)].Enthraled;
                }

                // get player input
                if ((IsPlayer(CurrentCombatant) && !Party.Members[GetId(CurrentCombatant)].IsDefending))
                {
                    Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

                    auto PlayerId = GetId(CurrentCombatant);

                    Character::Base &Character = Party.Members[PlayerId];

                    auto CurrentX = -1;

                    auto CurrentY = -1;

                    Interface::Find(Map, Map::Object::Player, PlayerId, CurrentX, CurrentY);

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

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_DOWN || ScrollDown)
                        {
                            if (Map.MapY < Map.Height - Map.SizeY)
                            {
                                Map.MapY++;

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_LEFT)
                        {
                            if (Map.MapX > 0)
                            {
                                Map.MapX--;

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_RIGHT)
                        {
                            if (Map.MapX < Map.Width - Map.SizeX)
                            {
                                Map.MapX++;

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::DEFEND && !Hold)
                        {
                            Engine::ResetSpellDifficulty(Character);

                            Character.IsDefending = true;

                            CycleCombatants();

                            Selected = false;

                            Current = -1;
                        }
                        else if (Controls[Current].Type == Control::Type::FLEE && !Hold)
                        {
                            if (Map.Exits.size() > 0)
                            {
                                if (Map.Tiles[CurrentY][CurrentX].IsExit())
                                {
                                    Interface::RenderMessage(Renderer, Controls, Map, intBK, ("The " + std::string(Character::ClassName[Character.Class]) + " escapes!"), intGR);

                                    Interface::Remove(Map, CurrentX, CurrentY);

                                    Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

                                    Character.Escaped = true;

                                    CycleCombatants();
                                }
                                else
                                {
                                    DisplayMessage("You must be standing on an exit point to flee!", intBK);
                                }
                            }
                            else
                            {
                                DisplayMessage("Defeat all opponents to escape the area!", intBK);
                            }

                            Selected = false;

                            Current = -1;
                        }
                        else if (Controls[Current].Type == Control::Type::PLAYER && !Hold)
                        {
                            SelectedSpell = -1;

                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                auto result = Interface::Find(Sequence, Map::Object::Player, Map.Tiles[SelectY][SelectX].Id - 1);

                                if (result != SelectedCombatant)
                                {
                                    SelectedCombatant = result;
                                }
                                else
                                {
                                    ResetSelection();
                                }
                            }
                            else if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                DisplayMessage("You cannot fight that!", intBK);
                            }
                            else if (CurrentMode == Combat::Mode::SHOOT)
                            {
                                DisplayMessage("You cannot shoot that!", intBK);
                            }
                            else
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                        }
                        else if ((Controls[Current].Type == Control::Type::DESTINATION || Controls[Current].Type == Control::Type::MAP_EXIT) && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                            else if (CurrentMode == Combat::Mode::MOVE)
                            {
                                SelectedSpell = -1;

                                if (Interface::Distance(CurrentX, CurrentY, SelectX, SelectY) > 1)
                                {
                                    CurrentPath[PlayerId] = AStar::FindPath(Map, CurrentX, CurrentY, SelectX, SelectY);

                                    CurrentMove[PlayerId] = 1;

                                    auto Damages = 0;

                                    // get attacked by a nearby enemy that has a higher awareness
                                    auto WasAttacked = AttackedWhileMoving(Map, Enemies, Character, PlayerId, Damages);

                                    if (CurrentPath[PlayerId].Points.size() > 2)
                                    {
                                        if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, Party, Enemies, CurrentX, CurrentY, CurrentPath[PlayerId].Points[1].X, CurrentPath[PlayerId].Points[1].Y))
                                        {
                                            DisplayMessage("Path blocked!", intBK);
                                        }
                                        else
                                        {
                                            Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

                                            if (WasAttacked)
                                            {
                                                Interface::RenderMessage(Renderer, Controls, Map, intBK, ("The " + std::string(Character::ClassName[Character.Class]) + " was attacked!"), intBK);

                                                Engine::Gain(Character, Attributes::Type::Endurance, Damages);

                                                if (!Engine::IsAlive(Character))
                                                {
                                                    Interface::Remove(Map, CurrentPath[PlayerId].Points[1].X, CurrentPath[PlayerId].Points[1].Y);

                                                    Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                                }
                                            }

                                            CurrentMove[PlayerId] = 2;

                                            CycleCombatants();
                                        }
                                    }
                                    else
                                    {
                                        DisplayMessage("Path blocked!", intBK);
                                    }
                                }
                                else
                                {
                                    CurrentMove[PlayerId] = 0;

                                    CurrentPath[PlayerId].Points.clear();

                                    auto Damages = 0;

                                    auto WasAttacked = AttackedWhileMoving(Map, Enemies, Character, PlayerId, Damages);

                                    if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, Party, Enemies, CurrentX, CurrentY, SelectX, SelectY))
                                    {
                                        DisplayMessage("Path Blocked!", intBK);
                                    }
                                    else
                                    {
                                        Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

                                        if (WasAttacked)
                                        {
                                            Interface::RenderMessage(Renderer, Controls, Map, intBK, ("The " + std::string(Character::ClassName[Character.Class]) + " was attacked!"), intBK);

                                            Engine::Gain(Character, Attributes::Type::Endurance, Damages);

                                            if (!Engine::IsAlive(Character))
                                            {
                                                Interface::Remove(Map, SelectX, SelectY);

                                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
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
                            else if (CurrentMode == Combat::Mode::ATTACK)
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

                                        auto WasAttacked = AttackedWhileMoving(Map, Enemies, Character, PlayerId, Damages);

                                        if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, Party, Enemies, CurrentX, CurrentY, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].X, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].Y))
                                        {
                                            DisplayMessage("Path blocked!", intBK);
                                        }
                                        else
                                        {
                                            Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

                                            // get attacked by a nearby enemy that has a higher awareness
                                            if (WasAttacked)
                                            {
                                                Interface::RenderMessage(Renderer, Controls, Map, intBK, ("The " + std::string(Character::ClassName[Character.Class]) + " was attacked!"), intBK);

                                                Engine::Gain(Character, Attributes::Type::Endurance, Damages);

                                                if (!Engine::IsAlive(Character))
                                                {
                                                    Interface::Remove(Map, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].X, CurrentPath[PlayerId].Points[CurrentMove[PlayerId]].Y);

                                                    Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
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
                            SelectedSpell = -1;

                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                if (Character.IsDefending)
                                {
                                    DisplayMessage("You cannot attack at this time.", intBK);

                                    if (QuickThinkingRound && Character.QuickThinking)
                                    {
                                        CycleCombatants();
                                    }
                                }
                                else if (Interface::NearbyEnemies(Map, Enemies, PlayerId, false))
                                {
                                    CurrentMode = Combat::Mode::ATTACK;
                                }
                                else
                                {
                                    DisplayMessage("There are no opponents nearby!", intBK);
                                }
                            }
                            else
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::SHOOT && !Hold)
                        {
                            SelectedSpell = -1;

                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                if (Character.IsDefending)
                                {
                                    DisplayMessage("You cannot shoot at this time!", intBK);

                                    if (QuickThinkingRound && Character.QuickThinking)
                                    {
                                        CycleCombatants();
                                    }
                                }
                                else if (Engine::CanShoot(Character))
                                {
                                    if (!Interface::NearbyEnemies(Map, Enemies, PlayerId, true))
                                    {
                                        if (Engine::HasBow(Character) && Engine::HasArrows(Character))
                                        {
                                            CurrentMode = Combat::Mode::SHOOT;
                                        }
                                        else if (Engine::HasBow(Character))
                                        {
                                            DisplayMessage("You do not have any arrows left!", intBK);
                                        }
                                        else
                                        {
                                            DisplayMessage("You do not have a bow!", intBK);
                                        }
                                    }
                                    else
                                    {
                                        DisplayMessage("You cannot shoot at this time!", intBK);
                                    }
                                }
                                else
                                {
                                    DisplayMessage("You do not have the archery skill!", intBK);
                                }
                            }
                            else
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::ABILITY && !Hold)
                        {
                            SelectedSpell = -1;

                            auto Result = Interface::Abilities(Renderer, Controls, intBK, Map, Character);

                            if (Result == Abilities::Type::Ambidextrousness || Result == Abilities::Type::UnarmedMartialArts || Result == Abilities::Type::Dodging || Result == Abilities::Type::Quarterstaff)
                            {
                                DisplayMessage("This ability is always in effect!", intGR);
                            }
                            else if (Result == Abilities::Type::Archery)
                            {
                                DisplayMessage("Use the shoot action to attack from range", intGR);
                            }
                            else if (Result == Abilities::Type::Healing || Result == Abilities::Type::Exorcism || Result == Abilities::Type::ESP || Result == Abilities::Type::ParanormalSight || Result == Abilities::Type::Levitation)
                            {
                                DisplayMessage("You cannot use this ability while in combat", intBK);
                            }
                            else if (Result == Abilities::Type::QuickThinking)
                            {
                                if (Character.QuickThinking)
                                {
                                    DisplayMessage("Quick thinking already activated!", intBK);
                                }
                                else if (!Character.UsedQuickThinking)
                                {
                                    DisplayMessage("Quick thinking activated!", intGR);

                                    Character.QuickThinking = true;
                                }
                                else
                                {
                                    DisplayMessage("Quick thinking can only be used once per combat!", intBK);
                                }
                            }
                            else if (Result == Abilities::Type::CastSpell)
                            {
                                if (Character.Spells.size() == 0)
                                {
                                    DisplayMessage("You have not called to mind any spells!", intBK);
                                }
                                else
                                {
                                    SelectedSpell = Interface::SelectSpell(Renderer, Controls, intBK, Map, Character);

                                    // cast spell
                                    if (SelectedSpell >= 0 && SelectedSpell < Character.Spells.size())
                                    {
                                        Spell::Base &Spell = Character.Spells[SelectedSpell];

                                        if (Spell.RequiresTarget)
                                        {
                                            CurrentMode = Combat::Mode::CAST;

                                            if (Spell.Type == Spell::Type::GhastlyTouch)
                                            {
                                                if (!NearbyEnemies(Map, Enemies, PlayerId, false))
                                                {
                                                    DisplayMessage("There are no enemies nearby!", intBK);

                                                    CurrentMode = Combat::Mode::NORMAL;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            auto Proceed = true;

                                            if (Spell.Type == Spell::Type::ImmediateDeliverance)
                                            {
                                                if (Map.Exits.size() == 0)
                                                {
                                                    DisplayMessage("You cannot flee from this combat!", intBK);

                                                    Proceed = false;
                                                }
                                            }

                                            // attempt to cast spell
                                            auto Result = Proceed ? Interface::CastSpell(Renderer, Controls, intBK, Map, Character, SelectedSpell) : Spell::Result::NONE;

                                            if (Result != Spell::Result::NONE)
                                            {
                                                if (Result == Spell::Result::SUCCESS)
                                                {
                                                    Interface::ApplySpellEffects(Renderer, Controls, intBK, Map, Party, Enemies, PlayerId, -1, Character.Spells[SelectedSpell].Type, CombatRound, StartMap);

                                                    Character.Spells.erase(Character.Spells.begin() + SelectedSpell);

                                                    SelectedSpell = -1;

                                                    Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                                }

                                                CycleCombatants();
                                            }
                                            else
                                            {
                                                CurrentMode = Combat::Mode::NORMAL;
                                            }

                                            SelectedSpell = -1;
                                        }
                                    }
                                    else
                                    {
                                        DisplayMessage("Spellcasting canceled!", intGR);
                                    }
                                }
                            }
                            else if (Result == Abilities::Type::CallToMind)
                            {
                                auto CalledToMind = Interface::CallToMind(Renderer, Controls, intBK, Map, Character, Control::Type::CALL);

                                if (CalledToMind >= 0 && CalledToMind < Spell::All.size())
                                {
                                    Character.Spells.push_back(Spell::All[CalledToMind]);

                                    auto Forget = -1;

                                    while (Character.Spells.size() > 4)
                                    {
                                        Forget = Interface::CallToMind(Renderer, Controls, intBK, Map, Character, Control::Type::FORGET);

                                        if (Forget >= 0 && Forget < Spell::All.size())
                                        {
                                            auto Result = Engine::Find(Character, Spell::All[Forget].Type);

                                            if (Result >= 0 && Result < Character.Spells.size())
                                            {
                                                Character.Spells.erase(Character.Spells.begin() + Result);
                                            }
                                        }
                                    }

                                    if (Forget != CalledToMind)
                                    {
                                        Interface::RenderMessage(Renderer, Controls, Map, intBK, Spell::All[CalledToMind].Name + " called to mind!", intGR);

                                        CycleCombatants();
                                    }
                                }
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::ENEMY && !Hold)
                        {
                            auto TargetId = Map.Tiles[SelectY][SelectX].Id - 1;

                            Enemy::Base &Target = Enemies[TargetId];

                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                SelectedSpell = -1;

                                auto result = Interface::Find(Sequence, Map::Object::Enemy, TargetId);

                                if (result != SelectedCombatant)
                                {
                                    SelectedCombatant = result;
                                }
                                else
                                {
                                    ResetSelection();
                                }
                            }
                            else if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                SelectedSpell = -1;

                                if (Interface::IsAdjacent(Map, PlayerId, TargetId) && Engine::IsAlive(Target))
                                {
                                    auto Result = Interface::Fight(Renderer, Controls, intBK, Map, Character, Target, Combat::FightMode::FIGHT, false);

                                    if (!Engine::IsAlive(Target))
                                    {
                                        Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " killed!", intGR);

                                        Interface::Remove(Map, SelectX, SelectY);

                                        Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                    }

                                    if (Result != Combat::Result::NONE)
                                    {
                                        Engine::ResetSpellDifficulty(Character);

                                        if (Result == Combat::Result::KNOCKED_OFF)
                                        {
                                            if (Engine::IsAlive(Target))
                                            {
                                                Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " knocked off!", intGR);
                                            }
                                        }

                                        CycleCombatants();
                                    }
                                    else
                                    {
                                        DisplayMessage("Fight canceled", intGR);

                                        CurrentMode = Combat::Mode::NORMAL;
                                    }

                                    Selected = false;

                                    Current = -1;
                                }
                                else if (Engine::IsAlive(Target))
                                {
                                    DisplayMessage("You can only attack adjacent targets!", intBK);

                                    CurrentMode = Combat::Mode::NORMAL;
                                }
                            }
                            else if (CurrentMode == Combat::Mode::SHOOT)
                            {
                                SelectedSpell = -1;

                                if (!Interface::IsAdjacent(Map, PlayerId, TargetId) && Engine::IsAlive(Target))
                                {
                                    auto Result = Interface::Fight(Renderer, Controls, intBK, Map, Character, Target, Combat::FightMode::SHOOT, false);

                                    if (!Engine::IsAlive(Target))
                                    {
                                        Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " killed!", intGR);

                                        Interface::Remove(Map, SelectX, SelectY);

                                        Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                    }

                                    if (Result != Combat::Result::NONE)
                                    {
                                        CycleCombatants();
                                    }
                                    else
                                    {
                                        DisplayMessage("Shot canceled", intGR);

                                        CurrentMode = Combat::Mode::NORMAL;
                                    }

                                    Selected = false;

                                    Current = -1;
                                }
                                else if (Engine::IsAlive(Target))
                                {
                                    DisplayMessage("You can only shoot from range!", intBK);

                                    CurrentMode = Combat::Mode::NORMAL;
                                }
                            }
                            else if (CurrentMode == Combat::Mode::MOVE)
                            {
                                SelectedSpell = -1;

                                DisplayMessage("You cannot move there!", intBK);
                            }
                            else if (CurrentMode == Combat::Mode::CAST)
                            {
                                auto Proceed = true;

                                if (Character.Spells[SelectedSpell].Type == Spell::Type::GhastlyTouch)
                                {
                                    if (!Interface::IsAdjacent(Map, PlayerId, TargetId))
                                    {
                                        DisplayMessage("Ghastly Touch can only be cast on an adjacent target!", intBK);

                                        Proceed = false;
                                    }
                                }

                                // attempt to spell
                                auto Result = Proceed ? Interface::CastSpell(Renderer, Controls, intBK, Map, Character, SelectedSpell) : Spell::Result::NONE;

                                if (Result != Spell::Result::NONE)
                                {
                                    if (Result == Spell::Result::SUCCESS)
                                    {
                                        // apply spell effects
                                        Interface::ApplySpellEffects(Renderer, Controls, intBK, Map, Party, Enemies, PlayerId, TargetId, Character.Spells[SelectedSpell].Type, CombatRound, StartMap);

                                        Character.Spells.erase(Character.Spells.begin() + SelectedSpell);

                                        SelectedSpell = -1;

                                        Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                    }

                                    CycleCombatants();
                                }
                                else
                                {
                                    CurrentMode = Combat::Mode::NORMAL;
                                }

                                SelectedSpell = -1;
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
                                DisplayMessage("You cannot move there!", intBK);
                            }
                        }
                    }

                    Selected = false;
                }
                else if (Enthraled)
                {
                    Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

                    auto EnemyId = GetId(CurrentCombatant);

                    auto CurrentX = -1;

                    auto CurrentY = -1;

                    Enemy::Base &Enemy = Enemies[EnemyId];

                    Interface::Find(Map, Map::Object::Enemy, EnemyId, CurrentX, CurrentY);

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

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_DOWN || ScrollDown)
                        {
                            if (Map.MapY < Map.Height - Map.SizeY)
                            {
                                Map.MapY++;

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_LEFT)
                        {
                            if (Map.MapX > 0)
                            {
                                Map.MapX--;

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::MAP_RIGHT)
                        {
                            if (Map.MapX < Map.Width - Map.SizeX)
                            {
                                Map.MapX++;

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::DEFEND && !Hold)
                        {
                            CycleCombatants();

                            Selected = false;

                            Current = -1;
                        }
                        else if (Controls[Current].Type == Control::Type::PLAYER && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                auto result = Interface::Find(Sequence, Map::Object::Player, Map.Tiles[SelectY][SelectX].Id - 1);

                                if (result != SelectedCombatant)
                                {
                                    SelectedCombatant = result;
                                }
                                else
                                {
                                    ResetSelection();
                                }
                            }
                            else if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                DisplayMessage("You cannot fight that!", intBK);
                            }
                            else if (CurrentMode == Combat::Mode::SHOOT)
                            {
                                DisplayMessage("You cannot shoot that!", intBK);
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
                                    auto EnemyPath = AStar::FindPath(Map, CurrentX, CurrentY, SelectX, SelectY);

                                    if (EnemyPath.Points.size() > 2)
                                    {
                                        if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, Party, Enemies, CurrentX, CurrentY, EnemyPath.Points[1].X, EnemyPath.Points[1].Y))
                                        {
                                            DisplayMessage("Path blocked!", intBK);
                                        }
                                        else
                                        {
                                            Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

                                            CycleCombatants();
                                        }
                                    }
                                    else
                                    {
                                        DisplayMessage("Path blocked!", intBK);
                                    }
                                }
                                else
                                {
                                    if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, Party, Enemies, CurrentX, CurrentY, SelectX, SelectY))
                                    {
                                        DisplayMessage("Path Blocked!", intBK);
                                    }
                                    else
                                    {
                                        Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

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
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::ATTACK && !Hold)
                        {
                            SelectedSpell = -1;

                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                if (Interface::NearbyOpponents(Map, Enemies, EnemyId, false))
                                {
                                    CurrentMode = Combat::Mode::ATTACK;
                                }
                                else
                                {
                                    DisplayMessage("There are no opponents nearby!", intBK);
                                }
                            }
                            else
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::SHOOT && !Hold)
                        {
                            SelectedSpell = -1;

                            DisplayMessage("Cannot shoot!", intBK);

                            CurrentMode = Combat::Mode::NORMAL;
                        }
                        else if (Controls[Current].Type == Control::Type::ABILITY && !Hold)
                        {
                            SelectedSpell = -1;

                            DisplayMessage("You have no usable special abilities!", intBK);
                        }
                        else if (Controls[Current].Type == Control::Type::ENEMY && !Hold)
                        {
                            auto TargetId = Map.Tiles[SelectY][SelectX].Id - 1;

                            Enemy::Base &Target = Enemies[TargetId];

                            if (CurrentMode == Combat::Mode::NORMAL)
                            {
                                auto result = Interface::Find(Sequence, Map::Object::Enemy, TargetId);

                                if (result != SelectedCombatant)
                                {
                                    SelectedCombatant = result;
                                }
                                else
                                {
                                    ResetSelection();
                                }
                            }
                            else if (CurrentMode == Combat::Mode::ATTACK)
                            {
                                if (Interface::IsAdjacent(Map, EnemyId, Map::Object::Enemy, TargetId, Map::Object::Enemy) && Engine::IsAlive(Target))
                                {
                                    // implement Enemy vs Enemy fight
                                    if (EnemyId != TargetId)
                                    {
                                        auto Result = Combat::Result::NONE;

                                        if (Interface::BreakControl(Renderer, Controls, intBK, Map, Enemy, 1, 6))
                                        {
                                            Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " has broken the enthralment!", intGR);

                                            Enemy.Enthraled = false;

                                            Result = Combat::Result::UNSUCCESSFUL;
                                        }
                                        else
                                        {
                                            // Enemy vs Enemy fight
                                            Result = Interface::Fight(Renderer, Controls, intBK, Map, Enemy, Target, Combat::FightMode::FIGHT);

                                            if (!Engine::IsAlive(Target))
                                            {
                                                Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " killed!", intGR);

                                                Interface::Remove(Map, SelectX, SelectY);

                                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                            }
                                        }

                                        // check Enemy vs Enemy combat results
                                        if (Result != Combat::Result::NONE)
                                        {
                                            CycleCombatants();
                                        }
                                        else
                                        {
                                            DisplayMessage("Fight canceled", intGR);
                                        }
                                    }
                                    else
                                    {
                                        DisplayMessage("Fight aborted!", intBK);
                                    }

                                    Selected = false;

                                    Current = -1;
                                }
                                else if (Engine::IsAlive(Target))
                                {
                                    DisplayMessage("You can only attack adjacent targets!", intBK);

                                    CurrentMode = Combat::Mode::NORMAL;
                                }
                            }
                            else if (CurrentMode == Combat::Mode::SHOOT)
                            {
                                // only specific enemies can shoot, e.g. ninja assassins with shurikens
                                DisplayMessage("This creature cannot attack from range!", intBK);

                                CurrentMode = Combat::Mode::NORMAL;
                            }
                            else if (CurrentMode == Combat::Mode::MOVE)
                            {
                                DisplayMessage("Cannot move there!", intBK);
                            }
                            else if (CurrentMode == Combat::Mode::CAST)
                            {
                                DisplayMessage("Cannot cast spells!", intBK);

                                CurrentMode = Combat::Mode::NORMAL;

                                SelectedSpell = -1;
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
                                if (Map.Tiles[SelectY][SelectX].IsPassableToEnemy)
                                {
                                    if (Interface::Distance(CurrentX, CurrentY, SelectX, SelectY) > 1)
                                    {
                                        auto EnemyPath = AStar::FindPath(Map, CurrentX, CurrentY, SelectX, SelectY);

                                        if (EnemyPath.Points.size() > 2)
                                        {
                                            if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, Party, Enemies, CurrentX, CurrentY, EnemyPath.Points[1].X, EnemyPath.Points[1].Y))
                                            {
                                                DisplayMessage("Path blocked!", intBK);
                                            }
                                            else
                                            {
                                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

                                                CycleCombatants();
                                            }
                                        }
                                        else
                                        {
                                            DisplayMessage("Path blocked!", intBK);
                                        }
                                    }
                                    else
                                    {
                                        if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, Party, Enemies, CurrentX, CurrentY, SelectX, SelectY))
                                        {
                                            DisplayMessage("Path Blocked!", intBK);
                                        }
                                        else
                                        {
                                            Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

                                            CycleCombatants();
                                        }
                                    }
                                }
                                else
                                {
                                    DisplayMessage("Cannot move there!", intBK);
                                }
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::FLEE && !Hold)
                        {
                            if (CurrentMode == Combat::Mode::MOVE)
                            {
                                DisplayMessage("Cannot flee the area!", intBK);
                            }
                            else
                            {
                                CurrentMode = Combat::Mode::NORMAL;
                            }
                        }
                    }

                    Selected = false;
                }
                else if (IsEnemy(CurrentCombatant) && !Enthraled)
                {
                    auto EnemyId = GetId(CurrentCombatant);

                    // Enemy attacks / moves
                    auto EnemyX = -1;

                    auto EnemyY = -1;

                    Interface::Find(Map, Map::Object::Enemy, EnemyId, EnemyX, EnemyY);

                    auto NearestPlayer = Interface::SelectTarget(Map, Party, GetId(CurrentCombatant));

                    auto PlayerId = Target(NearestPlayer);

                    if (PlayerId >= 0 && PlayerId < Party.Members.size())
                    {
                        auto LocationX = -1;

                        auto LocationY = -1;

                        Interface::Find(Map, Map::Object::Player, Target(NearestPlayer), LocationX, LocationY);

                        if (TargetDistance(NearestPlayer) <= 1)
                        {
                            // do attack
                            auto Result = Interface::Fight(Renderer, Controls, intBK, Map, Party.Members[PlayerId], Enemies[EnemyId], Combat::FightMode::FIGHT, true);

                            if (!Engine::IsAlive(Party.Members[PlayerId]))
                            {
                                Interface::RenderMessage(Renderer, Controls, Map, intBK, std::string(Character::ClassName[Party.Members[PlayerId].Class]) + " killed!", intBK);

                                Interface::Remove(Map, LocationX, LocationY);

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }

                            if (Result != Combat::Result::UNSUCCESSFUL)
                            {
                                Engine::ResetSpellDifficulty(Party.Members[PlayerId]);
                            }
                        }
                        else
                        {
                            // TODO: check if enemy can shoot from range (enemy-specific)

                            // close distance
                            auto EnemyPath = AStar::FindPath(Map, EnemyX, EnemyY, LocationX, LocationY, true);

                            if (EnemyPath.Points.size() > 2)
                            {
                                Interface::AnimateMove(Renderer, Controls, intBK, Map, Party, Enemies, EnemyX, EnemyY, EnemyPath.Points[1].X, EnemyPath.Points[1].Y);

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                        }
                    }

                    CycleCombatants();
                }

                if (!Engine::IsAlive(Party) || !Engine::IsAlive(Enemies) || Engine::Escaped(Party) || Engine::Enthraled(Enemies))
                {
                    done = true;
                }
            }
        }

        Engine::ClearDefendingStatus(Party);

        Engine::ResetSpellDifficulty(Party);

        // track Enemies who have survive
        auto SurvivingEnemies = std::vector<Enemy::Base>();

        for (auto i = 0; i < Enemies.size(); i++)
        {
            if (Engine::IsAlive(Enemies[i]))
            {
                SurvivingEnemies.push_back(Enemies[i]);
            }
        }

        if (SurvivingEnemies.size() > 0)
        {
            Party.Enemies.push_back(Party::SurvivingEnemies(Party.Book, Party.Story, SurvivingEnemies));
        }

        if (Engine::Enthraled(Enemies))
        {
            return Combat::Result::ENTHRALED;
        }
        else if (Engine::Escaped(Party))
        {
            return Combat::Result::ESCAPED;
        }
        else if (Engine::IsAlive(Party))
        {
            return Combat::Result::VICTORY;
        }

        return Combat::Result::DEFEAT;
    }

    Story::Base *FindStory(Engine::Destination destination)
    {
        Story::Base *Next = &Story::notImplemented;

        auto Book = destination.first;

        auto Story = destination.second;

        if (Book == Book::Type::Book1)
        {
            Next = Story::FindStory(Story, Book1::Stories);
        }

        Next->Book = Book;

        Next->Id = Story;

        return Next;
    }

    void DisplayParty(SDL_Renderer *Renderer, Party::Base &Party, ScreenDimensions &Screen)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);

        for (auto i = 0; i < Party.Members.size(); i++)
        {
            auto EquipmentFPR = Engine::Equipment(Party.Members[i], Attributes::Type::FightingProwess, false);
            auto EquipmentPSY = Engine::Equipment(Party.Members[i], Attributes::Type::PsychicAbility, false);
            auto EquipmentAWR = Engine::Equipment(Party.Members[i], Attributes::Type::Awareness, false);
            auto Weapons = Engine::Equipment(Party.Members[i], Attributes::Type::FightingProwess, true);

            auto FightingProwess = Engine::FightingProwess(Party.Members[i]) + (EquipmentFPR.size() > 0 ? EquipmentFPR[0].Score : 0) + (Weapons.size() > 0 ? Weapons[0].Score : 0);
            auto PsychicAbility = Engine::PsychicAbility(Party.Members[i]) + (EquipmentPSY.size() > 0 ? EquipmentPSY[0].Score : 0);
            auto Awareness = Engine::Awareness(Party.Members[i]) + (EquipmentAWR.size() > 0 ? EquipmentAWR[0].Score : 0);
            auto Endurance = Engine::Endurance(Party.Members[i]);
            auto Armour = Engine::Armour(Party.Members[i]);
            auto Damage = Party.Members[i].Damage;
            auto DamageModifier = Party.Members[i].DamageModifier + (Weapons.size() > 0 ? Weapons[0].Damage : 0);

            Graphics::RenderImage(Renderer, Assets::Get(Party.Members[i].Asset), Screen.InfoBoxX + text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + FontSize);
            Graphics::PutText(Renderer, Character::ClassName[Party.Members[i].Class], Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize));
            Graphics::PutText(Renderer, ("RNK: " + std::to_string(Party.Members[i].Rank)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + 3 * Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize));
            Graphics::PutText(Renderer, ("FPR: " + std::to_string(FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + FontSize);
            Graphics::PutText(Renderer, ("AWR: " + std::to_string(Awareness)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + 3 * Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + FontSize);
            Graphics::PutText(Renderer, ("ARM: " + std::to_string(Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + 5 * Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + FontSize);
            Graphics::PutText(Renderer, ("PSY: " + std::to_string(PsychicAbility)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + 2 * FontSize);
            Graphics::PutText(Renderer, ("END: " + std::to_string(Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + 3 * Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + 2 * FontSize);
            Graphics::PutText(Renderer, ("DMG: " + (std::to_string(Damage) + "D" + (DamageModifier < 0 ? "" : "+") + std::to_string(DamageModifier))).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + 5 * Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + 2 * FontSize);
        }

        // stats/info box
        Graphics::DrawRect(Renderer, Screen.InfoBoxWidth, Screen.InfoBoxHeight, Screen.InfoBoxX, Screen.InfoBoxY, intWH);
    }

    void RenderStoryScreen(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, ScreenDimensions &Screen, std::vector<Button> &Controls, SDL_Surface *Text, int Current, int Offset)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);

        std::string title_string = "";

        if (!Story->Title.empty())
        {
            title_string = Story->Title;

            SDL_SetWindowTitle(Window, Story->Title.c_str());
        }
        else
        {
            if (Story->Id != -1)
            {
                auto StoryId = Story->Id;

                if (StoryId < 0 && Story->DisplayId >= 0)
                {
                    StoryId = Story->DisplayId;
                }

                title_string = "Blood Sword - " + std::string(Book::Title[Story->Book]) + ": " + std::string(3 - std::to_string(std::abs(StoryId)).length(), '0') + std::to_string(std::abs(StoryId));

                SDL_SetWindowTitle(Window, title_string.c_str());
            }
            else
            {
                title_string = "Blood Sword - " + std::string(Book::Title[Story->Book]) + ": Not Implemented Yet";

                SDL_SetWindowTitle(Window, title_string.c_str());
            }
        }

        Graphics::FillWindow(Renderer, intBK);

        // title string
        Graphics::PutText(Renderer, title_string.c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoWidth, FontSize, Screen.InfoBoxX, Screen.InfoBoxY - (FontSize + text_space));

        // text box
        Graphics::FillRect(Renderer, Screen.TextBoxWidth, Screen.TextBoxHeight, Screen.TextBoxX, Screen.TextBoxY, intWH);

        if (!Story->Text.empty() && !Story->Image.empty() && Text)
        {
            Graphics::RenderImage(Renderer, Text, Screen.TextBoxX + text_space, Screen.TextBoxY + text_space, Screen.TextBounds, Offset);
        }
        else if (!Story->Text.empty() && Text)
        {
            Graphics::RenderText(Renderer, Text, 0, Screen.TextBoxX + text_space, Screen.TextBoxY + text_space, Screen.TextBounds, Offset);
        }

        // party display
        Interface::DisplayParty(Renderer, Party, Screen);

        Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);
    }

    std::vector<Button> EquipmentList(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Equipment::Base> &Equipment, int WindowW, int WindowH, int WindowTextX, int WindowTextY, int Start, int Last, int Limit, SDL_Color Fg, Uint32 Bg, Uint32 Highlight, Equipment::Mode Mode)
    {
        auto IconSize = (buttonw + 2 * text_space);
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowTextWidth = WindowW - (IconSize + 5 * text_space);
        auto OffsetY = (WindowTextY + WindowH) - (IconSize + 2 * (FontSize + text_space));

        auto Controls = std::vector<Button>();

        if (Equipment.size() > 0)
        {
            for (auto i = 0; i < Last - Start; i++)
            {
                auto index = Start + i;

                auto y = (i > 0 ? Controls[i - 1].Y + Controls[i - 1].H + 3 * text_space : WindowTextY + 2 * text_space);

                Controls.push_back(Button(i, Graphics::CreateHeaderButton(Window, FONT_BOOKMAN, FontSize, Equipment[index].String().c_str(), Fg, Bg, WindowTextWidth - 2 * text_space, FontSize * 2, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, WindowTextX, y, Highlight, Control::Type::CHOICE));

                Controls[i].W = Controls[i].Surface->w;

                Controls[i].H = Controls[i].Surface->h;
            }
        }

        auto idx = (int)Controls.size();

        if (Equipment.size() > Limit)
        {
            if (Start > 0)
            {
                Controls.push_back(Button(idx, Assets::Get(Assets::Type::Up), idx, idx, idx, idx + 1, WindowTextX + Controls[0].W + 3 * text_space, WindowTextY + 2 * text_space, Highlight, Control::Type::SCROLL_UP));

                idx += 1;
            }

            if (Equipment.size() - Last > 0)
            {
                Controls.push_back(Button(idx, Assets::Get(Assets::Type::Down), idx, idx, Start > 0 ? idx - 1 : idx, idx + 1, WindowTextX + Controls[0].W + 3 * text_space, Controls[Last - Start - 1].Y - border_pts, Highlight, Control::Type::SCROLL_DOWN));

                idx += 1;
            }
        }

        idx = (int)Controls.size();

        if (Mode == Equipment::Mode::USE || Mode == Equipment::Mode::DROP)
        {
            Controls.push_back(Button(idx, Assets::Get(Assets::Type::Use), idx, idx + 1, idx > 0 ? idx - 1 : idx, idx, WindowTextX, OffsetY, Highlight, Control::Type::USE));
            Controls.push_back(Button(idx + 1, Assets::Get(Assets::Type::Transfer), idx, idx + 2, idx > 0 ? idx - 1 : idx + 1, idx + 1, WindowTextX + IconSize, OffsetY, Highlight, Control::Type::TRANSFER));
            Controls.push_back(Button(idx + 2, Assets::Get(Assets::Type::Cancel), idx + 1, idx + 3, idx > 0 ? idx - 1 : idx + 2, idx + 2, WindowTextX + 2 * IconSize, OffsetY, Highlight, Control::Type::DROP));
            Controls.push_back(Button(idx + 3, Assets::Get(Assets::Type::Back), idx + 2, idx + 3, idx > 0 ? idx - 1 : idx + 3, idx + 3, WindowTextX + 3 * IconSize, OffsetY, Highlight, Control::Type::BACK));
        }
        else if (Mode == Equipment::Mode::TAKE)
        {
            Controls.push_back(Button(idx, Assets::Get(Assets::Type::Use), idx, idx + 1, idx > 0 ? idx - 1 : idx, idx, WindowTextX, OffsetY, Highlight, Control::Type::TAKE));
            Controls.push_back(Button(idx + 1, Assets::Get(Assets::Type::Back), idx, idx + 1, idx > 0 ? idx - 1 : idx + 1, idx + 1, WindowTextX + IconSize, OffsetY, Highlight, Control::Type::BACK));
        }

        return Controls;
    }

    int Choose(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, ScreenDimensions &Screen, SDL_Surface *Text, int Offset, std::vector<Assets::Type> Assets, std::vector<std::string> Captions, const char *Message)
    {
        auto Result = -1;

        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 4 * FontSize + Screen.IconSize;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto ButtonX = WindowX + 2 * text_space;

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<Button> Controls = {};

        auto NumControls = 0;

        for (auto i = 0; i < Assets.size(); i++)
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets[NumControls]), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * (Screen.IconSize + 2 * text_space), WindowY + Screen.IconSize, intBK, Control::Type::CHOICE));

            NumControls++;
        }

        Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Back), NumControls > 0 ? NumControls - 1 : 0, NumControls, NumControls, NumControls, ButtonX + NumControls * (Screen.IconSize + 2 * text_space), WindowY + Screen.IconSize, intBK, Control::Type::BACK));

        auto done = false;

        while (!done)
        {
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, Text, -1, Offset);
            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);
            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intGR);

            Graphics::PutText(Renderer, Message, Fonts::Normal, text_space, clrGR, intWH, TTF_STYLE_NORMAL, WindowW - 4 * text_space, TTF_FontHeight(Fonts::Normal), ButtonX - text_space, WindowY + text_space);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                if (Controls[Current].Type != Control::Type::BACK)
                {
                    Interface::RenderChoiceCaption(Renderer, Controls[Current], Captions[Current], clrBK, intWH);
                }
                else
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrBK, intWH);
                }
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK)
                {
                    Result = -1;

                    done = true;
                }
                else
                {
                    Result = Current;

                    done = true;
                }
            }
        }

        return Result;
    }

    int SelectAdventurer(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, ScreenDimensions &Screen, SDL_Surface *Text, int Offset, const char *SelectMessage)
    {
        auto Result = -1;
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 2 * Screen.TextBoxHeight / 5;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto WindowTextWidth = WindowW - 4 * text_space;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;

        auto ButtonX = WindowX + 2 * text_space;

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        auto OffsetY = (WindowY + WindowH) - (Screen.IconSize + 2 * text_space + FontSize);

        auto Controls = std::vector<Button>();

        for (auto i = 0; i < Party.Members.size(); i++)
        {
            Controls.push_back(Button(i, Assets::Get(Party.Members[i].Asset), i > 0 ? i - 1 : i, i + 1, i, i, ButtonX + i * Screen.IconSize, OffsetY, intWH, Interface::PlayerControls[Party.Members[i].Class]));
        }

        auto idx = Controls.size();

        Controls.push_back(Button(idx, Assets::Get(Assets::Type::Back), idx > 0 ? idx - 1 : idx, idx, idx, idx, ButtonX + idx * Screen.IconSize, OffsetY, intWH, Control::Type::BACK));

        auto done = false;

        while (!done)
        {
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, Text, -1, Offset);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, SelectMessage, Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, WindowTextWidth, RowHeight, ButtonX, TextY);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, 4);

            if (Current >= 0 && Current < Controls.size())
            {
                Graphics::RenderCaption(Renderer, Controls[Current], clrGR, intBK);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK && !Hold)
                {
                    done = true;
                }
                else if (Current >= 0 && Current < Party.Members.size() && !Hold)
                {
                    Result = Current;

                    done = true;
                }
            }
        }

        return Result;
    }

    void ItemScreen(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, ScreenDimensions &Screen, SDL_Surface *Text, int Offset, int Character, Equipment::Mode Mode)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 12 * FontSize + Screen.IconSize;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto WindowTextWidth = WindowW - 4 * text_space;
        auto TextY = WindowY + 2 * text_space;
        auto ButtonX = WindowX + 2 * text_space;

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;
        auto ScrollSpeed = 1;

        auto ItemOffset = 0;
        auto Limit = (8 * FontSize) / (2 * FontSize + 2 * text_space);
        auto Last = ItemOffset + Limit;

        std::vector<Equipment::Base> &Equipment = Party.Members[Character].Equipment;

        if (Last > Equipment.size())
        {
            Last = Equipment.size();
        }

        auto Fg = clrGR;
        auto Bg = intWH;
        auto Highlight = intBK;

        auto Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Mode);

        auto done = false;

        auto Selection = std::vector<int>();

        while (!done)
        {
            if (Equipment.size() > Party.Members[Character].Encumbrance)
            {
                Mode = Equipment::Mode::DROP;
            }
            else
            {
                Mode = Equipment::Mode::USE;
            }

            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, Text, -1, Offset);
            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);
            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intGR);

            if (Mode == Equipment::Mode::DROP)
            {
                Graphics::PutText(Renderer, ("The " + std::string(Character::ClassName[Party.Members[Character].Class]) + " is carrying too many items").c_str(), Fonts::Normal, 0, clrBK, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY);
            }
            else
            {
                Graphics::PutText(Renderer, (std::string(Character::ClassName[Party.Members[Character].Class]) + "'s items").c_str(), Fonts::Normal, 0, clrBK, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY);
            }

            // render choice boxes
            for (auto i = 0; i < Controls.size(); i++)
            {
                if (Controls[i].Type == Control::Type::CHOICE)
                {
                    if (Engine::InList(Selection, ItemOffset + i))
                    {
                        Graphics::ThickRect(Renderer, Controls[i].W, Controls[i].H, Controls[i].X, Controls[i].Y, intGR, border_pts);
                    }
                    else
                    {
                        Graphics::DrawRect(Renderer, Controls[i].W + 2 * border_pts, Controls[i].H + 2 * border_pts, Controls[i].X - border_pts, Controls[i].Y - border_pts, intGR);
                    }
                }
            }

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                Graphics::RenderCaption(Renderer, Controls[Current], clrBK, intWH);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK && !Hold)
                {
                    if (Equipment.size() > Party.Members[Character].Encumbrance)
                    {
                        Mode = Equipment::Mode::DROP;
                    }
                    else
                    {
                        done = true;
                    }
                }
                else if (Controls[Current].Type == Control::Type::SCROLL_UP || (Controls[Current].Type == Control::Type::SCROLL_UP && Hold) || ScrollUp)
                {
                    if (ItemOffset > 0)
                    {
                        ItemOffset -= ScrollSpeed;

                        if (ItemOffset < 0)
                        {
                            ItemOffset = 0;
                        }

                        Last = ItemOffset + Limit;

                        if (Last > Equipment.size())
                        {
                            Last = Equipment.size();
                        }

                        Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Mode);

                        SDL_Delay(50);
                    }

                    if (ItemOffset <= 0)
                    {
                        Current = -1;

                        Selected = false;
                    }
                }
                else if (Controls[Current].Type == Control::Type::SCROLL_DOWN || (Controls[Current].Type == Control::Type::SCROLL_DOWN && Hold) || ScrollDown)
                {
                    if (Equipment.size() - Last > 0)
                    {
                        if (ItemOffset < Equipment.size() - Limit)
                        {
                            ItemOffset += ScrollSpeed;
                        }

                        if (ItemOffset > Equipment.size() - Limit)
                        {
                            ItemOffset = Equipment.size() - Limit;
                        }

                        Last = ItemOffset + Limit;

                        if (Last > Equipment.size())
                        {
                            Last = Equipment.size();
                        }

                        Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Equipment::Mode::TAKE);

                        SDL_Delay(50);

                        if (ItemOffset > 0)
                        {
                            Current = Interface::FindControl(Controls, Control::Type::SCROLL_DOWN);
                        }
                    }

                    if (Equipment.size() - Last <= 0)
                    {
                        Selected = false;

                        Current = -1;
                    }
                }
                else if (Controls[Current].Type == Control::Type::CHOICE && !Hold)
                {
                    if (!Engine::InList(Selection, Current + ItemOffset))
                    {
                        Selection.push_back(Current + ItemOffset);
                    }
                    else
                    {
                        Engine::Erase(Selection, Current + ItemOffset);
                    }
                }
                else if (Controls[Current].Type == Control::Type::DROP && !Hold)
                {
                    if (Equipment.size() > 0 && Selection.size() > 0)
                    {
                        std::string Message = "You are about to drop " + (Selection.size() > 1 ? std::string("several items") : "the " + Equipment[Selection[0]].String());

                        std::string Options = Selection.size() > 1 ? "Drop items" : ("Drop the " + Equipment[Selection[0]].String());

                        auto Result = Interface::Choose(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, {Assets::Type::Ok}, {Options.c_str()}, Message.c_str());

                        if (Result == 0)
                        {
                            auto New = std::vector<Equipment::Base>();

                            for (auto i = 0; i < Equipment.size(); i++)
                            {
                                if (!Engine::InList(Selection, i))
                                {
                                    New.push_back(Equipment[i]);
                                }
                            }

                            Equipment = New;

                            Selection.clear();

                            if (Equipment.size() == 0)
                            {
                                done = true;
                            }
                            else
                            {
                                ItemOffset = 0;

                                Last = ItemOffset + Limit;

                                if (Last > Equipment.size())
                                {
                                    Last = Equipment.size();
                                }

                                Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Mode);

                                Current = -1;
                            }
                        }
                    }
                }
            }
        }
    }

    void TakeScreen(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, ScreenDimensions &Screen, SDL_Surface *Text, int Offset)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 12 * FontSize + Screen.IconSize;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto WindowTextWidth = WindowW - 4 * text_space;
        auto TextY = WindowY + 2 * text_space;
        auto ButtonX = WindowX + 2 * text_space;

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;
        auto ScrollSpeed = 1;

        auto ItemOffset = 0;
        auto Limit = (8 * FontSize) / (2 * FontSize + 2 * text_space);
        auto Last = ItemOffset + Limit;

        std::vector<Equipment::Base> &Equipment = Story->Equipment;

        if (Last > Equipment.size())
        {
            Last = Equipment.size();
        }

        auto Fg = clrGR;
        auto Bg = intWH;
        auto Highlight = intBK;

        auto Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Equipment::Mode::TAKE);

        auto done = false;

        auto Selection = std::vector<int>();

        std::string TakeLimit = "";

        if (Story->Limit > 0 && Limit < Story->Equipment.size())
        {
            if (Story->Limit == 1)
            {
                TakeLimit = "Select which item to take";
            }
            else
            {
                TakeLimit = "You may take up to " + std::to_string(Story->Limit) + " items";
            }
        }
        else
        {
            TakeLimit = "Select items to take";
        }

        while (!done)
        {
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, Text, -1, Offset);
            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);
            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intGR);

            Graphics::PutText(Renderer, TakeLimit.c_str(), Fonts::Normal, 0, clrBK, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY);

            // render choice boxes
            for (auto i = 0; i < Controls.size(); i++)
            {
                if (Controls[i].Type == Control::Type::CHOICE)
                {
                    if (Engine::InList(Selection, ItemOffset + i))
                    {
                        Graphics::ThickRect(Renderer, Controls[i].W, Controls[i].H, Controls[i].X, Controls[i].Y, intGR, border_pts);
                    }
                    else
                    {
                        Graphics::DrawRect(Renderer, Controls[i].W + 2 * border_pts, Controls[i].H + 2 * border_pts, Controls[i].X - border_pts, Controls[i].Y - border_pts, intGR);
                    }
                }
            }

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                Graphics::RenderCaption(Renderer, Controls[Current], clrBK, intWH);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK && !Hold)
                {
                    done = true;
                }
                else if (Controls[Current].Type == Control::Type::SCROLL_UP || (Controls[Current].Type == Control::Type::SCROLL_UP && Hold) || ScrollUp)
                {
                    if (ItemOffset > 0)
                    {
                        ItemOffset -= ScrollSpeed;

                        if (ItemOffset < 0)
                        {
                            ItemOffset = 0;
                        }

                        Last = ItemOffset + Limit;

                        if (Last > Equipment.size())
                        {
                            Last = Equipment.size();
                        }

                        Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Equipment::Mode::TAKE);

                        SDL_Delay(50);
                    }

                    if (ItemOffset <= 0)
                    {
                        Current = -1;

                        Selected = false;
                    }
                }
                else if (Controls[Current].Type == Control::Type::SCROLL_DOWN || (Controls[Current].Type == Control::Type::SCROLL_DOWN && Hold) || ScrollDown)
                {
                    if (Equipment.size() - Last > 0)
                    {
                        if (ItemOffset < Equipment.size() - Limit)
                        {
                            ItemOffset += ScrollSpeed;
                        }

                        if (ItemOffset > Equipment.size() - Limit)
                        {
                            ItemOffset = Equipment.size() - Limit;
                        }

                        Last = ItemOffset + Limit;

                        if (Last > Equipment.size())
                        {
                            Last = Equipment.size();
                        }

                        Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Equipment::Mode::TAKE);

                        SDL_Delay(50);

                        if (ItemOffset > 0)
                        {
                            Current = Interface::FindControl(Controls, Control::Type::SCROLL_DOWN);
                        }
                    }

                    if (Equipment.size() - Last <= 0)
                    {
                        Selected = false;

                        Current = -1;
                    }
                }
                else if (Controls[Current].Type == Control::Type::CHOICE && !Hold)
                {
                    if (!Engine::InList(Selection, Current + ItemOffset))
                    {
                        if ((Story->Limit > 0 && Selection.size() <= Story->Limit) || Story->Limit == 0)
                        {
                            Selection.push_back(Current + ItemOffset);
                        }
                    }
                    else
                    {
                        Engine::Erase(Selection, Current + ItemOffset);
                    }
                }
                else if (Controls[Current].Type == Control::Type::TAKE && !Hold)
                {
                    if (Equipment.size() > 0 && Selection.size() > 0)
                    {
                        for (auto i = 0; i < Selection.size(); i++)
                        {
                            std::string TakeMessage = "Give the " + Equipment[Selection[i]].Name + " to";

                            auto Character = Interface::SelectAdventurer(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, TakeMessage.c_str());

                            if (Character >= 0 && Character < Party.Members.size())
                            {
                                Party.Members[Character].Equipment.push_back(Equipment[Selection[i]]);

                                while (Party.Members[Character].Equipment.size() > Party.Members[Character].Encumbrance)
                                {
                                    Interface::ItemScreen(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, Character, Equipment::Mode::DROP);
                                }
                            }
                        }
                    }

                    Equipment.clear();

                    Selection.clear();

                    done = true;
                }
            }
        }
    }

    void ManageAdventurer(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, ScreenDimensions &Screen, SDL_Surface *Text, int Offset, int Character)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 12 * FontSize + Screen.IconSize;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto WindowTextWidth = WindowW - 4 * text_space;
        auto ColumnWidth = WindowTextWidth / 2;
        auto MidColumn = WindowX + ColumnWidth + 2 * text_space;
        auto TextY = WindowY + 2 * text_space;
        auto ButtonX = WindowX + 2 * text_space;
        auto OffsetY = (WindowY + WindowH) - (Screen.IconSize + FontSize + border_pts);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        auto Controls = std::vector<Button>();

        std::vector<Equipment::Base> &Equipment = Party.Members[Character].Equipment;

        Controls.push_back(Button(0, Assets::Get(Assets::Type::Encyclopedia), 0, 1, 0, 0, ButtonX, OffsetY, intBK, Control::Type::INFO));
        Controls.push_back(Button(1, Assets::Get(Assets::Type::Stats), 0, 2, 1, 1, ButtonX + Screen.IconSize, OffsetY, intBK, Control::Type::STATS));
        Controls.push_back(Button(2, Assets::Get(Assets::Type::Ability), 1, 3, 2, 2, ButtonX + 2 * Screen.IconSize, OffsetY, intBK, Control::Type::ABILITY));
        Controls.push_back(Button(3, Assets::Get(Assets::Type::Items), 2, 4, 3, 3, ButtonX + 3 * Screen.IconSize, OffsetY, intBK, Control::Type::ITEMS));
        Controls.push_back(Button(4, Assets::Get(Assets::Type::Back), 3, 4, 3, 3, ButtonX + 4 * Screen.IconSize, OffsetY, intBK, Control::Type::BACK));

        auto done = false;

        while (!done)
        {
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, Text, -1, Offset);
            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);
            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intGR);

            Graphics::PutText(Renderer, Character::ClassName[Party.Members[Character].Class], Fonts::Normal, 0, clrBK, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY);

            if (Current >= 0 && Current < Controls.size())
            {
                if (Controls[Current].Type == Control::Type::STATS)
                {
                    Interface::CharacterSheet(Renderer, Party.Members[Character], Fonts::Normal, WindowTextWidth, ButtonX, TextY, intWH, true);
                }
                else if (Controls[Current].Type == Control::Type::ITEMS && Equipment.size() > 0)
                {
                    for (auto i = 0; i < Equipment.size(); i++)
                    {
                        Graphics::PutText(Renderer, Equipment[i].String().c_str(), Fonts::Normal, 0, clrGR, intWH, TTF_STYLE_NORMAL, ColumnWidth, FontSize, i < 5 ? ButtonX : MidColumn, TextY + 2 * FontSize + (i < 5 ? i : i - 5) * FontSize);
                    }
                }
                else
                {
                    Graphics::PutText(Renderer, Character::Description[Party.Members[Character].Class], Fonts::Normal, 0, clrGR, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY + 2 * FontSize);
                }
            }
            else
            {
                Graphics::PutText(Renderer, Character::Description[Party.Members[Character].Class], Fonts::Normal, 0, clrGR, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY + 2 * FontSize);
            }

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, 4);

            if (Current >= 0 && Current < Controls.size())
            {
                Graphics::RenderCaption(Renderer, Controls[Current], clrBK, intWH);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK && !Hold)
                {
                    done = true;
                }
                if (Controls[Current].Type == Control::Type::ITEMS && !Hold)
                {
                    if (!Equipment.empty())
                    {
                        Interface::ItemScreen(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, Character, Equipment::Mode::USE);
                    }

                    Current = -1;
                }
            }
        }
    }

    std::vector<Button> CreateChoices(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Choice::Base> &Choices, ScreenDimensions &Screen, int Start, int Last, int Limit, SDL_Color Fg, Uint32 Bg, Uint32 Highlight)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);

        auto Controls = std::vector<Button>();

        if (Choices.size() > 0)
        {
            for (auto i = 0; i < Last - Start; i++)
            {
                auto index = Start + i;

                auto y = (i > 0 ? Controls[i - 1].Y + Controls[i - 1].H + 3 * text_space : Screen.TextBoxY + 2 * text_space);

                Controls.push_back(Button(i, Graphics::CreateHeaderButton(Window, FONT_BOOKMAN, FontSize, Choices[index].Text.c_str(), Fg, Bg, Screen.TextWidth - 2 * text_space, FontSize * 2, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, Screen.TextBoxX + 2 * text_space, y, Highlight, Control::Type::CHOICE));

                Controls[i].W = Controls[i].Surface->w;

                Controls[i].H = Controls[i].Surface->h;
            }
        }

        auto idx = (int)Controls.size();

        if (Choices.size() > Limit)
        {
            if (Start > 0)
            {
                Controls.push_back(Button(idx, Assets::Get(Assets::Type::Up), idx, idx, idx, idx + 1, SCREEN_WIDTH - (buttonw + 4 * text_space), (buttonh + 3 * text_space), Highlight, Control::Type::SCROLL_UP));

                idx += 1;
            }

            if (Choices.size() - Last > 0)
            {
                Controls.push_back(Button(idx, Assets::Get(Assets::Type::Down), idx, idx, Start > 0 ? idx - 1 : idx, idx + 1, SCREEN_WIDTH - (buttonw + 4 * text_space), SCREEN_HEIGHT - 3 * (buttonh + 2 * text_space) + text_space, Highlight, Control::Type::SCROLL_DOWN));

                idx += 1;
            }
        }

        idx = (int)Controls.size();

        auto IconSize = (buttonw + 2 * text_space);
        auto OffsetY = SCREEN_HEIGHT - 2 * (IconSize - text_space);
        auto LastX = SCREEN_WIDTH - (2 * IconSize) - (3 * text_space);

        Controls.push_back(Button(idx, Assets::Get(Assets::Type::Back), idx, idx, idx > 0 ? idx - 1 : idx, idx, LastX, OffsetY, Highlight, Control::Type::BACK));

        return Controls;
    }

    void RenderChoiceScreen(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, ScreenDimensions &Screen, std::vector<Button> &Controls, int Current, Uint32 Bg)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);

        std::string title_string = "";

        if (!Story->Title.empty())
        {
            title_string = Story->Title;

            SDL_SetWindowTitle(Window, Story->Title.c_str());
        }
        else
        {
            if (Story->Id != -1)
            {
                auto StoryId = Story->Id;

                if (StoryId < 0 && Story->DisplayId >= 0)
                {
                    StoryId = Story->DisplayId;
                }

                title_string = "Blood Sword - " + std::string(Book::Title[Story->Book]) + ": " + std::string(3 - std::to_string(std::abs(StoryId)).length(), '0') + std::to_string(std::abs(StoryId));

                SDL_SetWindowTitle(Window, title_string.c_str());
            }
            else
            {
                title_string = "Blood Sword - " + std::string(Book::Title[Story->Book]) + ": Not Implemented Yet";

                SDL_SetWindowTitle(Window, title_string.c_str());
            }
        }

        Graphics::FillWindow(Renderer, intBK);

        // title string
        Graphics::PutText(Renderer, title_string.c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoWidth, FontSize, Screen.InfoBoxX, Screen.InfoBoxY - (FontSize + text_space));

        // display party
        Interface::DisplayParty(Renderer, Party, Screen);

        // text box
        Graphics::FillRect(Renderer, Screen.TextBoxWidth, Screen.TextBoxHeight, Screen.TextBoxX, Screen.TextBoxY, intWH);

        // render choice boxes
        for (auto i = 0; i < Controls.size(); i++)
        {
            if (Controls[i].Type == Control::Type::CHOICE)
            {
                Graphics::FillRect(Renderer, Controls[i].W + text_space, Controls[i].H + text_space, Controls[i].X - border_pts, Controls[i].Y - border_pts, Bg);
            }
        }

        Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);
    }

    Attributes::Result Test(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &ChoiceScreen, Uint32 Bg, ScreenDimensions &Screen, Story::Base *Story, Party::Base &Party, int Character, Attributes::Type Attribute)
    {
        auto Result = Attributes::Result::NONE;
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 4 * Screen.TextBoxHeight / 5;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto ColumnWidth = WindowW - 4 * text_space;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;

        auto TextButtonX = WindowX + 2 * text_space;
        auto TextButtonY = (WindowY + WindowH) - (text_buttonh + 2 * text_space);
        auto TextWidth = WindowW - 3 * text_space;

        const char *ResistChoices[2] = {"TEST"};

        auto ResistControls = Graphics::CreateFixedTextButtons(ResistChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        ResistControls[0].Fg = clrWH;
        ResistControls[0].Highlight = intGR;
        ResistControls[0].Color = intBK;
        ResistControls[0].Type = Control::Type::TEST;

        const char *DoneChoices[1] = {"DONE"}; // end of psychic resistance check
        auto DoneControls = Graphics::CreateFixedTextButtons(DoneChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DoneControls[0].Fg = clrWH;
        DoneControls[0].Highlight = intGR;
        DoneControls[0].Color = intBK;
        DoneControls[0].Type = Control::Type::BACK;

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

        std::vector<TextButton> &Controls = ResistControls;

        auto done = false;

        auto CurrentStage = Attributes::Stage::START;

        Engine::Randomize();

        auto TestRolls = 2;

        std::vector<int> Rolls(TestRolls, 0);

        auto TestSum = 0;

        auto AttributeValue = 0;

        auto Equipment = Engine::Equipment(Party.Members[Character], Attribute, false);

        auto Weapons = Engine::Equipment(Party.Members[Character], Attribute, true);

        if (Attribute == Attributes::Type::FightingProwess)
        {
            AttributeValue = (Engine::FightingProwess(Party.Members[Character]) + (Equipment.size() > 0 ? Equipment[0].Score : 0) + (Weapons.size() > 0 ? Weapons[0].Score : 0));
        }
        else if (Attribute == Attributes::Type::PsychicAbility)
        {
            AttributeValue = (Engine::PsychicAbility(Party.Members[Character]) + (Equipment.size() > 0 ? Equipment[0].Score : 0));
        }
        else if (Attribute == Attributes::Type::Awareness)
        {
            AttributeValue = (Engine::Awareness(Party.Members[Character]) + (Equipment.size() > 0 ? Equipment[0].Score : 0));
        }

        while (!done)
        {
            // render current combat screen
            Interface::RenderChoiceScreen(Window, Renderer, Party, Story, Screen, ChoiceScreen, -1, Bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + " TEST").c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY);

            Graphics::PutText(Renderer, Character::ClassName[Party.Members[Character].Class], Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 2 * RowHeight);

            Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + ": " + std::to_string(AttributeValue)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 3 * RowHeight);

            if (CurrentStage == Attributes::Stage::TEST && Result == Attributes::Result::NONE)
            {
                TestSum = 0;

                for (auto i = 0; i < TestRolls; i++)
                {
                    Rolls[i] = Engine::Roll(1, 0);

                    TestSum += Rolls[i];
                }

                TestSum = std::max(0, TestSum);

                auto TestResult = AttributeValue >= TestSum;

                if (TestResult)
                {
                    Result = Attributes::Result::SUCCESS;
                }
                else
                {
                    Result = Attributes::Result::FAILURE;
                }

                CurrentStage = Attributes::Stage::END;
            }
            else if (CurrentStage == Attributes::Stage::END)
            {
                // show casting results
                for (auto i = 0; i < TestRolls; i++)
                {
                    Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Screen.ObjectSize + 2 * text_space), TextY + 5 * RowHeight, Screen.ObjectSize, Screen.ObjectSize);
                }

                Graphics::PutText(Renderer, ("Test Score: " + std::to_string(TestSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + 6 * RowHeight + Screen.ObjectSize);

                if (Result == Attributes::Result::SUCCESS)
                {
                    Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + " test passed!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + 7 * RowHeight + Screen.ObjectSize);
                }
                else
                {
                    Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + " test failed!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + 7 * RowHeight + Screen.ObjectSize);
                }
            }

            Graphics::RenderTextButtons(Renderer, Controls, FONT_BOOKMAN, Current, 24, TTF_STYLE_NORMAL);

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::TEST && !Hold)
                {
                    CurrentStage = Attributes::Stage::TEST;

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

    Story::Base *ProcessChoices(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, ScreenDimensions &Screen)
    {
        Story::Base *Next = &Story::notImplemented;

        auto FlashMessage = false;

        auto FlashColor = intGR;

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

        auto RenderFlashMessage = [&]()
        {
            if (FlashMessage)
            {
                if ((SDL_GetTicks() - StartTicks) < Duration)
                {
                    auto FlashW = 3 * SCREEN_WIDTH / 5;

                    auto FlashH = SCREEN_HEIGHT / 5;

                    Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, FlashH, (SCREEN_WIDTH - FlashW) / 2, (SCREEN_HEIGHT - FlashH) / 2);

                    if (FlashColor == intBK)
                    {
                        Graphics::DrawRect(Renderer, FlashW, FlashH, (SCREEN_WIDTH - FlashW) / 2, (SCREEN_HEIGHT - FlashH) / 2, intWH);
                    }
                }
                else
                {
                    FlashMessage = false;
                }
            }
        };

        if (Renderer && Story->Choices.size() > 0)
        {
            auto FontSize = TTF_FontHeight(Fonts::Normal);
            auto Selected = false;
            auto Current = -1;
            auto Done = false;
            auto ScrollUp = false;
            auto ScrollDown = false;
            auto Hold = false;
            auto ScrollSpeed = 1;

            auto Offset = 0;
            auto Limit = (Screen.TextBounds) / (2 * FontSize + 2 * text_space);
            auto Last = Offset + Limit;

            if (Last > Story->Choices.size())
            {
                Last = Story->Choices.size();
            }

            auto Fg = clrBK;
            auto Bg = intGR;
            auto Highlight = intBK;

            auto Controls = Interface::CreateChoices(Window, Renderer, Story->Choices, Screen, Offset, Last, Limit, Fg, Bg, Highlight);

            while (!Done)
            {
                Interface::RenderChoiceScreen(Window, Renderer, Party, Story, Screen, Controls, Current, Bg);

                if (Current >= 0 && Current < Controls.size())
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                }

                RenderFlashMessage();

                Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

                if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
                {
                    if (Controls[Current].Type == Control::Type::SCROLL_UP || (Controls[Current].Type == Control::Type::SCROLL_UP && Hold) || ScrollUp)
                    {
                        if (Offset > 0)
                        {
                            Offset -= ScrollSpeed;

                            if (Offset < 0)
                            {
                                Offset = 0;
                            }

                            Last = Offset + Limit;

                            if (Last > Story->Choices.size())
                            {
                                Last = Story->Choices.size();
                            }

                            Controls = Interface::CreateChoices(Window, Renderer, Story->Choices, Screen, Offset, Last, Limit, Fg, Bg, Highlight);

                            SDL_Delay(50);
                        }

                        if (Offset <= 0)
                        {
                            Current = -1;

                            Selected = false;
                        }
                    }
                    else if (Controls[Current].Type == Control::Type::SCROLL_DOWN || (Controls[Current].Type == Control::Type::SCROLL_DOWN && Hold) || ScrollDown)
                    {
                        if (Story->Choices.size() - Last > 0)
                        {
                            if (Offset < Story->Choices.size() - Limit)
                            {
                                Offset += ScrollSpeed;
                            }

                            if (Offset > Story->Choices.size() - Limit)
                            {
                                Offset = Story->Choices.size() - Limit;
                            }

                            Last = Offset + Limit;

                            if (Last > Story->Choices.size())
                            {
                                Last = Story->Choices.size();
                            }

                            Controls = Interface::CreateChoices(Window, Renderer, Story->Choices, Screen, Offset, Last, Limit, Fg, Bg, Highlight);

                            SDL_Delay(50);

                            if (Offset > 0)
                            {
                                Current = Interface::FindControl(Controls, Control::Type::SCROLL_DOWN);
                            }
                        }

                        if (Story->Choices.size() - Last <= 0)
                        {
                            Selected = false;

                            Current = -1;
                        }
                    }
                    else if (Controls[Current].Type == Control::Type::CHOICE && !Hold)
                    {
                        auto Choice = Current + Offset;

                        if (Choice >= 0 && Choice < Story->Choices.size())
                        {
                            if (Story->Choices[Choice].Type == Choice::Type::Normal)
                            {
                                Next = Interface::FindStory(Story->Choices[Choice].Destination);

                                Done = true;
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::Character)
                            {
                                if (Engine::IsPresent(Party, Story->Choices[Choice].Character))
                                {
                                    Next = Interface::FindStory(Story->Choices[Choice].Destination);

                                    Done = true;
                                }
                                else
                                {
                                    DisplayMessage(("No " + std::string(Character::ClassName[Story->Choices[Choice].Character]) + "s present in your party!").c_str(), intBK);
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::Attribute)
                            {
                                if (Engine::IsPresent(Party, Story->Choices[Choice].Character))
                                {
                                    auto Character = Engine::Find(Party, Story->Choices[Choice].Character);

                                    auto Result = Interface::Test(Window, Renderer, Controls, intGR, Screen, Story, Party, Character, Story->Choices[Choice].Attribute);

                                    if (Result == Attributes::Result::SUCCESS)
                                    {
                                        Next = Interface::FindStory(Story->Choices[Choice].Destination);

                                        Done = true;
                                    }
                                    else
                                    {
                                        Next = Interface::FindStory(Story->Choices[Choice].DestinationFail);

                                        Done = true;
                                    }
                                }
                                else
                                {
                                    DisplayMessage(("No " + std::string(Character::ClassName[Story->Choices[Choice].Character]) + "s present in your party!").c_str(), intBK);
                                }
                            }
                        }
                    }
                    else if (Controls[Current].Type == Control::Type::BACK && !Hold)
                    {
                        Next = Story;

                        Done = true;
                    }
                }
            }
        }

        return Next;
    }

    Story::Base *RenderChoices(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, ScreenDimensions &Screen)
    {
        Story::Base *Next = &Story::notImplemented;

        if (Story->Choices.size() > 0)
        {
            Next = Interface::ProcessChoices(Window, Renderer, Party, Story, Screen);
        }
        else
        {
            Next = Interface::FindStory(Story->Continue(Party));
        }

        return Next;
    }

    void ProcessStory(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story)
    {
        Interface::ScreenDimensions Screen;

        if (Window && Renderer)
        {
            auto FlashMessage = false;

            auto FlashColor = intGR;

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

            auto RenderFlashMessage = [&]()
            {
                if (FlashMessage)
                {
                    if ((SDL_GetTicks() - StartTicks) < Duration)
                    {
                        auto FlashW = 3 * SCREEN_WIDTH / 5;

                        auto FlashH = SCREEN_HEIGHT / 5;

                        Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, FlashH, (SCREEN_WIDTH - FlashW) / 2, (SCREEN_HEIGHT - FlashH) / 2);

                        if (FlashColor == intBK)
                        {
                            Graphics::DrawRect(Renderer, FlashW, FlashH, (SCREEN_WIDTH - FlashW) / 2, (SCREEN_HEIGHT - FlashH) / 2, intWH);
                        }
                    }
                    else
                    {
                        FlashMessage = false;
                    }
                }
            };

            auto FontSize = TTF_FontHeight(Fonts::Normal);

            std::vector<Button> Controls = {};

            auto Quit = false;

            Screen.IconSize = (buttonw + 2 * text_space);
            Screen.BoxWidth = (SCREEN_WIDTH - 2 * Screen.IconSize) / 2 + text_space;
            Screen.BoxHeight = (SCREEN_HEIGHT - 3 * (buttonh + 2 * text_space));
            Screen.TextBoxWidth = Screen.BoxWidth;
            Screen.TextBoxHeight = Screen.BoxHeight;
            Screen.TextBoxX = Screen.IconSize / 2 + Screen.BoxWidth - text_space;
            Screen.TextBoxY = Screen.IconSize;
            Screen.TextBounds = Screen.TextBoxHeight - 2 * text_space;
            Screen.TextWidth = Screen.TextBoxWidth - 2 * text_space;
            Screen.InfoBoxWidth = Screen.TextBoxWidth - 2 * text_space;
            Screen.InfoBoxHeight = Screen.TextBoxHeight;
            Screen.InfoWidth = Screen.InfoBoxWidth - 2 * text_space;
            Screen.InfoBoxX = Screen.IconSize / 2;
            Screen.InfoBoxY = Screen.IconSize;
            Screen.ObjectSize = buttonw;

            while (!Quit)
            {
                SDL_Surface *Text = NULL;

                auto RunOnce = true;

                if (RunOnce)
                {
                    RunOnce = false;

                    auto Jump = Story->Background(Party);

                    if (Jump.first != Book::Type::None)
                    {
                        Story = Interface::FindStory(Jump);

                        continue;
                    }

                    Story->Event(Party);
                }

                // generate story text/image surface
                if (!Story->Image.empty() && !Story->Text.empty())
                {
                    Text = Graphics::CreateTextAndImage(Story->Text.c_str(), Story->Image.c_str(), FONT_BOOKMAN, FontSize, clrBK, intGR, Screen.TextWidth, TTF_STYLE_NORMAL, Story->TopImage);
                }
                else if (!Story->Text.empty())
                {
                    Text = Glyphs::FormattedText(Story->Text.c_str(), FONT_BOOKMAN, FontSize, clrBK, Screen.TextWidth);
                }

                auto Compact = (Text && Text->h <= Screen.TextBounds - 2 * text_space) || !Text;

                if (Story->Controls == Story::Controls::Standard)
                {
                    Controls = Story::StandardControls(Compact);
                }
                else
                {
                    Controls = Story::ExitControls();
                }

                auto ScrollSpeed = 20;
                auto Hold = false;
                auto Selected = false;
                auto Current = -1;
                auto Offset = 0;
                auto Transition = false;
                auto ScrollUp = false;
                auto ScrollDown = false;

                while (!Transition)
                {
                    Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, Controls, Text, Current, Offset);

                    RenderFlashMessage();

                    if (Current >= 0 && Current < Controls.size())
                    {
                        Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                    }

                    Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold);

                    if (((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold))
                    {
                        if (Controls[Current].Type == Control::Type::SCROLL_UP || (Controls[Current].Type == Control::Type::SCROLL_UP && Hold) || ScrollUp)
                        {
                            if (Text)
                            {
                                if (Offset > 0)
                                {
                                    Offset -= ScrollSpeed;
                                }

                                if (Offset < 0)
                                {
                                    Offset = 0;
                                }
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::SCROLL_DOWN || (Controls[Current].Type == Control::Type::SCROLL_DOWN && Hold) || ScrollDown)
                        {
                            if (Text)
                            {
                                if (Text->h >= (Screen.TextBounds))
                                {
                                    if (Offset < (Text->h - Screen.TextBounds))
                                    {
                                        Offset += ScrollSpeed;
                                    }

                                    if (Offset > (Text->h - Screen.TextBounds))
                                    {
                                        Offset = Text->h - Screen.TextBounds;
                                    }
                                }
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::ENCYCLOPEDIA && !Hold)
                        {
                            DisplayMessage("Not yet implemented!", intBK);

                            Selected = false;
                        }
                        else if (Controls[Current].Type == Control::Type::MAP && !Hold)
                        {
                            DisplayMessage("Not yet implemented!", intBK);

                            Selected = false;
                        }
                        else if (Controls[Current].Type == Control::Type::PARTY && !Hold)
                        {
                            auto Character = Party.Members.size() > 1 ? Interface::SelectAdventurer(Window, Renderer, Controls, Party, Story, Screen, Text, Offset, "Select adventurer to manage") : 0;

                            if (Character >= 0 && Character < Party.Members.size())
                            {
                                Interface::ManageAdventurer(Window, Renderer, Controls, Party, Story, Screen, Text, Offset, Character);
                            }

                            Current = -1;

                            Selected = false;
                        }
                        else if (Controls[Current].Type == Control::Type::GAME && !Hold)
                        {
                            DisplayMessage("Not yet implemented!", intBK);

                            Selected = false;
                        }
                        else if (Controls[Current].Type == Control::Type::CONTINUE && !Hold)
                        {
                            if (Story->Equipment.size() > 0)
                            {
                                Interface::TakeScreen(Window, Renderer, Controls, Party, Story, Screen, Text, Offset);
                            }

                            if (Story->Equipment.size() > 0)
                            {
                                continue;
                            }

                            // handle encumbrance
                            for (auto i = 0; i < Party.Members.size(); i++)
                            {
                                while (Party.Members[i].Equipment.size() > Party.Members[i].Encumbrance)
                                {
                                    Interface::ItemScreen(Window, Renderer, Controls, Party, Story, Screen, Text, Offset, i, Equipment::Mode::DROP);
                                }
                            }

                            auto Next = Interface::RenderChoices(Window, Renderer, Party, Story, Screen);

                            if (Next->Id != Story->Id || Story->Book != Next->Book)
                            {
                                Story = Next;

                                Transition = true;
                            }

                            Selected = false;
                        }
                        else if (Controls[Current].Type == Control::Type::EXIT && !Hold)
                        {
                            Transition = true;

                            Quit = true;
                        }
                    }
                }

                if (Text)
                {
                    SDL_FreeSurface(Text);

                    Text = NULL;
                }
            }
        }
    }

    void StoryScreen(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Engine::Destination Destination)
    {
        auto Story = Interface::FindStory(Destination);

        return ProcessStory(Window, Renderer, Party, Story);
    }
}
#endif