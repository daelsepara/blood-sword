#ifndef __INTERFACE_COMBAT_HPP__
#define __INTERFACE_COMBAT_HPP__

#include "interface_common.hpp"

namespace Interface
{
    // (player/enemy, id, awareness)
    typedef std::tuple<Map::Object, int, int> Combatants;

    // (player / enemy), id, distance, endurance
    typedef std::tuple<Map::Object, int, int, int> Targets;

    const int SortId = 1;
    const int SortDistance = 2;
    const int SortEndurance = 3;

    Interface::Targets EmptyTarget = {Map::Object::None, -1, -1, -1};

    // player id, location x, location y
    typedef std::tuple<int, int, int> TargetDestination;

    void RenderCombatScreen(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, int Current, Uint32 bg)
    {
        Graphics::FillWindow(Renderer, bg);

        Graphics::RenderButtons(Renderer, BattleScreen, Current, text_space, border_pts);
    }

    void RenderMessage(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Map::Base &Map, Uint32 bg, std::string Message, Uint32 FlashColor)
    {
        Uint32 Duration = 1500;

        auto MapSizeX = (Map.SizeX < 13 ? 13 : Map.SizeX) * Map.ObjectSize;

        auto MapSizeY = Map.SizeY * Map.ObjectSize;

        Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

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

    void GenerateMapControls(Map::Base &Map, std::vector<Button> &Controls, Party::Base &Party, std::vector<Enemy::Base> &Enemies, int NumControls)
    {
        auto StartMap = NumControls;

        Controls.erase(Controls.begin() + StartMap, Controls.end());

        if (Controls.size() > 3)
        {
            Controls[0].Highlight = Map.MapY > 0 ? intWH : intGR;
            Controls[1].Highlight = Map.MapX > 0 ? intWH : intGR;
            Controls[2].Highlight = (Map.MapX < Map.Width - Map.SizeX) ? intWH : intGR;
            Controls[3].Highlight = (Map.MapY < Map.Height - Map.SizeY) ? intWH : intGR;
        }

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
                    if (CtrlX < (StartMap - 4))
                    {
                        CtrlDn = CtrlX + 4;
                    }
                    else
                    {
                        CtrlDn = 4;
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

                Map::Tile &Tile = Map.Tiles[y][x];

                auto AssetX = Map.DrawX + (x - Map.MapX) * Map.ObjectSize;

                auto ObjectId = Tile.Id - 1;

                if (Tile.IsPlayer())
                {
                    Controls.push_back(Button(NumControls, Assets::Get(Party.Members[ObjectId].Asset), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intWH, Control::Type::PLAYER));
                }
                else if (Tile.IsEnemy())
                {
                    Controls.push_back(Button(NumControls, Enemies[ObjectId].Enthraled ? Assets::Get(Enemies[ObjectId].Asset, 0x66) : Assets::Get(Enemies[ObjectId].Asset), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intWH, Control::Type::ENEMY));
                }
                else if (Tile.IsExit())
                {
                    auto Asset = Tile.Asset;

                    if (Asset == Assets::Type::MapExit)
                    {
                        if (x == 0)
                        {
                            Asset = Assets::Type::MapExitW;
                        }
                        else if (x == Map.Width - 1)
                        {
                            Asset = Assets::Type::MapExitE;
                        }
                        else if (y == 0)
                        {
                            Asset = Assets::Type::MapExitN;
                        }
                        else if (y == Map.Height - 1)
                        {
                            Asset = Assets::Type::MapExitS;
                        }
                    }

                    Controls.push_back(Button(NumControls, Assets::Get(Asset), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intGR, Control::Type::MAP_EXIT));
                }
                else if (Tile.IsPassable)
                {
                    Controls.push_back(Button(NumControls, Assets::Get(Tile.Asset), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intGR, Control::Type::DESTINATION));
                }
                else if (Tile.IsPassableToEnemy)
                {
                    Controls.push_back(Button(NumControls, Assets::Get(Tile.Asset, 0x66), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intGR, Control::Type::MAP_NONE));
                }
                else
                {
                    Controls.push_back(Button(NumControls, Assets::Get(Tile.Asset), CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, intGR, Control::Type::MAP_NONE));
                }

                NumControls++;
            }
        }
    }

    bool AnimateMove(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 Bg, Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies, int SrcX, int SrcY, int DstX, int DstY)
    {
        // do not render off screen animations
        if (!Map.IsVisible(SrcX, SrcY) || !Map.IsVisible(DstX, DstY))
        {
            SDL_Delay(5);

            return Map.Move(SrcX, SrcY, DstX, DstY);
        }

        auto Sign = [&](int Value)
        {
            return Value >= 0 ? 1 : -1;
        };

        auto Draw = [&](SDL_Surface *Origin, SDL_Surface *Asset, int X, int Y, int Delay)
        {
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, Bg);

            Graphics::RenderImage(Renderer, Origin, Map.DrawX + (SrcX - Map.MapX) * Map.ObjectSize, Map.DrawY + (SrcY - Map.MapY) * Map.ObjectSize);

            Graphics::RenderImage(Renderer, Asset, X, Y);

            SDL_RenderPresent(Renderer);

            SDL_Delay(Delay);
        };

        auto Animate = [&](SDL_Surface *Origin, SDL_Surface *Asset)
        {
            auto DeltaX = (DstX - SrcX);

            auto DeltaY = (DstY - SrcY);

            // move along x
            for (auto i = 0; i < std::abs(DeltaX) * Map.ObjectSize; i += 5)
            {
                Draw(Origin, Asset, Map.DrawX + (SrcX - Map.MapX) * Map.ObjectSize + Sign(DeltaX) * i, Map.DrawY + (SrcY - Map.MapY) * Map.ObjectSize, 5);
            }

            // move along y, assumes movement along x was successful
            for (auto i = 0; i < std::abs(DeltaY) * Map.ObjectSize; i += 5)
            {
                Draw(Origin, Asset, Map.DrawX + (DstX - Map.MapX) * Map.ObjectSize, Map.DrawY + (SrcY - Map.MapY) * Map.ObjectSize + Sign(DeltaY) * i, 5);
            }
        };

        auto Result = false;

        if (Map.ValidX(SrcX) && Map.ValidY(SrcY) && Map.ValidX(DstX) && Map.ValidY(DstY))
        {
            if (Map.Tiles[SrcY][SrcX].IsPlayer() && (Map.Tiles[DstY][DstX].IsPassable || Map.Tiles[DstY][DstX].IsExit()) && !Map.Tiles[DstY][DstX].IsOccupied())
            {
                auto PlayerId = Map.Tiles[SrcY][SrcX].Id - 1;

                auto Origin = Assets::Copy(Map.Tiles[SrcY][SrcX].Asset);

                auto Asset = Assets::Copy(Party.Members[PlayerId].Asset);

                Animate(Origin, Asset);

                SDL_FreeSurface(Origin);

                SDL_FreeSurface(Asset);

                Result = Map.Move(SrcX, SrcY, DstX, DstY);
            }
            else if (Map.Tiles[SrcY][SrcX].IsEnemy() && (Map.Tiles[DstY][DstX].IsPassable || Map.Tiles[DstY][DstX].IsPassableToEnemy) && !Map.Tiles[DstY][DstX].IsOccupied())
            {
                auto EnemyId = Map.Tiles[SrcY][SrcX].Id - 1;

                auto Origin = Map.Tiles[SrcY][SrcX].IsPassableToEnemy ? Assets::Copy(Map.Tiles[SrcY][SrcX].Asset, 0x66) : Assets::Copy(Map.Tiles[SrcY][SrcX].Asset);

                auto Asset = Enemies[EnemyId].Enthraled ? Assets::Copy(Enemies[EnemyId].Asset, 0x66) : Assets::Copy(Enemies[EnemyId].Asset);

                Animate(Origin, Asset);

                SDL_FreeSurface(Origin);

                SDL_FreeSurface(Asset);

                Result = Map.Move(SrcX, SrcY, DstX, DstY);
            }
        }

        return Result;
    }

    bool FullMove(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies, AStar::Path &Path, int StartMap)
    {
        auto Result = false;

        if (Path.Points.size() > 0)
        {
            auto Limit = Path.Points.size();

            for (auto i = 1; i < Limit; i++)
            {
                auto Point = Path.Points[i - 1];

                auto Next = Path.Points[i];

                Result = Interface::AnimateMove(Renderer, BattleScreen, bg, Map, Party, Enemies, Point.X, Point.Y, Next.X, Next.Y);

                Interface::GenerateMapControls(Map, BattleScreen, Party, Enemies, StartMap);

                if (!Result)
                {
                    break;
                }
            }
        }

        return Result;
    }

    int Find(std::vector<Combatants> &Sequence, Map::Object object, int Id)
    {
        auto Found = -1;

        for (auto i = 0; i < Sequence.size(); i++)
        {
            if (std::get<0>(Sequence[i]) == object && std::get<1>(Sequence[i]) == Id)
            {
                Found = i;

                break;
            }
        }

        return Found;
    }

    int CountNearbyEnemies(Map::Base &Map, std::vector<Enemy::Base> &Enemies, int PlayerId, bool ShootMode)
    {
        auto Result = 0;

        for (auto i = 0; i < Enemies.size(); i++)
        {
            if (Engine::IsAlive(Enemies[i]) && Map.IsAdjacent(PlayerId, i) && ((ShootMode && !Enemies[i].Enthraled) || !ShootMode))
            {
                Result++;
            }
        }

        return Result;
    }

    int FirstNearbyEnemy(Map::Base &Map, std::vector<Enemy::Base> &Enemies, int PlayerId, bool ShootMode)
    {
        auto Result = 0;

        for (auto i = 0; i < Enemies.size(); i++)
        {
            if (Engine::IsAlive(Enemies[i]) && Map.IsAdjacent(PlayerId, i) && ((ShootMode && !Enemies[i].Enthraled) || !ShootMode))
            {
                Result = i;

                break;
            }
        }

        return Result;
    }

    bool NearbyEnemies(Map::Base &Map, std::vector<Enemy::Base> &Enemies, int PlayerId, bool ShootMode)
    {
        return Interface::CountNearbyEnemies(Map, Enemies, PlayerId, ShootMode) > 0;
    }

    bool NearbyOpponents(Map::Base &Map, std::vector<Enemy::Base> &Enemies, int EnemyId, bool ShootMode)
    {
        auto Result = false;

        for (auto i = 0; i < Enemies.size(); i++)
        {
            Result |= (i != EnemyId && Engine::IsAlive(Enemies[i]) && Map.IsAdjacent(EnemyId, Map::Object::Enemy, i, Map::Object::Enemy) && ((ShootMode && !Enemies[i].Enthraled) || !ShootMode));
        }

        return Result;
    }

    bool EnemyAdjacent(Map::Base &Map, std::vector<Enemy::Base> &Enemies, Character::Base &character, int PlayerId)
    {
        auto isEnemyAdjacent = false;

        for (auto i = 0; i < Enemies.size(); i++)
        {
            Enemy::Base &Enemy = Enemies[i];

            if (Engine::IsAlive(Enemy) && Map.IsAdjacent(PlayerId, i))
            {
                isEnemyAdjacent = true;

                break;
            }
        }

        return isEnemyAdjacent;
    }

    // TODO: Deprecated in 2014 version
    bool AttackedWhileMoving(Map::Base &Map, std::vector<Enemy::Base> &Enemies, Character::Base &character, int PlayerId, int &Damages)
    {
        auto WasAttacked = false;

        Damages = 0;

        for (auto i = 0; i < Enemies.size(); i++)
        {
            Enemy::Base &Enemy = Enemies[i];

            if (Engine::IsAlive(Enemy) && Map.IsAdjacent(PlayerId, i) && Enemy.Awareness >= Engine::Awareness(character) && Enemy.Attacked == PlayerId && !character.ActFirst)
            {
                WasAttacked = true;

                Damages -= 1;
            }
        }

        return WasAttacked;
    }

    void AttackedWhileFleeing(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Map::Base &Map, Uint32 bg, Character::Base &Character, int Damage)
    {
        Interface::RenderMessage(Renderer, BattleScreen, Map, bg, std::string(Character::ClassName[Character.Class]) + " shot while attempting to flee!", intBK);

        Interface::RenderMessage(Renderer, BattleScreen, Map, bg, std::string(Character::ClassName[Character.Class]) + " dealt " + std::to_string(Damage) + " damage!", intBK);

        Engine::Gain(Character, -Damage);
    }

    void DrawPath(SDL_Renderer *Renderer, Map::Base &Map, AStar::Path &CurrentPath, int CurrentMove, Uint32 color, Uint8 alpha)
    {
        if (CurrentMove > 0 && CurrentMove < CurrentPath.Points.size())
        {
            auto TargetX = CurrentPath.Points.back().X;

            auto TargetY = CurrentPath.Points.back().Y;

            SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);

            auto Color = O(color, alpha);

            for (auto i = CurrentMove; i < CurrentPath.Points.size() - 1; i++)
            {
                auto X = CurrentPath.Points[i].X;

                auto Y = CurrentPath.Points[i].Y;

                if (Map.IsVisible(X, Y))
                {
                    Graphics::FillRect(Renderer, Map.ObjectSize, Map.ObjectSize, Map.DrawX + (X - Map.MapX) * Map.ObjectSize, Map.DrawY + (Y - Map.MapY) * Map.ObjectSize, Color);
                }
            }

            SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_NONE);

            if (Map.IsVisible(TargetX, TargetY))
            {
                Graphics::ThickRect(Renderer, Map.ObjectSize - 4 * border_pts, Map.ObjectSize - 4 * border_pts, Map.DrawX + (TargetX - Map.MapX) * Map.ObjectSize + 2 * border_pts, Map.DrawY + (TargetY - Map.MapY) * Map.ObjectSize + 2 * border_pts, color, border_pts);
            }
        }
    }

    void SortTargets(std::vector<Interface::Targets> &Distances, int SortMode)
    {
        if (SortMode == Interface::SortDistance)
        {
            // sort players based on distance, endurance
            std::sort(Distances.begin(), Distances.end(), [](Interface::Targets &a, Interface::Targets &b) -> bool
                      {
                          if (std::get<2>(a) < std::get<2>(b))
                          {
                              return true;
                          }
                          else if (std::get<2>(a) == std::get<2>(b))
                          {
                              // on equidistant targets, give priority to the one with less endurance
                              if (std::get<3>(a) < std::get<3>(b))
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
        else if (SortMode == Interface::SortEndurance)
        {
            // sort players based on endurance, distance
            std::sort(Distances.begin(), Distances.end(), [](Interface::Targets &a, Interface::Targets &b) -> bool
                      {
                          if (std::get<3>(a) < std::get<3>(b))
                          {
                              return true;
                          }
                          else if (std::get<3>(a) == std::get<3>(b))
                          {
                              // on equal endurance, give priority to the nearest target
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
        else
        {
            // sort players based on Id, distance
            std::sort(Distances.begin(), Distances.end(), [](Interface::Targets &a, Interface::Targets &b) -> bool
                      {
                          if (std::get<1>(a) < std::get<1>(b))
                          {
                              return true;
                          }
                          else if (std::get<1>(a) == std::get<1>(b))
                          {
                              // on equal Id targets, give priority to the nearest target
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
    }

    std::vector<Interface::Targets> CycleTargets(Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies, int EnemyId, int EnemyX, int EnemyY, bool ignore)
    {
        auto Distances = std::vector<Interface::Targets>();

        // cycle through the players
        for (auto i = 0; i < Party.Members.size(); i++)
        {
            if (Engine::IsAlive(Party.Members[i]) && !Party.Members[i].Escaped && !Party.Members[i].Paralyzed && !Party.Members[i].Away)
            {
                auto LocationX = 0;

                auto LocationY = 0;

                Map.Find(Map::Object::Player, i, LocationX, LocationY);

                auto TempPath = AStar::FindPath(Map, EnemyX, EnemyY, LocationX, LocationY, ignore);

                if (TempPath.Points.size() > 0)
                {
                    auto Distance = Map.Distance(EnemyX, EnemyY, LocationX, LocationY);

                    Distances.push_back({Map::Object::Player, i, ignore ? (Distance + Map.SizeX * Map.SizeY) : Distance, Engine::Endurance(Party.Members[i])});
                }
            }
        }

        if (EnemyId >= 0 && EnemyId < Enemies.size())
        {
            // cycle through the enemies
            for (auto i = 0; i < Enemies.size(); i++)
            {
                if (Engine::IsAlive(Enemies[i]) && Enemies[i].Type != Enemies[EnemyId].Type && EnemyId != i)
                {
                    auto LocationX = 0;

                    auto LocationY = 0;

                    Map.Find(Map::Object::Enemy, i, LocationX, LocationY);

                    auto TempPath = AStar::FindPath(Map, EnemyX, EnemyY, LocationX, LocationY, ignore);

                    if (TempPath.Points.size() > 0)
                    {
                        auto Distance = Map.Distance(EnemyX, EnemyY, LocationX, LocationY);

                        Distances.push_back({Map::Object::Enemy, i, ignore ? (Distance + Map.SizeX * Map.SizeY) : Distance, Enemies[i].Endurance});
                    }
                }
            }
        }

        return Distances;
    }

    Interface::Targets SelectAllTargets(Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies, int EnemyId, int SortMode, bool Ignore)
    {
        Interface::Targets NearestTarget = Interface::EmptyTarget;

        // (player/enemy), id, distance, endurance
        std::vector<Interface::Targets> Distances = {};

        auto EnemyX = -1;

        auto EnemyY = -1;

        Map.Find(Map::Object::Enemy, EnemyId, EnemyX, EnemyY);

        Distances = Interface::CycleTargets(Map, Party, Enemies, EnemyId, EnemyX, EnemyY, Ignore);

        if (Distances.size() > 0)
        {
            // sort players based on distance and endurance
            Interface::SortTargets(Distances, SortMode);

            NearestTarget = Distances.front();
        }
        else
        {
            Distances = Interface::CycleTargets(Map, Party, Enemies, EnemyId, EnemyX, EnemyY, true);

            if (Distances.size() > 0)
            {
                // sort players based on distance and endurance
                Interface::SortTargets(Distances, SortMode);

                NearestTarget = Distances.front();
            }
        }

        return NearestTarget;
    }

    Interface::Targets SelectTarget(Map::Base &Map, Party::Base &Party, int EnemyId, int SortMode, bool Ignore)
    {
        auto EmptyEnemies = std::vector<Enemy::Base>();

        return SelectAllTargets(Map, Party, EmptyEnemies, EnemyId, SortMode, Ignore);
    }

    Interface::Targets BowmanTarget(Map::Base &Map, Party::Base &Party, int EnemyId)
    {
        // (player/enemy), id, distance, endurance
        Interface::Targets NearestTarget = Interface::EmptyTarget;

        std::vector<Interface::Targets> Distances = {};

        auto EnemyX = -1;

        auto EnemyY = -1;

        Map.Find(Map::Object::Enemy, EnemyId, EnemyX, EnemyY);

        auto ValidEnemyX = Map.ValidX(EnemyX);

        auto ValidEnemyY = Map.ValidY(EnemyY);

        auto PlayerX = -1;

        auto PlayerY = -1;

        if (Engine::Count(Party) == 1 || Engine::Remaining(Party) == 1)
        {
            auto PlayerId = Engine::First(Party);

            Map.Find(Map::Object::Player, PlayerId, PlayerX, PlayerY);

            auto ValidX = ValidEnemyX && Map.ValidX(PlayerX);

            auto ValidY = ValidEnemyY && Map.ValidY(PlayerY);

            if (ValidX && ValidY)
            {
                NearestTarget = {Map::Object::Player, PlayerId, Map.Distance(EnemyX, EnemyY, PlayerX, PlayerY), Engine::Endurance(Party.Members[PlayerId])};
            }
        }
        else
        {
            for (auto i = 0; i < Party.Members.size(); i++)
            {
                if (Engine::IsAlive(Party.Members[i]) && Party.Members[i].Engaged && !Party.Members[i].Escaped)
                {
                    Map.Find(Map::Object::Player, i, PlayerX, PlayerY);

                    auto ValidX = ValidEnemyX && Map.ValidX(PlayerX);

                    auto ValidY = ValidEnemyY && Map.ValidY(PlayerY);

                    if (ValidX && ValidY)
                    {
                        Distances.push_back({Map::Object::Player, i, Map.Distance(EnemyX, EnemyY, PlayerX, PlayerY), Engine::Endurance(Party.Members[i])});
                    }
                }
            }

            if (Distances.size() > 1)
            {
                Interface::SortTargets(Distances, Interface::SortEndurance);

                NearestTarget = Distances.front();
            }
            else if (Distances.size() == 1)
            {
                NearestTarget = Distances.front();
            }
        }

        return NearestTarget;
    }

    void SortLocations(std::vector<Interface::TargetDestination> &Distances)
    {
        // sort players based on distance
        std::sort(Distances.begin(), Distances.end(), [](Interface::TargetDestination &a, Interface::TargetDestination &b) -> bool
                  {
                      if (std::get<2>(a) < std::get<2>(b))
                      {
                          return true;
                      }
                      else
                      {
                          return false;
                      }
                  });
    }

    bool CloseDistance(Map::Base &Map, int SrcX, int SrcY, int &DstX, int &DstY)
    {
        std::vector<std::pair<int, int>> Neighbors = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};

        auto Result = false;

        std::vector<Interface::TargetDestination> Locations = {};

        for (auto i = 0; i < Neighbors.size(); i++)
        {
            auto TargetX = DstX + Neighbors[i].first;

            auto TargetY = DstY + Neighbors[i].second;

            if (Map.ValidX(TargetX) && Map.ValidY(TargetY) && (Map.Tiles[TargetY][TargetX].IsPassable || Map.Tiles[TargetY][TargetX].IsPassableToEnemy) && !Map.Tiles[TargetY][TargetX].IsOccupied())
            {
                auto TempPath = AStar::FindPath(Map, SrcX, SrcY, TargetX, TargetY);

                if (TempPath.Points.size() > 0)
                {
                    Locations.push_back({TargetX, TargetY, Map.Distance(DstX, DstY, TargetX, TargetY)});
                }
            }
        }

        if (Locations.size() > 0)
        {
            Interface::SortLocations(Locations);

            DstX = std::get<0>(Locations.front());

            DstY = std::get<1>(Locations.front());

            Result = true;
        }

        return Result;
    }

    void ShowCoordinates(SDL_Renderer *Renderer, Map::Object Type, int X, int Y, TTF_Font *Font, int TextW, int TextX, int TextY)
    {
        auto FontSize = TTF_FontHeight(Font);

        std::string Coordinates = "(" + std::to_string(X) + ", " + std::to_string(Y) + ")";

        Graphics::PutText(Renderer, Coordinates.c_str(), Font, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY);

        Graphics::PutText(Renderer, Map::Description[Type], Font, text_space, clrWH, intBK, TTF_STYLE_NORMAL, TextW, FontSize, TextX, TextY + (FontSize + text_space));
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

        if (Party.Members[PlayerId].Defending)
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

        auto LocationX = -1;

        auto LocationY = -1;

        Map.Find(Map::Object::Player, PlayerId, LocationX, LocationY);

        if (Map.ValidX(LocationX) && Map.ValidY(LocationY))
        {
            Interface::ShowCoordinates(Renderer, Map.Tiles[LocationY][LocationX].Type, LocationX, LocationY, Font, Map.TextRightWidth, Map.TextRightX - text_space, Map.DrawY + TextOffset * (FontSize + 2));

            TextOffset += 2;
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

        auto LocationX = -1;

        auto LocationY = -1;

        Map.Find(Map::Object::Enemy, EnemyId, LocationX, LocationY);

        if (Map.ValidX(LocationX) && Map.ValidY(LocationY))
        {
            Interface::ShowCoordinates(Renderer, Map.Tiles[LocationY][LocationX].Type, LocationX, LocationY, Font, Map.TextRightWidth, Map.TextRightX - text_space, Map.DrawY + RowOffset * (FontSize + 2));

            RowOffset += 2;
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
                          else if (std::get<0>(a) == Map::Object::Enemy && std::get<0>(b) == Map::Object::Player)
                          {
                              return false;
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

        Map.Find(Map::Object::Enemy, EnemyId, EnemyX, EnemyY);

        auto TotalDamage = std::max(0, Damage - (UseArmour ? Enemies[EnemyId].Armour : 0));

        Engine::Gain(Enemies[EnemyId], -TotalDamage);

        if (!Engine::IsAlive(Enemies[EnemyId]))
        {
            Map.Remove(EnemyX, EnemyY);
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

        SDL_Surface *Dice[6];

        Dice[0] = Assets::Copy(Assets::Type::Dice1);
        Dice[1] = Assets::Copy(Assets::Type::Dice2);
        Dice[2] = Assets::Copy(Assets::Type::Dice3);
        Dice[3] = Assets::Copy(Assets::Type::Dice4);
        Dice[4] = Assets::Copy(Assets::Type::Dice5);
        Dice[5] = Assets::Copy(Assets::Type::Dice6);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<TextButton> &Controls = ResistControls;

        auto Done = false;

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

            if (!IsEnemy && Weapons.empty() && !Engine::HasAbility(Character, Abilities::Type::UnarmedMartialArts))
            {
                AttributeValue -= 2;
            }
        }
        else if (Attribute == Attributes::Type::PsychicAbility)
        {
            AttributeValue = IsEnemy ? Enemy.PsychicAbility : (Engine::PsychicAbility(Character) + (Equipment.size() > 0 ? Equipment[0].Score : 0) - Character.Spells.size());
        }
        else if (Attribute == Attributes::Type::Awareness)
        {
            AttributeValue = IsEnemy ? Enemy.Awareness : (Engine::Awareness(Character) + (Equipment.size() > 0 ? Equipment[0].Score : 0));
        }

        AttributeValue = std::max(0, AttributeValue);

        while (!Done)
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

            Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + ": " + std::to_string(AttributeValue) + (Attribute == Attributes::Type::PsychicAbility && !IsEnemy && Character.Spells.size() > 0 ? (" (-" + std::to_string(Character.Spells.size()) + ")") : "")).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 3 * RowHeight);

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
                    Graphics::StretchImage(Renderer, Dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + 6 * RowHeight, Map.ObjectSize, Map.ObjectSize);
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
                    Done = true;
                }
            }
        }

        for (auto i = 0; i < 6; i++)
        {
            if (Dice[i])
            {
                SDL_FreeSurface(Dice[i]);

                Dice[i] = NULL;
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

        SDL_Surface *Dice[6];

        Dice[0] = Assets::Copy(Assets::Type::Dice1);
        Dice[1] = Assets::Copy(Assets::Type::Dice2);
        Dice[2] = Assets::Copy(Assets::Type::Dice3);
        Dice[3] = Assets::Copy(Assets::Type::Dice4);
        Dice[4] = Assets::Copy(Assets::Type::Dice5);
        Dice[5] = Assets::Copy(Assets::Type::Dice6);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<TextButton> &Controls = ResistControls;

        auto Done = false;

        auto CurrentStage = Attributes::Stage::START;

        Engine::Randomize();

        std::vector<int> TestRolls(Rolls, 0);

        auto TestSum = 0;

        while (!Done)
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
                    Graphics::StretchImage(Renderer, Dice[TestRolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + 6 * RowHeight, Map.ObjectSize, Map.ObjectSize);
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
                    Done = true;
                }
            }
        }

        for (auto i = 0; i < 6; i++)
        {
            if (Dice[i])
            {
                SDL_FreeSurface(Dice[i]);

                Dice[i] = NULL;
            }
        }

        return Result;
    }

    int Choose(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, std::vector<std::string> Labels, const char *Message)
    {
        auto Result = -1;

        auto MapSizeX = (Map.SizeX < 15 ? 15 : Map.SizeX) * Map.ObjectSize;
        auto MapSizeY = (Map.SizeY < 4 ? 4 : Map.SizeY) * Map.ObjectSize;
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

        auto TextButtonX = WindowX + 2 * text_space;
        auto TextButtonY = (WindowY + WindowH) - (text_buttonh + 2 * text_space);

        auto Controls = Graphics::CreateFixedTextButtons(Labels, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);

        for (auto i = 0; i < Controls.size(); i++)
        {
            Controls[i].Fg = clrWH;
            Controls[i].Color = intBK;
            Controls[i].Highlight = intGR;
            Controls[i].Type = Control::Type::CHOICE;
        }

        auto Done = false;

        while (!Done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, Message, Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, WindowW - 4 * text_space, TTF_FontHeight(Fonts::Normal), WindowButtonX - text_space, WindowY + text_space);

            Graphics::RenderTextButtons(Renderer, Controls, Fonts::Normal, Current, TTF_STYLE_NORMAL);

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::CHOICE)
                {
                    Result = Current;

                    Done = true;
                }
            }
        }

        return Result;
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

        auto Done = false;

        while (!Done)
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

                    Done = true;
                }
                else
                {
                    Result = Current;

                    Done = true;
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

                        Map.Find(Map::Object::Player, i, PlayerX, PlayerY);

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

                                    if (Map.ValidX(X) && Map.ValidY(Y) && Map.Tiles[Y][X].Occupant == Map::Object::None)
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

            for (auto i = 0; i < Enemies.size(); i++)
            {
                if (Engine::IsAlive(Enemies[i]))
                {
                    auto Damage = Engine::Roll(DamageRolls, DamageModifier);

                    Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[i].Name + " dealt " + std::to_string(Damage) + " damage!"), intGR);

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

            Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[EnemyId].Name + " dealt " + std::to_string(Damage) + " damage!"), intGR);

            Interface::DealDamage(Map, Enemies, EnemyId, Damage, true);

            if (!Engine::IsAlive(Enemies[EnemyId]))
            {
                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[EnemyId].Name + " killed!", intGR);
            }
        }
        else if (Spell == Spell::Type::MistsOfDeath)
        {
            for (auto i = 0; i < Enemies.size(); i++)
            {
                if (Engine::IsAlive(Enemies[i]))
                {
                    auto Result = Interface::Test(Renderer, BattleScreen, bg, Map, Party.Members[PlayerId], Enemies[i], Attributes::Type::PsychicAbility, true);

                    if (Result == Attributes::Result::FAILURE)
                    {
                        auto Damage = Engine::Roll(2, 0);

                        Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[i].Name + " dealt " + std::to_string(Damage) + " damage!").c_str(), intGR);

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

                    Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[EnemyId].Name + " dealt " + std::to_string(Damage) + " damage!").c_str(), intGR);

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

                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, (Enemies[EnemyId].Name + " dealt " + std::to_string(Damage) + " damage!").c_str(), intGR);

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

                    Party.Members[PlayerId].SpellStatus.push_back({Spell::Type::EyeOfTheTiger, CombatRound, 5, 2});
                }
            }
            else if (Result == 1)
            {
                Interface::RenderMessage(Renderer, BattleScreen, Map, bg, "Party gains +1 Fighting Prowess and +1 to Damage!", intGR);

                for (auto i = 0; i < Party.Members.size(); i++)
                {
                    if (Engine::IsAlive(Party.Members[i]))
                    {
                        if (!Engine::HasStatus(Party.Members[i], Spell::Type::EyeOfTheTiger))
                        {
                            Party.Members[i].SpellStatus.push_back({Spell::Type::EyeOfTheTiger, CombatRound, i == PlayerId ? 5 : 4, 1});
                        }
                    }
                }
            }
        }
        else if (Spell == Spell::Type::ServileEnthralment)
        {
            if (!Enemies[EnemyId].Enthraled)
            {
                if (Enemies[EnemyId].Type == Enemy::Type::IconTheUngodly)
                {
                    Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[EnemyId].Name + " is immune to entrhalment!", intBK);
                }
                else
                {
                    Interface::RenderMessage(Renderer, BattleScreen, Map, bg, Enemies[EnemyId].Name + " is now under your control!", intGR);

                    Enemies[EnemyId].Enthraled = true;
                }
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

        SDL_Surface *Dice[6];

        Dice[0] = Assets::Copy(Assets::Type::Dice1);
        Dice[1] = Assets::Copy(Assets::Type::Dice2);
        Dice[2] = Assets::Copy(Assets::Type::Dice3);
        Dice[3] = Assets::Copy(Assets::Type::Dice4);
        Dice[4] = Assets::Copy(Assets::Type::Dice5);
        Dice[5] = Assets::Copy(Assets::Type::Dice6);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<TextButton> &Controls = SpellControls;

        auto Done = false;

        auto CurrentStage = Spell::Stage::START;

        if (Character.Defending)
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

        while (!Done)
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
                    Graphics::StretchImage(Renderer, Dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + 6 * RowHeight, Map.ObjectSize, Map.ObjectSize);
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
                    if (Result != Spell::Result::NONE)
                    {
                        Character.Engaged = true;
                    }

                    Done = true;
                }
            }
        }

        for (auto i = 0; i < 6; i++)
        {
            if (Dice[i])
            {
                SDL_FreeSurface(Dice[i]);

                Dice[i] = NULL;
            }
        }

        if (Result == Spell::Result::FAILURE)
        {
            Spell.Difficulty--;
        }

        return Result;
    }

    // fight/shoot encounter between player and enemy
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

        SDL_Surface *Dice[6];

        Dice[0] = Assets::Copy(Assets::Type::Dice1);
        Dice[1] = Assets::Copy(Assets::Type::Dice2);
        Dice[2] = Assets::Copy(Assets::Type::Dice3);
        Dice[3] = Assets::Copy(Assets::Type::Dice4);
        Dice[4] = Assets::Copy(Assets::Type::Dice5);
        Dice[5] = Assets::Copy(Assets::Type::Dice6);

        auto Fight = Assets::Copy(Assets::Type::Fight);

        auto Shoot = Attacked && Enemy.Type == Enemy::Type::NinjaAssassin ? Assets::Copy(Assets::Type::Shuriken) : Assets::Copy(Assets::Type::Shoot);

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
            auto SteelSceptre = false;
            auto HasSteelSceptre = !Attacked && Engine::HasItem(Character, Item::Type::SteelSceptre, 1);

            auto CurrentStage = Combat::Stage::START;
            auto Weapons = Engine::Weapons(Character);
            auto Equipment = Engine::Equipment(Character, Attributes::Type::FightingProwess, false);
            auto FightingProwess = Engine::FightingProwess(Character) + (Weapons.size() > Round ? Weapons[Round].Score : 0) + (Equipment.size() > 0 ? Equipment[0].Score : 0);
            auto Damage = Character.Damage;
            auto DamageModifier = Character.DamageModifier + (Weapons.size() > Round ? Weapons[Round].Damage : 0) + (Equipment.size() > 0 ? Equipment[0].Damage : 0);
            auto Armour = Engine::Armour(Character);

            if (Engine::HasCharge(Character, Item::Type::RingOfWarding, 1))
            {
                Armour += 1;
            }

            DamageModifier = FightMode == Combat::FightMode::SHOOT ? 0 : DamageModifier;

            if (!Attacked && Engine::HasStatus(Character, Spell::Type::EyeOfTheTiger))
            {
                auto EyeResult = Engine::GetStatus(Character, Spell::Type::EyeOfTheTiger);

                if (EyeResult >= 0 && EyeResult < Character.SpellStatus.size())
                {
                    FightingProwess += std::get<3>(Character.SpellStatus[EyeResult]);

                    DamageModifier += std::get<3>(Character.SpellStatus[EyeResult]);
                }
            }

            if (Weapons.size() < Round && !Engine::HasAbility(Character, Abilities::Type::UnarmedMartialArts) && FightMode != Combat::FightMode::SHOOT)
            {
                FightingProwess = std::max(0, FightingProwess - 2);

                DamageModifier -= 2;
            }

            if (Character.Defending && !Attacked)
            {
                Controls = DoneControls;

                CurrentStage = Combat::Stage::END;
            }

            auto FightRolls = 2;

            FightRolls += (Attacked && Character.Defending) ? 1 : 0;
            FightRolls += (Attacked && Engine::HasAbility(Character, Abilities::Type::Dodging)) ? 1 : 0;
            FightRolls += (Attacked && Engine::HasStatus(Enemy, Spell::Type::Nighthowl)) ? 1 : 0;

            auto DamageRolls = FightMode == Combat::FightMode::SHOOT ? 1 : (Attacked ? Enemy.Damage : Damage);

            std::vector<int> Rolls(FightRolls, 0);
            std::vector<int> Damages(DamageRolls, 0);

            auto DamageSum = 0;
            auto FightingSum = 0;
            auto CalculatedDamage = false;
            auto AssignedDamage = false;
            auto Retribution = false;
            auto Poisoned = false;
            auto Done = false;

            Result = Combat::Result::NONE;

            while (!Done)
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
                Graphics::PutText(Renderer, ("DMG: " + (FightMode == Combat::FightMode::SHOOT ? "1D" : (std::to_string(Damage) + "D")) + (DamageModifier < 0 ? "" : "+") + std::to_string(DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 3 * RowHeight);
                Graphics::PutText(Renderer, ("ARM: " + std::to_string(Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? MidWindow : TextButtonX, TextY + 4 * RowHeight);

                auto StatusOffset = 5;

                if (Attacked && Character.Defending)
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

                // Enemy stats
                Graphics::PutText(Renderer, Enemy.Name.c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY);
                Graphics::PutText(Renderer, ("FPR: " + std::to_string(Enemy.FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + RowHeight);
                Graphics::PutText(Renderer, ("END: " + std::to_string(Enemy.Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 2 * RowHeight);

                if (Attacked && FightMode == Combat::FightMode::SHOOT)
                {
                    if (Enemy.Type == Enemy::Type::NinjaAssassin)
                    {
                        Graphics::PutText(Renderer, "DMG: 1D-1", Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 3 * RowHeight);
                    }
                    else
                    {
                        Graphics::PutText(Renderer, ("DMG: " + std::to_string(Enemy.Damage) + "D" + (Enemy.DamageModifier < 0 ? "" : "+") + std::to_string(Enemy.DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 3 * RowHeight);
                    }
                }
                else
                {
                    Graphics::PutText(Renderer, ("DMG: " + std::to_string(Enemy.Damage) + "D" + (Enemy.DamageModifier < 0 ? "" : "+") + std::to_string(Enemy.DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 3 * RowHeight);
                }

                Graphics::PutText(Renderer, ("ARM: " + std::to_string(Enemy.Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, Attacked ? TextButtonX : MidWindow, TextY + 4 * RowHeight);

                if (FightMode == Combat::FightMode::FIGHT)
                {
                    Graphics::RenderImage(Renderer, Fight, MidWindow - (3 * Map.ObjectSize / 2 + border_pts), WindowY + Map.ObjectSize);
                }
                else if (FightMode == Combat::FightMode::SHOOT)
                {
                    Graphics::RenderImage(Renderer, Shoot, MidWindow - (3 * Map.ObjectSize / 2 + border_pts), WindowY + Map.ObjectSize);
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
                    if (!SteelSceptre)
                    {
                        // show fight results
                        for (auto i = 0; i < FightRolls; i++)
                        {
                            Graphics::StretchImage(Renderer, Dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (RowOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                        }

                        Graphics::PutText(Renderer, ((FightMode != Combat::FightMode::SHOOT ? "Fight Score: " : "Shooting Score: ") + std::to_string(FightingSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (RowOffset + 4) * RowHeight);

                        if (Attacked)
                        {
                            Graphics::PutText(Renderer, (Enemy.Name + " hits the " + std::string(Character::ClassName[Character.Class]) + "!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (RowOffset + 5) * RowHeight);
                        }
                        else
                        {
                            Graphics::PutText(Renderer, ("The " + std::string(Character::ClassName[Character.Class]) + " hits " + Enemy.Name + "!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (RowOffset + 5) * RowHeight);
                        }
                    }
                    else
                    {
                        Graphics::PutText(Renderer, ("Bolts from the steel sceptre blasts the " + Enemy.Name + "!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (RowOffset + 5) * RowHeight);
                    }

                    if (!CalculatedDamage)
                    {
                        DamageSum = 0;

                        if (Quarterstaff && !Attacked)
                        {
                            DamageRolls += 1;

                            Rolls.resize(DamageRolls, 0);
                        }

                        if (SteelSceptre)
                        {
                            Engine::Discharge(Character, Item::Type::SteelSceptre, 1);

                            DamageRolls = 5;

                            Rolls.resize(DamageRolls, 0);
                        }

                        // compute damage
                        for (auto i = 0; i < DamageRolls; i++)
                        {
                            Damages[i] = Engine::Roll(1, 0);

                            DamageSum += Damages[i];
                        }

                        DamageSum += FightMode == Combat::FightMode::SHOOT ? 0 : (Attacked ? Enemy.DamageModifier : DamageModifier);

                        if (Attacked)
                        {
                            if (Enemy.Type == Enemy::Type::NinjaAssassin && FightMode == Combat::FightMode::SHOOT)
                            {
                                DamageSum -= 1;
                            }

                            if (Enemy.Type != Enemy::Type::Skiapyr)
                            {
                                DamageSum -= Armour;

                                if (DamageSum > 0)
                                {
                                    if (Engine::HasCharge(Character, Item::Type::RingOfWarding, 1))
                                    {
                                        Engine::Discharge(Character, Item::Type::RingOfWarding, 1);
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (!SteelSceptre)
                            {
                                DamageSum -= Enemy.Armour;
                            }
                        }

                        DamageSum = std::max(0, DamageSum);

                        if (!Attacked && Quarterstaff && DamageSum > 0)
                        {
                            Enemy.KnockedOff = true;

                            Result = Combat::Result::KNOCKED_OFF;
                        }

                        if (!Attacked && FightMode != Combat::FightMode::SHOOT && DamageSum > 0 && Enemy.Type == Enemy::Type::IconTheUngodly)
                        {
                            Retribution = true;
                        }

                        if (Attacked && FightMode != Combat::FightMode::SHOOT && DamageSum > 0 && Enemy.Type == Enemy::Type::Echidna)
                        {
                            if (Engine::Roll(1, 0) == 6)
                            {
                                Poisoned = true;
                            }
                        }

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
                            Graphics::StretchImage(Renderer, Dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (RowOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                        }

                        Graphics::PutText(Renderer, ("Fight Score: " + std::to_string(FightingSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (RowOffset + 4) * RowHeight);

                        if (Attacked)
                        {
                            Graphics::PutText(Renderer, (Enemy.Name + "'s attack was unsuccessful!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (RowOffset + 5) * RowHeight);
                        }
                        else
                        {
                            Graphics::PutText(Renderer, ("The " + std::string(Character::ClassName[Character.Class]) + "'s attack was unsuccessful!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (RowOffset + 5) * RowHeight);
                        }
                    }
                    else
                    {
                        // show damage results
                        for (auto i = 0; i < DamageRolls; i++)
                        {
                            Graphics::StretchImage(Renderer, Dice[Damages[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (RowOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                        }

                        std::string DamageString = "";

                        if (Attacked)
                        {
                            if (Enemy.Type == Enemy::Type::Skiapyr)
                            {
                                DamageString = "Damage Dealt: ";
                            }
                            else
                            {
                                DamageString = "Damage Dealt (-Armour): ";
                            }
                        }
                        else
                        {
                            if (SteelSceptre)
                            {
                                DamageString = "Damage Dealt: ";
                            }
                            else
                            {
                                DamageString = "Damage Dealt (-Armour): ";
                            }
                        }

                        Graphics::PutText(Renderer, (DamageString + std::to_string(DamageSum)).c_str(), Fonts::Normal, 0, Attacked ? clrGR : clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (RowOffset + 4) * RowHeight);

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
                        if (HasSteelSceptre)
                        {
                            auto Choice = Interface::Choose(Renderer, BattleScreen, bg, Map, {"YES", "NO"}, "Use the steel sceptre?");

                            if (Choice == 0)
                            {
                                CurrentStage = Combat::Stage::DAMAGE;

                                Result = Combat::Result::FIGHT;

                                Controls = DamageControls;

                                SteelSceptre = true;

                                Damage = 5;

                                DamageModifier = 0;
                            }
                            else
                            {
                                CurrentStage = Combat::Stage::FIGHT;

                                Controls = DamageControls;
                            }
                        }
                        else
                        {
                            CurrentStage = Combat::Stage::FIGHT;

                            Controls = DamageControls;
                        }
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
                        if (Attacked && Enemy.Type == Enemy::Type::MagusVyl && Character.Class != Character::Class::Sage && Engine::IsAlive(Character) && Result == Combat::Result::FIGHT)
                        {
                            auto TestResult = Interface::Test(Renderer, BattleScreen, bg, Map, Character, Enemy, Attributes::Type::PsychicAbility, false);

                            if (TestResult == Attributes::Result::FAILURE)
                            {
                                Character.Paralyzed = true;
                            }
                        }
                        else if (Attacked && Enemy.Type == Enemy::Type::Echidna && Poisoned)
                        {
                            auto PoisonDamage = Engine::Roll(3, 0);

                            RenderMessage(Renderer, BattleScreen, Map, bg, "Poison flows into your wounds for " + std::to_string(PoisonDamage) + " damage!", intBK);

                            Engine::Gain(Character, -PoisonDamage);
                        }
                        else if (!Attacked && Retribution)
                        {
                            RenderMessage(Renderer, BattleScreen, Map, bg, "Retribution spell retaliates for 1 damage!", intBK);

                            Engine::Gain(Character, -1);
                        }

                        if (Result != Combat::Result::NONE)
                        {
                            Character.Engaged = true;
                        }

                        Done = true;
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
            if (Dice[i])
            {
                SDL_FreeSurface(Dice[i]);

                Dice[i] = NULL;
            }
        }

        SDL_FreeSurface(Fight);

        Fight = NULL;

        SDL_FreeSurface(Shoot);

        Shoot = NULL;

        return Result;
    }

    // fight encounter between enemies
    Combat::Result Fight(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 bg, Map::Base &Map, Enemy::Base &Attacker, Enemy::Base &Defender, Combat::FightMode FightMode)
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

        const char *FightChoices1[2] = {(FightMode != Combat::FightMode::SHOOT ? "FIGHT" : "SHOOT"), "CANCEL"}; // attacker controls

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

        SDL_Surface *Dice[6];

        Dice[0] = Assets::Copy(Assets::Type::Dice1);
        Dice[1] = Assets::Copy(Assets::Type::Dice2);
        Dice[2] = Assets::Copy(Assets::Type::Dice3);
        Dice[3] = Assets::Copy(Assets::Type::Dice4);
        Dice[4] = Assets::Copy(Assets::Type::Dice5);
        Dice[5] = Assets::Copy(Assets::Type::Dice6);

        auto Fight = Assets::Copy(Assets::Type::Fight);

        auto Shoot = FightMode == Combat::FightMode::SHOOT && Attacker.Type == Enemy::Type::NinjaAssassin ? Assets::Copy(Assets::Type::Shuriken) : Assets::Copy(Assets::Type::Shoot);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;
        auto QuarterStaff = false;

        std::vector<TextButton> &Controls = Attacker.Enthraled ? FightControls1 : FightControls2;

        auto CurrentStage = Combat::Stage::START;

        Engine::Randomize();

        auto FightRolls = 2;
        FightRolls += Engine::HasStatus(Attacker, Spell::Type::Nighthowl) ? 1 : 0;

        auto DamageRolls = FightMode != Combat::FightMode::SHOOT ? Attacker.Damage : 1;

        std::vector<int> Rolls(FightRolls, 0);
        std::vector<int> Damages(DamageRolls, 0);

        auto DamageSum = 0;
        auto FightingSum = 0;
        auto CalculatedDamage = false;
        auto AssignedDamage = false;
        auto Retribution = false;
        auto Done = false;

        while (!Done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            // attacker stats
            Graphics::PutText(Renderer, Attacker.Name.c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY);
            Graphics::PutText(Renderer, ("FPR: " + std::to_string(Attacker.FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + RowHeight);
            Graphics::PutText(Renderer, ("END: " + std::to_string(Attacker.Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 2 * RowHeight);

            if (FightMode == Combat::FightMode::SHOOT)
            {
                auto DamageModifier = Attacker.DamageModifier;

                if (Attacker.Type == Enemy::Type::NinjaAssassin)
                {
                    DamageModifier -= 1;
                }

                Graphics::PutText(Renderer, ("DMG: 1D" + std::string(DamageModifier < 0 ? "" : "+") + std::to_string(DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 3 * RowHeight);
            }
            else
            {
                Graphics::PutText(Renderer, ("DMG: " + std::to_string(Attacker.Damage) + "D" + (Attacker.DamageModifier < 0 ? "" : "+") + std::to_string(Attacker.DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 3 * RowHeight);
            }

            Graphics::PutText(Renderer, ("ARM: " + std::to_string(Attacker.Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 4 * RowHeight);

            auto EnemyOffset = 5;

            if (Attacker.Enthraled)
            {
                Graphics::PutText(Renderer, "ENTHRALED", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + EnemyOffset * RowHeight);

                EnemyOffset++;
            }

            if (Engine::HasStatus(Attacker, Spell::Type::Nighthowl))
            {
                Graphics::PutText(Renderer, "NIGHTHOWL", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + EnemyOffset * RowHeight);

                EnemyOffset++;
            }

            // defender stats
            Graphics::PutText(Renderer, Defender.Name.c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY);
            Graphics::PutText(Renderer, ("FPR: " + std::to_string(Defender.FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + RowHeight);
            Graphics::PutText(Renderer, ("END: " + std::to_string(Defender.Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + 2 * RowHeight);
            Graphics::PutText(Renderer, ("DMG: " + std::to_string(Defender.Damage) + "D" + (Defender.DamageModifier < 0 ? "" : "+") + std::to_string(Defender.DamageModifier)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + 3 * RowHeight);
            Graphics::PutText(Renderer, ("ARM: " + std::to_string(Defender.Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + 4 * RowHeight);

            auto TargetOffset = 5;

            if (Defender.Enthraled)
            {
                Graphics::PutText(Renderer, "ENTHRALED", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + TargetOffset * RowHeight);

                TargetOffset++;
            }

            if (Engine::HasStatus(Defender, Spell::Type::Nighthowl))
            {
                Graphics::PutText(Renderer, "NIGHTHOWL", Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, MidWindow, TextY + TargetOffset * RowHeight);

                TargetOffset++;
            }

            if (FightMode == Combat::FightMode::FIGHT)
            {
                Graphics::RenderImage(Renderer, Fight, MidWindow - (3 * Map.ObjectSize / 2 + border_pts), WindowY + Map.ObjectSize);
            }
            else if (FightMode == Combat::FightMode::SHOOT)
            {
                Graphics::RenderImage(Renderer, Shoot, MidWindow - (3 * Map.ObjectSize / 2 + border_pts), WindowY + Map.ObjectSize);
            }

            if (CurrentStage == Combat::Stage::FIGHT && Result == Combat::Result::NONE)
            {
                FightingSum = 0;

                for (auto i = 0; i < FightRolls; i++)
                {
                    Rolls[i] = Engine::Roll(1, 0);

                    FightingSum += Rolls[i];
                }

                FightingSum = std::max(0, FightingSum);

                auto FightResult = Attacker.FightingProwess >= FightingSum;

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
                    Graphics::StretchImage(Renderer, Dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (EnemyOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                }

                Graphics::PutText(Renderer, ((FightMode != Combat::FightMode::SHOOT ? "Fight Score: " : "Shooting Score: ") + std::to_string(FightingSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (EnemyOffset + 4) * RowHeight);

                Graphics::PutText(Renderer, (Attacker.Name + " hits " + Defender.Name + "!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (EnemyOffset + 5) * RowHeight);

                if (!CalculatedDamage)
                {
                    DamageSum = 0;

                    // compute damage
                    for (auto i = 0; i < DamageRolls; i++)
                    {
                        Damages[i] = Engine::Roll(1, 0);

                        DamageSum += Damages[i];
                    }

                    DamageSum += Attacker.DamageModifier;

                    if (FightMode == Combat::FightMode::SHOOT)
                    {
                        if (Attacker.Type == Enemy::Type::NinjaAssassin)
                        {
                            DamageSum -= 1;
                        }
                    }

                    if (Attacker.Type != Enemy::Type::Skiapyr)
                    {
                        DamageSum -= Defender.Armour;
                    }

                    DamageSum = std::max(0, DamageSum);

                    if (FightMode != Combat::FightMode::SHOOT && DamageSum > 0 && Defender.Type == Enemy::Type::IconTheUngodly)
                    {
                        Retribution = true;
                    }

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
                        Graphics::StretchImage(Renderer, Dice[Rolls[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (EnemyOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                    }

                    Graphics::PutText(Renderer, ((FightMode != Combat::FightMode::SHOOT ? "Fight Score: " : "Shooting Score: ") + std::to_string(FightingSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (EnemyOffset + 4) * RowHeight);

                    Graphics::PutText(Renderer, (Attacker.Name + "'s attack was unsuccessful!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (EnemyOffset + 5) * RowHeight);
                }
                else
                {
                    // show damage results
                    for (auto i = 0; i < DamageRolls; i++)
                    {
                        Graphics::StretchImage(Renderer, Dice[Damages[i] - 1], TextButtonX + i * (Map.ObjectSize + 2 * text_space), TextY + (EnemyOffset + 1) * RowHeight, Map.ObjectSize, Map.ObjectSize);
                    }

                    std::string DamageString = "";

                    if (Attacker.Type != Enemy::Type::Skiapyr)
                    {
                        DamageString = "Damage Dealt (-Armour): ";
                    }
                    else
                    {
                        DamageString = "Damage Dealt: ";
                    }

                    Graphics::PutText(Renderer, (DamageString + std::to_string(DamageSum)).c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + (EnemyOffset + 4) * RowHeight);

                    if (!AssignedDamage)
                    {
                        Engine::Gain(Defender, -DamageSum);

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
                    if (Retribution)
                    {
                        RenderMessage(Renderer, BattleScreen, Map, bg, "Retribution spell retaliates for 1 damage!", intBK);

                        Engine::Gain(Attacker, -1);
                    }

                    Done = true;
                }
            }
        }

        for (auto i = 0; i < 6; i++)
        {
            if (Dice[i])
            {
                SDL_FreeSurface(Dice[i]);

                Dice[i] = NULL;
            }
        }

        if (Fight)
        {
            SDL_FreeSurface(Fight);

            Fight = NULL;
        }

        if (Shoot)
        {
            SDL_FreeSurface(Shoot);

            Shoot = NULL;
        }

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

        auto Done = false;

        while (!Done)
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
                    Done = true;
                }
                else if (Controls[Current].Type == Control::Type::CAST)
                {
                    if (Mode == Control::Type::CALL)
                    {
                        if (Engine::WasCalledToMind(Character, Spell::All[Current].Type))
                        {
                            DisplayMessage((Spell::All[Current].Name + " already called to mind!"), intBK);
                        }
                        else
                        {
                            Result = Current;

                            Done = true;
                        }
                    }
                    else if (Mode == Control::Type::FORGET)
                    {
                        if (!Engine::WasCalledToMind(Character, Spell::All[Current].Type))
                        {
                            DisplayMessage((Spell::All[Current].Name + " not called to mind!"), intBK);
                        }
                        else
                        {
                            Result = Current;

                            Done = true;
                        }
                    }
                }
            }
        }

        return Result;
    }

    int SelectSpell(SDL_Renderer *Renderer, std::vector<Button> &BattleScreen, Uint32 Bg, Map::Base &Map, Character::Base &Character)
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

        auto Done = false;

        while (!Done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, Bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, "Select a spell to cast", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, WindowW - 4 * text_space, TTF_FontHeight(Fonts::Normal), WindowButtonX - text_space, WindowY + text_space);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Character.Spells.size())
            {
                // render spell names
                auto CaptionX = Controls[Current].X - text_space;

                auto CaptionY = Controls[Current].Y + Controls[Current].H + text_space;

                std::string Caption = "Cast " + Character.Spells[Current].Name + " (" + std::string(Spell::ClassDescription[Character.Spells[Current].Class]) + ")";

                auto CaptionW = 0;

                auto CaptionH = 0;

                TTF_SizeText(Fonts::Caption, Caption.c_str(), &CaptionW, &CaptionH);

                Graphics::PutText(Renderer, Caption.c_str(), Fonts::Caption, border_pts, clrWH, Bg, TTF_STYLE_NORMAL, CaptionW + 2 * text_space, CaptionH, CaptionX, CaptionY);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK)
                {
                    Done = true;
                }
                else if (Controls[Current].Type == Control::Type::CAST)
                {
                    Result = Current;

                    Done = true;
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

        auto Done = false;

        while (!Done)
        {
            // render current combat screen
            Interface::RenderCombatScreen(Renderer, BattleScreen, -1, bg);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, "Use Ability", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, WindowW - 4 * text_space, TTF_FontHeight(Fonts::Normal), WindowButtonX - text_space, WindowY + text_space);

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
                    Done = true;
                }
                else if (Current >= 0 && Current < Abilities.size())
                {
                    Result = Abilities[Current];

                    Done = true;
                }
            }
        }

        return Result;
    }

    void RenderMapInfo(SDL_Renderer *Renderer, Map::Base &Map, Party::Base &Party, std::vector<Enemy::Base> &Enemies, std::vector<Button> &Controls, std::vector<Combatants> &Sequence, Combat::Mode CurrentMode, int CombatRound, int ShootingRounds, int Current, int CurrentCombatant, int SelectedCombatant, int SelectedSpell)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);

        auto MapSizeX = Map.SizeX * Map.ObjectSize;

        auto MapSizeY = Map.SizeY * Map.ObjectSize;

        auto IsPlayer = std::get<0>(Sequence[CurrentCombatant]) == Map::Object::Player;

        auto NoneSelected = (SelectedCombatant < 0 || SelectedCombatant >= Sequence.size());

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

            if (ControlType == Control::Type::PLAYER && NoneSelected)
            {
                if (Map.ValidX(SelectX) && Map.ValidY(SelectY))
                {
                    Interface::CharacterSheet(Renderer, Map, Party, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                }
            }
            else if (ControlType == Control::Type::ENEMY && NoneSelected)
            {
                if (Map.ValidX(SelectX) && Map.ValidY(SelectY))
                {
                    Interface::EnemyData(Renderer, Map, Enemies, Fonts::Fixed, Map.Tiles[SelectY][SelectX].Id - 1);
                }
            }
            else if ((ControlType == Control::Type::MAP_NONE || ControlType == Control::Type::DESTINATION || ControlType == Control::Type::MAP_EXIT) && NoneSelected)
            {
                if (Map.ValidX(SelectX) && Map.ValidY(SelectY))
                {
                    Interface::ShowCoordinates(Renderer, Map.Tiles[SelectY][SelectX].Type, SelectX, SelectY, Fonts::Normal, Map.TextWidth, Map.TextRightX, Map.DrawY);
                }
            }

            if (CurrentMode == Combat::Mode::NORMAL)
            {
                if (ControlType == Control::Type::PLAYER)
                {
                    Graphics::PutText(Renderer, "View party member", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
                }
                else if (ControlType == Control::Type::ENEMY)
                {
                    Graphics::PutText(Renderer, "View opponent", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
                }
                else
                {
                    if (ShootingRounds > 0)
                    {
                        Graphics::PutText(Renderer, ("Shooting Rounds Left: " + std::to_string(ShootingRounds)).c_str(), Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
                    }
                    else
                    {
                        Graphics::PutText(Renderer, ("Round: " + std::to_string(CombatRound + 1)).c_str(), Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
                    }
                }
            }
            else if (CurrentMode == Combat::Mode::MOVE)
            {
                Graphics::PutText(Renderer, "Move to location", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);

                auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                if ((ControlType == Control::Type::DESTINATION) || (IsPlayer && ControlType == Control::Type::MAP_EXIT) || (!IsPlayer && ControlType == Control::Type::MAP_NONE && Map.Tiles[SelectY][SelectX].IsPassableToEnemy))
                {
                    auto PlayerX = -1;

                    auto PlayerY = -1;

                    Map.Find(std::get<0>(Sequence[CurrentCombatant]), PlayerId, PlayerX, PlayerY);

                    if (Map.ValidX(PlayerX) && Map.ValidY(PlayerY) && Map.ValidX(SelectX) && Map.ValidY(SelectY) && Map.Distance(PlayerX, PlayerY, SelectX, SelectY) > 1)
                    {
                        auto TempPath = AStar::FindPath(Map, PlayerX, PlayerY, SelectX, SelectY, IsPlayer ? false : true);

                        if (TempPath.Points.size() > 0)
                        {
                            Interface::DrawPath(Renderer, Map, TempPath, 1, intGR, 0x66);
                        }
                    }
                }
            }
            else if (CurrentMode == Combat::Mode::ATTACK)
            {
                if (ControlType == Control::Type::PLAYER || ControlType == Control::Type::ENEMY)
                {
                    Graphics::PutText(Renderer, "Fight target", Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
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
                }
            }
            else if (CurrentMode == Combat::Mode::CAST && ControlType == Control::Type::PLAYER)
            {
                if (IsPlayer)
                {
                    auto PlayerId = std::get<1>(Sequence[CurrentCombatant]);

                    std::string cast = "Cast " + Party.Members[PlayerId].Spells[SelectedSpell].Name + " on target";

                    Graphics::PutText(Renderer, cast.c_str(), Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, Map.TextWidth, FontSize, Map.TextX, Map.TextY);
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

            auto Selected = std::get<0>(Sequence[SelectedCombatant]);

            Map.Find(Selected, SelectedId, SelectedX, SelectedY);

            auto IsPlayer = Selected == Map::Object::Player;

            auto IsEnemy = Selected == Map::Object::Enemy;

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
                    // show potential target
                    auto NearestTarget = Interface::EmptyTarget;

                    if (!Enemies[SelectedId].TargetAll)
                    {
                        if (Enemies[SelectedId].Type == Enemy::Type::Bowmen)
                        {
                            NearestTarget = Interface::BowmanTarget(Map, Party, SelectedId);
                        }
                        else
                        {
                            NearestTarget = Interface::SelectTarget(Map, Party, SelectedId, Interface::SortDistance, Enemies[SelectedId].CanShoot);
                        }
                    }
                    else
                    {
                        NearestTarget = Interface::SelectAllTargets(Map, Party, Enemies, SelectedId, Interface::SortDistance, Enemies[SelectedId].CanShoot);
                    }

                    auto TargetId = std::get<1>(NearestTarget);

                    auto LocationX = -1;

                    auto LocationY = -1;

                    auto EnemyX = -1;

                    auto EnemyY = -1;

                    auto TargetType = std::get<0>(NearestTarget);

                    Map.Find(Map::Object::Enemy, SelectedId, EnemyX, EnemyY);

                    if (TargetType == Map::Object::Player)
                    {
                        if (TargetId >= 0 && TargetId < Party.Members.size())
                        {
                            Map.Find(Map::Object::Player, TargetId, LocationX, LocationY);
                        }
                    }
                    else if (TargetType == Map::Object::Enemy)
                    {
                        if (TargetId >= 0 && TargetId < Enemies.size())
                        {
                            Map.Find(Map::Object::Enemy, TargetId, LocationX, LocationY);
                        }
                    }

                    if (Map.ValidX(EnemyX) && Map.ValidY(EnemyY) && Map.ValidX(LocationX) && Map.ValidY(LocationY))
                    {
                        if (Enemies[SelectedId].CanMove)
                        {
                            auto EnemyPath = AStar::FindPath(Map, EnemyX, EnemyY, LocationX, LocationY, true);

                            if (EnemyPath.Points.size() > 0)
                            {
                                Interface::DrawPath(Renderer, Map, EnemyPath, 1, intGR, 0x66);
                            }
                        }
                        else
                        {
                            if (Map.IsVisible(LocationX, LocationY))
                            {
                                SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);

                                Graphics::FillRect(Renderer, Map.ObjectSize, Map.ObjectSize, Map.DrawX + (LocationX - Map.MapX) * Map.ObjectSize, Map.DrawY + (LocationY - Map.MapY) * Map.ObjectSize, O(intGR, 0x66));

                                SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_NONE);

                                Graphics::ThickRect(Renderer, Map.ObjectSize - 4 * border_pts, Map.ObjectSize - 4 * border_pts, Map.DrawX + (LocationX - Map.MapX) * Map.ObjectSize + 2 * border_pts, Map.DrawY + (LocationY - Map.MapY) * Map.ObjectSize + 2 * border_pts, intWH, border_pts);
                            }
                        }
                    }
                }
            }
        }
    }

    Combat::Result CombatScreen(SDL_Window *Window, SDL_Renderer *Renderer, Story::Base *Story, Map::Base &Map, Party::Base &Party)
    {
        // setup additional players
        Map.AdditionalPlayers(Party);

        // remove non-existent players and enemies
        Map.Clean(Party, Story->Enemies);

        Map.ClearExitTriggers();

        Battle::Base &Battle = Story->Battle;

        if (Battle.SoloCombat >= 0 && Battle.SoloCombat < Party.Members.size())
        {
            Map.Solo(Party, Battle.SoloCombat);
        }

        std::vector<Enemy::Base> &Enemies = Story->Enemies;

        auto Exit = false;

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

        auto PaddingY = 2 * starty + Map.ObjectSize;

        // size of viewable grid
        Map.SizeX = (SCREEN_WIDTH - 2 * PaddingX) / Map.ObjectSize;

        Map.SizeY = (SCREEN_HEIGHT - PaddingY) / Map.ObjectSize;

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

        auto CheckMapBounds = [&]
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
        };

        auto Center = [&](int PlayerId)
        {
            auto CenterX = -1;

            auto CenterY = -1;

            Map.Find(Map::Object::Player, PlayerId, CenterX, CenterY);

            Map.MapX = CenterX - (Map.SizeX / 2);

            Map.MapY = CenterY - (Map.SizeY / 2);

            CheckMapBounds();
        };

        // player input
        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;
        auto CombatRound = 0;

        auto GenerateSequence = [&](std::vector<Interface::Combatants> &Sequence)
        {
            Sequence.clear();

            auto SoloCombat = Battle.SoloCombat >= 0 && Battle.SoloCombat < Party.Members.size();

            for (auto i = 0; i < Party.Members.size(); i++)
            {
                if (((SoloCombat && i == Battle.SoloCombat) || !SoloCombat) && Engine::IsAlive(Party.Members[i]))
                {
                    Sequence.push_back({Map::Object::Player, i, Engine::Awareness(Party.Members[i])});
                }
            }

            for (auto i = 0; i < Enemies.size(); i++)
            {
                if (Enemies[i].StartRound <= CombatRound && Engine::IsAlive(Enemies[i]))
                {
                    Sequence.push_back({Map::Object::Enemy, i, Enemies[i].Awareness});
                }
            }
        };

        // round sequence
        std::vector<Interface::Combatants> Sequence = {};

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

        auto QuickThinkingRound = false;

        auto UsedScrollOfInvisibility = false;

        auto ScrollOfInvisibilityRound = 0;

        auto ActFirstRound = false;

        auto ShootingRounds = Battle.ShootingRounds;

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
            return std::get<1>(target);
        };

        auto NextQuickThinker = [&]()
        {
            auto Next = 0;

            for (auto i = 0; i < Sequence.size(); i++)
            {
                if (IsPlayer(i))
                {
                    if (Party.Members[GetId(i)].QuickThinking)
                    {
                        Next = i;

                        break;
                    }
                }
            }

            return Next;
        };

        auto NextFirst = [&]()
        {
            auto Next = 0;

            for (auto i = 0; i < Sequence.size(); i++)
            {
                if (IsPlayer(i) && Battle.SurprisedEnemy)
                {
                    if (Party.Members[GetId(i)].ActFirst)
                    {
                        Next = i;

                        break;
                    }
                }
                else if (IsEnemy(i) && Battle.SurprisedByEnemy)
                {
                    if (Enemies[GetId(i)].ActFirst)
                    {
                        Next = i;

                        break;
                    }
                }
            }

            return Next;
        };

        auto NextShooter = [&]()
        {
            auto Next = -1;

            for (auto i = 0; i < Sequence.size(); i++)
            {
                if (IsPlayer(i))
                {
                    if (Party.Members[GetId(i)].ShootFirst)
                    {
                        Next = i;

                        break;
                    }
                }
            }

            return Next;
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
        auto ActionsY = Map.DrawY + ((Map.SizeY < 9 ? 9 : Map.SizeY) * Map.ObjectSize) + 2 * text_space;
        auto ActionsGrid = MapButtonSize;

        auto StartMap = 12;
        auto BottomMapX = StartMap + (Map.SizeX * (Map.SizeY - 1));
        auto MidMapY = StartMap + (Map.SizeY / 2 * Map.SizeX) - Map.SizeX;

        for (auto i = 0; i < Enemies.size(); i++)
        {
            if (Enemies[i].StartRound != 0 && Map.ValidX(Enemies[i].StartX) && Map.ValidY(Enemies[i].StartY))
            {
                Map.Tiles[Enemies[i].StartY][Enemies[i].StartX].IsPassable = false;

                Map.Tiles[Enemies[i].StartY][Enemies[i].StartX].IsPassableToEnemy = false;
            }
        }

        // preserve starting map, party, and enemy stats (for use with time blink)
        auto InitialMap = Map;
        auto InitialParty = Party;
        auto InitialEnemies = Enemies;

        auto Controls = std::vector<Button>();

        Controls.push_back(Button(0, Assets::Get(Assets::Type::Up), 0, StartMap, 0, 1, MapButtonsX, MapButtonsY, Map.MapY > 0 ? intWH : intGR, Control::Type::MAP_UP));
        Controls.push_back(Button(1, Assets::Get(Assets::Type::Left), 1, MidMapY, 0, 2, MapButtonsX, MapButtonsY + (MapButtonsGridSize + 2 * text_space), Map.MapX > 0 ? intWH : intGR, Control::Type::MAP_LEFT));
        Controls.push_back(Button(2, Assets::Get(Assets::Type::Right), 2, MidMapY + Map.SizeX, 1, 3, MapButtonsX, MapButtonsY + 2 * (MapButtonsGridSize + 2 * text_space), (Map.MapX < Map.Width - Map.SizeX) ? intWH : intGR, Control::Type::MAP_RIGHT));
        Controls.push_back(Button(3, Assets::Get(Assets::Type::Down), 3, BottomMapX, 2, 5, MapButtonsX, MapButtonsY + 3 * (MapButtonsGridSize + 2 * text_space), (Map.MapY < Map.Height - Map.SizeY) ? intWH : intGR, Control::Type::MAP_DOWN));
        Controls.push_back(Button(4, Assets::Get(Assets::Type::Move), 3, 5, BottomMapX, 4, ActionsX, ActionsY, intWH, Control::Type::MOVE));
        Controls.push_back(Button(5, Assets::Get(Assets::Type::Fight), 4, 6, Map.SizeX > 1 ? BottomMapX + 1 : 5, 5, ActionsX + ActionsGrid, ActionsY, intWH, Control::Type::ATTACK));
        Controls.push_back(Button(6, Assets::Get(Assets::Type::Defend), 5, 7, Map.SizeX > 2 ? BottomMapX + 2 : 6, 6, ActionsX + 2 * ActionsGrid, ActionsY, intWH, Control::Type::DEFEND));
        Controls.push_back(Button(7, Assets::Get(Assets::Type::Shoot), 6, 8, Map.SizeX > 3 ? BottomMapX + 3 : 7, 7, ActionsX + 3 * ActionsGrid, ActionsY, intWH, Control::Type::SHOOT));
        Controls.push_back(Button(8, Assets::Get(Assets::Type::Ability), 7, 9, Map.SizeX > 4 ? BottomMapX + 4 : 8, 8, ActionsX + 4 * ActionsGrid, ActionsY, intWH, Control::Type::ABILITY));
        Controls.push_back(Button(9, Assets::Get(Assets::Type::Items), 8, 10, Map.SizeX > 5 ? BottomMapX + 5 : 9, 9, ActionsX + 5 * ActionsGrid, ActionsY, intWH, Control::Type::ITEMS));
        Controls.push_back(Button(10, Assets::Get(Assets::Type::Flee), 9, 11, Map.SizeX > 6 ? BottomMapX + 6 : 10, 10, ActionsX + 6 * ActionsGrid, ActionsY, intWH, Control::Type::FLEE));
        Controls.push_back(Button(11, Assets::Get(Assets::Type::Exit), 10, 11, Map.SizeX > 6 ? BottomMapX + 7 : 11, 11, ActionsX + 7 * ActionsGrid, ActionsY, intWH, Control::Type::EXIT));

        // center map on first player
        Center(Engine::First(Party));

        // generate controls within the map window
        Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

        auto StartShootingRound = [&]()
        {
            if (ShootingRounds > 0)
            {
                Engine::ShootFirst(Party);

                CurrentCombatant = NextShooter();
            }
            else
            {
                if (Battle.ShootingRounds > 0)
                {
                    Interface::RenderMessage(Renderer, Controls, Map, intBK, "Shooting Round Ends!", intGR);
                }
            }
        };

        auto EnterNewCombatants = [&]()
        {
            auto NewCombatants = false;

            for (auto i = 0; i < Enemies.size(); i++)
            {
                if (Enemies[i].StartRound > 0 && Enemies[i].StartRound == CombatRound && Map.ValidX(Enemies[i].StartX) && Map.ValidY(Enemies[i].StartY))
                {
                    NewCombatants = true;

                    Sequence.push_back({Map::Object::Enemy, i, Enemies[i].Awareness});

                    Map.Put(Enemies[i].StartX, Enemies[i].StartY, Map::Object::Enemy, i);

                    Map.Tiles[Enemies[i].StartY][Enemies[i].StartX].IsPassable = true;

                    Map.Tiles[Enemies[i].StartY][Enemies[i].StartX].IsPassableToEnemy = true;
                }
            }

            if (NewCombatants)
            {
                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
            }
        };

        auto CycleCombatants = [&]()
        {
            if (IsPlayer(CurrentCombatant))
            {
                if (QuickThinkingRound && Party.Members[GetId(CurrentCombatant)].QuickThinking)
                {
                    Party.Members[GetId(CurrentCombatant)].QuickThinking = false;

                    Party.Members[GetId(CurrentCombatant)].UsedQuickThinking = true;
                }

                if (ActFirstRound && Party.Members[GetId(CurrentCombatant)].ActFirst)
                {
                    Party.Members[GetId(CurrentCombatant)].ActFirst = false;
                }

                if (Party.Members[GetId(CurrentCombatant)].ShootFirst)
                {
                    Party.Members[GetId(CurrentCombatant)].ShootFirst = false;
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

                if (ActFirstRound && Enemies[GetId(CurrentCombatant)].ActFirst)
                {
                    Enemies[GetId(CurrentCombatant)].ActFirst = false;
                }

                if (!QuickThinkingRound)
                {
                    Engine::UpdateSpellStatus(Enemies[GetId(CurrentCombatant)], CombatRound);
                }
            }

            auto Active = false;

            while (!Active)
            {
                if (!Engine::IsAlive(Enemies) || Engine::Enthraled(Enemies) || Engine::Remaining(Party) == 0)
                {
                    break;
                }

                if (ActFirstRound)
                {
                    if (Battle.SurprisedEnemy && Engine::ActingFirst(Party))
                    {
                        CurrentCombatant = NextFirst();
                    }
                    else if (Battle.SurprisedByEnemy && Engine::ActingFirst(Enemies))
                    {
                        CurrentCombatant = NextFirst();
                    }
                    else
                    {
                        Interface::RenderMessage(Renderer, Controls, Map, intBK, "Surprise attack ends!", intGR);

                        ActFirstRound = false;

                        CurrentCombatant = 0;

                        Selected = false;
                    }
                }
                else if (ShootingRounds > 0)
                {
                    CurrentCombatant = NextShooter();

                    if (CurrentCombatant == 0 || CurrentCombatant >= Sequence.size())
                    {
                        ShootingRounds--;

                        StartShootingRound();
                    }
                }
                else if (QuickThinkingRound)
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

                        Selected = false;
                    }
                }
                else
                {
                    CurrentCombatant++;

                    if (CurrentCombatant >= Sequence.size())
                    {
                        if (ActFirstRound && Battle.SurprisedEnemy && Engine::ActingFirst(Party))
                        {
                            CurrentCombatant = NextFirst();
                        }
                        else if (ActFirstRound && Battle.SurprisedByEnemy && Engine::ActingFirst(Enemies))
                        {
                            CurrentCombatant = NextFirst();
                        }
                        else if (Engine::KnockedOff(Enemies))
                        {
                            ActFirstRound = false;

                            Interface::KnockedOffSequence(Sequence, Enemies);

                            CurrentCombatant = 0;
                        }
                        else if (Engine::QuickThinking(Party))
                        {
                            Interface::RenderMessage(Renderer, Controls, Map, intBK, "Quick thinking round begins!", intGR);

                            ActFirstRound = false;

                            QuickThinkingRound = true;

                            CurrentCombatant = NextQuickThinker();

                            Selected = false;
                        }
                        else
                        {
                            ActFirstRound = false;

                            QuickThinkingRound = false;

                            CurrentCombatant = 0;

                            CombatRound++;

                            EnterNewCombatants();

                            Interface::SortCombatants(Sequence);
                        }
                    }
                }

                if (IsPlayer(CurrentCombatant))
                {
                    auto character = Party.Members[GetId(CurrentCombatant)];

                    Active = Engine::IsAlive(character) && !character.Escaped && !Engine::Paralyzed(character) && !character.Away;

                    if (Party.Members[GetId(CurrentCombatant)].Defending)
                    {
                        Party.Members[GetId(CurrentCombatant)].Defending = false;

                        Party.Members[GetId(CurrentCombatant)].Defended = true;
                    }
                    else
                    {
                        Party.Members[GetId(CurrentCombatant)].Defended = false;
                    }

                    if (Party.Members[GetId(CurrentCombatant)].Engaged)
                    {
                        Party.Members[GetId(CurrentCombatant)].Engaged = false;
                    }
                }
                else if (IsEnemy(CurrentCombatant))
                {
                    Enemies[GetId(CurrentCombatant)].Attacked = -1;

                    Active = Engine::IsAlive(Enemies[GetId(CurrentCombatant)]);
                }
            }

            if (IsPlayer(CurrentCombatant))
            {
                Center(GetId(CurrentCombatant));

                // generate controls within the map window
                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
            }

            CurrentMode = Combat::Mode::NORMAL;

            ResetSelection();
        };

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

        auto StartSurpriseRound = [&]()
        {
            ActFirstRound = (Battle.SurprisedEnemy || Battle.SurprisedByEnemy) ? true : false;

            if (ActFirstRound)
            {
                if (Battle.SurprisedEnemy)
                {
                    Engine::ActFirst(Party);
                }
                else if (Battle.SurprisedByEnemy)
                {
                    Engine::ActFirst(Enemies);
                }

                CurrentCombatant = NextFirst();

                Interface::RenderMessage(Renderer, Controls, Map, intBK, "Surprise attack begins!", intGR);
            }
        };

        auto ClearPartyStatus = [&]()
        {
            Engine::ClearAwayStatus(Party);

            Engine::ClearDefendingStatus(Party);

            Engine::ClearEngaged(Party);

            Engine::NormalThinking(Party);

            Engine::ResetSpellDifficulty(Party);

            Engine::ClearSpellStatus(Party);

            Engine::NormalCombatOrder(Party);
        };

        auto TimeBlink = [&](int Reader)
        {
            Map = InitialMap;

            Party = InitialParty;

            Enemies = InitialEnemies;

            // clear combat status
            ClearPartyStatus();

            Engine::ClearEscaped(Party);

            Engine::ClearParalyzed(Party);

            Engine::NormalCombatOrder(Enemies);

            Sequence.clear();

            GenerateSequence(Sequence);

            SortCombatants(Sequence);

            ResetSelection();

            CurrentMode = Combat::Mode::NORMAL;

            QuickThinkingRound = false;

            UsedScrollOfInvisibility = false;

            CurrentCombatant = 0;

            CombatRound = 0;

            SelectedSpell = -1;

            // setup shooting round
            ShootingRounds = Battle.ShootingRounds;

            StartShootingRound();

            Center(Engine::First(Party));

            GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

            if (ShootingRounds > 0)
            {
                if (Battle.ShootingRounds > 0)
                {
                    Interface::RenderMessage(Renderer, Controls, Map, intBK, "Shooting Round Begins!", intGR);
                }
            }

            // setup surprise first round attacks
            StartSurpriseRound();

            if (Reader >= 0 && Reader < Party.Members.size())
            {
                // remove scroll from adventurer who read it
                Engine::Drop(Party.Members[Reader], Item::Type::ScrollOfTimeBlink);

                InitialParty = Party;
            }
        };

        if (Window && Renderer && Map.Width > 0 && Map.Height > 0)
        {
            TimeBlink(-1);

            while (Engine::IsAlive(Enemies) && !Engine::Enthraled(Enemies) && Engine::Remaining(Party) > 0)
            {
                CheckMapBounds();

                // select which object to blink (player/Enemy)
                auto BlinkX = -1;

                auto BlinkY = -1;

                if (IsPlayer(CurrentCombatant))
                {
                    auto PlayerX = -1;

                    auto PlayerY = -1;

                    Map.Find(Map::Object::Player, GetId(CurrentCombatant), PlayerX, PlayerY);

                    if (Map.IsVisible(PlayerX, PlayerY))
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

                        Map.Find(Map::Object::Enemy, GetId(CurrentCombatant), EnemyX, EnemyY);

                        if (Map.IsVisible(EnemyX, EnemyY))
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

                Interface::RenderMapInfo(Renderer, Map, Party, Enemies, Controls, Sequence, CurrentMode, CombatRound, ShootingRounds, Current, CurrentCombatant, SelectedCombatant, SelectedSpell);

                RenderFlashMessage();

                auto Enthraled = false;

                if (IsEnemy(CurrentCombatant))
                {
                    Enthraled = Enemies[GetId(CurrentCombatant)].Enthraled;
                }

                // get player input
                if ((IsPlayer(CurrentCombatant) && !Party.Members[GetId(CurrentCombatant)].Defending && !Engine::Paralyzed(Party.Members[GetId(CurrentCombatant)]) && Engine::IsAlive(Party.Members[GetId(CurrentCombatant)])))
                {
                    Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

                    auto PlayerId = GetId(CurrentCombatant);

                    Character::Base &Character = Party.Members[PlayerId];

                    auto CurrentX = -1;

                    auto CurrentY = -1;

                    Map.Find(Map::Object::Player, PlayerId, CurrentX, CurrentY);

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
                            Exit = true;

                            Selected = false;

                            break;
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

                            Character.Defending = true;

                            Selected = false;

                            CycleCombatants();
                        }
                        else if (Controls[Current].Type == Control::Type::FLEE && !Hold)
                        {
                            if (!Character.ShootFirst)
                            {
                                if (Map.Exits.size() > 0)
                                {
                                    if (Map.Tiles[CurrentY][CurrentX].IsExit() || (UsedScrollOfInvisibility && CombatRound <= ScrollOfInvisibilityRound))
                                    {
                                        if (Engine::IsPresent(Enemies, Enemy::Type::Bowmen))
                                        {
                                            Interface::AttackedWhileFleeing(Renderer, Controls, Map, intBK, Character, 2);
                                        }

                                        if (Engine::IsAlive(Character))
                                        {
                                            Interface::RenderMessage(Renderer, Controls, Map, intBK, ("The " + std::string(Character::ClassName[Character.Class]) + " escapes!"), intGR);

                                            Character.Escaped = true;

                                            Map.TriggerExit(CurrentX, CurrentY);
                                        }

                                        Map.Remove(CurrentX, CurrentY);

                                        Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

                                        CycleCombatants();
                                    }
                                    else if (UsedScrollOfInvisibility)
                                    {
                                        DisplayMessage(std::string(Character::ClassName[Character.Class]) + "'s invisibility has worn off!", intBK);
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
                            }
                            else
                            {
                                DisplayMessage("You cannot flee at this time!", intBK);
                            }

                            Selected = false;
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
                                if (!Character.ShootFirst)
                                {
                                    SelectedSpell = -1;

                                    if (Map.Distance(CurrentX, CurrentY, SelectX, SelectY) > 1)
                                    {
                                        if (Interface::EnemyAdjacent(Map, Enemies, Character, PlayerId) && !Character.Defended)
                                        {
                                            DisplayMessage("There are enemies nearby!", intBK);
                                        }
                                        else
                                        {
                                            auto CurrentPath = AStar::FindPath(Map, CurrentX, CurrentY, SelectX, SelectY);

                                            if (CurrentPath.Points.size() > 0)
                                            {
                                                if (!Interface::FullMove(Renderer, Controls, intBK, Map, Party, Enemies, CurrentPath, StartMap))
                                                {
                                                    DisplayMessage("Path blocked!", intBK);
                                                }
                                                else
                                                {
                                                    Character.Defended = false;

                                                    Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

                                                    CycleCombatants();
                                                }
                                            }
                                            else
                                            {
                                                DisplayMessage("Path blocked!", intBK);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (Interface::EnemyAdjacent(Map, Enemies, Character, PlayerId) && !Character.Defended)
                                        {
                                            DisplayMessage("There are enemies nearby!", intBK);
                                        }
                                        else
                                        {
                                            if (!Interface::AnimateMove(Renderer, Controls, intBK, Map, Party, Enemies, CurrentX, CurrentY, SelectX, SelectY))
                                            {
                                                DisplayMessage("Path Blocked!", intBK);
                                            }
                                            else
                                            {
                                                Character.Defended = false;

                                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);

                                                CycleCombatants();
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    DisplayMessage("You can only shoot at this time!", intBK);
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
                                if (Character.ShootFirst)
                                {
                                    DisplayMessage("You cannot move at this time!", intBK);
                                }
                                else
                                {
                                    CurrentMode = Combat::Mode::MOVE;
                                }
                            }
                            else if (CurrentMode == Combat::Mode::ATTACK)
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
                                if (Character.Defending || Character.ShootFirst)
                                {
                                    DisplayMessage("You cannot attack at this time.", intBK);

                                    if (QuickThinkingRound && Character.QuickThinking)
                                    {
                                        CycleCombatants();
                                    }
                                }
                                else if (Interface::NearbyEnemies(Map, Enemies, PlayerId, false))
                                {
                                    if (Interface::CountNearbyEnemies(Map, Enemies, PlayerId, false) > 1)
                                    {
                                        CurrentMode = Combat::Mode::ATTACK;
                                    }
                                    else if (Interface::CountNearbyEnemies(Map, Enemies, PlayerId, false) == 1)
                                    {
                                        auto TargetId = Interface::FirstNearbyEnemy(Map, Enemies, PlayerId, false);

                                        Map.Find(Map::Object::Enemy, TargetId, SelectX, SelectY);

                                        Enemy::Base &Target = Enemies[TargetId];

                                        auto Result = Interface::Fight(Renderer, Controls, intBK, Map, Character, Target, Combat::FightMode::FIGHT, false);

                                        if (!Engine::IsAlive(Target))
                                        {
                                            Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " killed!", intGR);

                                            Map.Remove(SelectX, SelectY);

                                            Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                        }

                                        if (!Engine::IsAlive(Character))
                                        {
                                            Interface::RenderMessage(Renderer, Controls, Map, intBK, (std::string(Character::ClassName[Character.Class]) + " killed!"), intBK);

                                            Map.Remove(CurrentX, CurrentY);

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
                                    }
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
                                if (Character.Defending)
                                {
                                    DisplayMessage("You cannot shoot at this time!", intBK);

                                    if (QuickThinkingRound && Character.QuickThinking)
                                    {
                                        CycleCombatants();
                                    }
                                }
                                else if (Engine::CanShoot(Character))
                                {
                                    if (!Interface::NearbyEnemies(Map, Enemies, PlayerId, true) || Character.ShootFirst)
                                    {
                                        if (Engine::HasBow(Character) && Engine::HasArrows(Character))
                                        {
                                            if (Engine::Count(Enemies) == 1)
                                            {
                                                auto TargetId = Engine::First(Enemies);

                                                Map.Find(Map::Object::Enemy, TargetId, SelectX, SelectY);

                                                Enemy::Base &Target = Enemies[TargetId];

                                                auto Result = Interface::Fight(Renderer, Controls, intBK, Map, Character, Target, Combat::FightMode::SHOOT, false);

                                                if (!Engine::IsAlive(Target))
                                                {
                                                    Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " killed!", intGR);

                                                    Map.Remove(SelectX, SelectY);

                                                    Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                                }

                                                if (Result != Combat::Result::NONE)
                                                {
                                                    Engine::ResetSpellDifficulty(Character);

                                                    CycleCombatants();
                                                }
                                                else
                                                {
                                                    DisplayMessage("Shot canceled", intGR);
                                                }

                                                Selected = false;
                                            }
                                            else
                                            {
                                                CurrentMode = Combat::Mode::SHOOT;
                                            }
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
                            if (!Character.ShootFirst)
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
                                                    if (!Interface::NearbyEnemies(Map, Enemies, PlayerId, false))
                                                    {
                                                        DisplayMessage("There are no enemies nearby!", intBK);

                                                        CurrentMode = Combat::Mode::NORMAL;
                                                    }
                                                }
                                                else if (Engine::Count(Enemies) == 1)
                                                {
                                                    // automatically select lone enemy
                                                    auto TargetId = Engine::First(Enemies);

                                                    auto Result = Interface::CastSpell(Renderer, Controls, intBK, Map, Character, SelectedSpell);

                                                    if (Result != Spell::Result::NONE)
                                                    {
                                                        if (Result == Spell::Result::SUCCESS)
                                                        {
                                                            Interface::ApplySpellEffects(Renderer, Controls, intBK, Map, Party, Enemies, PlayerId, TargetId, Character.Spells[SelectedSpell].Type, CombatRound, StartMap);

                                                            Character.Spells.erase(Character.Spells.begin() + SelectedSpell);

                                                            SelectedSpell = -1;

                                                            Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                                        }

                                                        CycleCombatants();
                                                    }

                                                    CurrentMode = Combat::Mode::NORMAL;

                                                    SelectedSpell = -1;
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

                                        while (Character.Spells.size() > Character.SpellLimit)
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
                            else
                            {
                                DisplayMessage("You cannot use abilities at this time!", intBK);
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
                                if (!Character.ShootFirst)
                                {
                                    SelectedSpell = -1;

                                    if (Map.IsAdjacent(PlayerId, TargetId) && Engine::IsAlive(Target))
                                    {
                                        auto Result = Interface::Fight(Renderer, Controls, intBK, Map, Character, Target, Combat::FightMode::FIGHT, false);

                                        if (!Engine::IsAlive(Target))
                                        {
                                            Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " killed!", intGR);

                                            Map.Remove(SelectX, SelectY);

                                            Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                        }

                                        if (!Engine::IsAlive(Character))
                                        {
                                            Interface::RenderMessage(Renderer, Controls, Map, intBK, (std::string(Character::ClassName[Character.Class]) + " killed!"), intBK);

                                            Map.Remove(CurrentX, CurrentY);

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
                                    }
                                    else if (Engine::IsAlive(Target))
                                    {
                                        DisplayMessage("You can only attack adjacent targets!", intBK);

                                        CurrentMode = Combat::Mode::NORMAL;
                                    }
                                }
                                else
                                {
                                    DisplayMessage("You cannot fight at this time!", intBK);
                                }
                            }
                            else if (CurrentMode == Combat::Mode::SHOOT)
                            {
                                SelectedSpell = -1;

                                if ((!Map.IsAdjacent(PlayerId, TargetId) || Character.ShootFirst) && Engine::IsAlive(Target))
                                {
                                    auto Result = Interface::Fight(Renderer, Controls, intBK, Map, Character, Target, Combat::FightMode::SHOOT, false);

                                    if (!Engine::IsAlive(Target))
                                    {
                                        Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " killed!", intGR);

                                        Map.Remove(SelectX, SelectY);

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
                                    if (!Map.IsAdjacent(PlayerId, TargetId))
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

                    Map.Find(Map::Object::Enemy, EnemyId, CurrentX, CurrentY);

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
                            Exit = true;

                            break;
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
                            Selected = false;

                            CycleCombatants();
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
                                if (Map.Distance(CurrentX, CurrentY, SelectX, SelectY) > 1)
                                {
                                    auto EnemyPath = AStar::FindPath(Map, CurrentX, CurrentY, SelectX, SelectY);

                                    if (EnemyPath.Points.size() > 0)
                                    {
                                        if (!Interface::FullMove(Renderer, Controls, intBK, Map, Party, Enemies, EnemyPath, StartMap))
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

                            if (Enemy.CanShoot)
                            {
                                CurrentMode = Combat::Mode::SHOOT;
                            }
                            else
                            {

                                DisplayMessage("Cannot shoot!", intBK);

                                CurrentMode = Combat::Mode::NORMAL;
                            }
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
                                if (Map.IsAdjacent(EnemyId, Map::Object::Enemy, TargetId, Map::Object::Enemy) && Engine::IsAlive(Target))
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
                                            // enemy vs enemy fight
                                            Result = Interface::Fight(Renderer, Controls, intBK, Map, Enemy, Target, Combat::FightMode::FIGHT);

                                            if (!Engine::IsAlive(Target))
                                            {
                                                Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " killed!", intGR);

                                                Map.Remove(SelectX, SelectY);

                                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                            }

                                            if (!Engine::IsAlive(Enemy))
                                            {
                                                Interface::RenderMessage(Renderer, Controls, Map, intBK, Enemy.Name + " killed!", intGR);

                                                Map.Remove(CurrentX, CurrentY);

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
                                }
                                else if (Engine::IsAlive(Target))
                                {
                                    DisplayMessage("You can only attack adjacent targets!", intBK);

                                    CurrentMode = Combat::Mode::NORMAL;
                                }
                            }
                            else if (CurrentMode == Combat::Mode::SHOOT)
                            {
                                if (Enemy.CanShoot)
                                {
                                    // implement enemy vs enemy shot
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
                                            // enemy vs enemy fight
                                            Result = Interface::Fight(Renderer, Controls, intBK, Map, Enemy, Target, Combat::FightMode::SHOOT);

                                            if (!Engine::IsAlive(Target))
                                            {
                                                Interface::RenderMessage(Renderer, Controls, Map, intBK, Target.Name + " killed!", intGR);

                                                Map.Remove(SelectX, SelectY);

                                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                            }
                                        }

                                        // check enemy vs enemy shot results
                                        if (Result != Combat::Result::NONE)
                                        {
                                            CycleCombatants();
                                        }
                                        else
                                        {
                                            DisplayMessage("Shot canceled", intGR);
                                        }
                                    }
                                    else
                                    {
                                        DisplayMessage("Shot aborted!", intBK);
                                    }

                                    Selected = false;
                                }
                                else
                                {
                                    DisplayMessage("This creature cannot attack from range!", intBK);

                                    CurrentMode = Combat::Mode::NORMAL;
                                }
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
                                    if (Map.Distance(CurrentX, CurrentY, SelectX, SelectY) > 1)
                                    {
                                        auto EnemyPath = AStar::FindPath(Map, CurrentX, CurrentY, SelectX, SelectY);

                                        if (EnemyPath.Points.size() > 0)
                                        {
                                            if (!Interface::FullMove(Renderer, Controls, intBK, Map, Party, Enemies, EnemyPath, StartMap))
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
                    // Enemy attacks / moves
                    auto EnemyId = GetId(CurrentCombatant);

                    auto EnemyX = -1;

                    auto EnemyY = -1;

                    Map.Find(Map::Object::Enemy, EnemyId, EnemyX, EnemyY);

                    auto NearestTarget = Interface::EmptyTarget;

                    if (!Enemies[EnemyId].TargetAll)
                    {
                        if (Enemies[EnemyId].Type == Enemy::Type::Bowmen)
                        {
                            NearestTarget = Interface::BowmanTarget(Map, Party, EnemyId);
                        }
                        else
                        {
                            NearestTarget = Interface::SelectTarget(Map, Party, EnemyId, Interface::SortDistance, Enemies[EnemyId].CanShoot);
                        }
                    }
                    else
                    {
                        NearestTarget = Interface::SelectAllTargets(Map, Party, Enemies, EnemyId, Interface::SortDistance, Enemies[EnemyId].CanShoot);
                    }

                    auto TargetId = Target(NearestTarget);

                    auto TargetType = std::get<0>(NearestTarget);

                    auto LocationX = -1;

                    auto LocationY = -1;

                    auto TargetIsPlayer = TargetType == Map::Object::Player;

                    auto TargetIsEnemy = TargetType == Map::Object::Enemy;

                    if (TargetIsPlayer)
                    {
                        if (TargetId >= 0 && TargetId < Party.Members.size())
                        {
                            Map.Find(Map::Object::Player, TargetId, LocationX, LocationY);
                        }
                    }
                    else if (TargetIsEnemy)
                    {
                        if (TargetId >= 0 && TargetId < Enemies.size())
                        {
                            Map.Find(Map::Object::Enemy, TargetId, LocationX, LocationY);
                        }
                    }

                    if (TargetIsPlayer || TargetIsEnemy)
                    {
                        if (Map.ValidX(LocationX) && Map.ValidY(LocationY) && Map.Distance(EnemyX, EnemyY, LocationX, LocationY) <= 1)
                        {
                            // do attack
                            auto Result = TargetIsPlayer ? Interface::Fight(Renderer, Controls, intBK, Map, Party.Members[TargetId], Enemies[EnemyId], Combat::FightMode::FIGHT, true) : Interface::Fight(Renderer, Controls, intBK, Map, Enemies[EnemyId], Enemies[TargetId], Combat::FightMode::FIGHT);

                            // indicate player last attacked, if successful
                            if (TargetIsPlayer && Result != Combat::Result::UNSUCCESSFUL)
                            {
                                Enemies[EnemyId].Attacked = TargetId;
                            }

                            if (TargetIsPlayer && !Engine::IsAlive(Party.Members[TargetId]))
                            {
                                Interface::RenderMessage(Renderer, Controls, Map, intBK, std::string(Character::ClassName[Party.Members[TargetId].Class]) + " killed!", intBK);

                                Map.Remove(LocationX, LocationY);

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                            else if (TargetIsPlayer && Engine::Paralyzed(Party.Members[TargetId]))
                            {
                                Interface::RenderMessage(Renderer, Controls, Map, intBK, std::string(Character::ClassName[Party.Members[TargetId].Class]) + " paralyzed!", intBK);

                                Map.Remove(LocationX, LocationY);

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }
                            else if (TargetIsEnemy && !Engine::IsAlive(Enemies[TargetId]))
                            {
                                Interface::RenderMessage(Renderer, Controls, Map, intBK, Enemies[TargetId].Name + " killed!", intGR);

                                Map.Remove(LocationX, LocationY);

                                Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                            }

                            if (TargetIsPlayer && Result != Combat::Result::UNSUCCESSFUL)
                            {
                                Engine::ResetSpellDifficulty(Party.Members[TargetId]);
                            }
                        }
                        else
                        {
                            // check if enemy can shoot from range (enemy-specific)
                            if (Enemies[EnemyId].CanMove)
                            {
                                auto CanMove = Interface::CloseDistance(Map, EnemyX, EnemyY, LocationX, LocationY);

                                // move to adjacent tile or close distance
                                auto EnemyPath = AStar::FindPath(Map, EnemyX, EnemyY, LocationX, LocationY, !CanMove);

                                if (EnemyPath.Points.size() > 0)
                                {
                                    Interface::FullMove(Renderer, Controls, intBK, Map, Party, Enemies, EnemyPath, StartMap);

                                    Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                }
                            }
                            else if (Enemies[EnemyId].CanShoot)
                            {
                                auto Result = TargetIsPlayer ? Interface::Fight(Renderer, Controls, intBK, Map, Party.Members[TargetId], Enemies[EnemyId], Combat::FightMode::SHOOT, true) : Interface::Fight(Renderer, Controls, intBK, Map, Enemies[EnemyId], Enemies[TargetId], Combat::FightMode::SHOOT);

                                if (TargetIsPlayer && !Engine::IsAlive(Party.Members[TargetId]))
                                {
                                    Interface::RenderMessage(Renderer, Controls, Map, intBK, std::string(Character::ClassName[Party.Members[TargetId].Class]) + " killed!", intBK);

                                    Map.Remove(LocationX, LocationY);

                                    Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                }
                                else if (TargetIsEnemy && !Engine::IsAlive(Enemies[TargetId]))
                                {
                                    Interface::RenderMessage(Renderer, Controls, Map, intBK, Enemies[TargetId].Name + " killed!", intGR);

                                    Map.Remove(LocationX, LocationY);

                                    Interface::GenerateMapControls(Map, Controls, Party, Enemies, StartMap);
                                }

                                if (TargetIsPlayer && Result != Combat::Result::UNSUCCESSFUL)
                                {
                                    Engine::ResetSpellDifficulty(Party.Members[TargetId]);
                                }
                            }
                        }
                    }

                    CycleCombatants();
                }
                else
                {
                    CycleCombatants();
                }
            }
        }

        auto CombatResult = Combat::Result::NONE;

        if (Exit)
        {
            CombatResult = Combat::Result::NONE;
        }
        else
        {
            ClearPartyStatus();

            // track Enemies who have survived
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

            if (Engine::Paralyzed(Party))
            {
                CombatResult = Combat::Result::DEFEAT;
            }
            else if (Engine::Escaped(Party))
            {
                Story->ExitTriggered = Map.ExitTriggered();

                CombatResult = Combat::Result::ESCAPED;
            }
            else if (Engine::Enthraled(Enemies))
            {
                CombatResult = Combat::Result::ENTHRALED;
            }
            else if (Engine::IsAlive(Party))
            {
                CombatResult = Combat::Result::VICTORY;
            }
            else
            {
                CombatResult = Combat::Result::DEFEAT;
            }
        }

        Enemies.clear();

        Engine::ClearEscaped(Party);

        return CombatResult;
    }
}
#endif
