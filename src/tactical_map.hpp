#ifndef __TACTICAL_MAP_HPP__
#define __TACTICAL_MAP_HPP__

#include <map>

#include <string>
#include <vector>

#include "party.hpp"

namespace TacticalMap
{
    const char HotCoals = '%';
    const char Wall = '#';
    const char Flee = 'F';
    const char Passable = ' ';

    std::string Players = "1234";
    std::string Monsters = "ABCDEFGHIJ";

    enum class Object
    {
        None = 0,
        Warrior,
        Sage,
        Trickster,
        Enchanter,
        Passable,
        Wall,
        Exit,
        HotCoals,
        Monster = 100,
        Barbarian
    };

    class Base
    {
    public:
        // Types of objects in the map
        std::vector<std::vector<TacticalMap::Object>> Objects = {};

        // Object ID number
        std::vector<std::vector<int>> ObjectID = {};

        // String representation for use with A*
        std::vector<std::string> Map;

        // Tactical Map Dimensions
        int SizeX = 0;

        int SizeY = 0;

        void Initialize(int sizex, int sizey)
        {
            SizeX = sizex;

            SizeY = sizey;

            Objects.clear();
            Objects.resize(sizey);

            ObjectID.clear();
            ObjectID.resize(sizey);

            for (auto i = 0; i < sizey; i++)
            {
                Objects[i] = std::vector<TacticalMap::Object>(sizex);

                ObjectID[i] = std::vector<int>(sizex);
            }
        }

        void Convert(std::vector<std::string> &map, Party::Base &party, std::vector<Monster::Base> &monsters)
        {
            if (map.size() > 0)
            {
                if (Objects.size() == 0 || Objects.front().size() == 0)
                {
                    Initialize(map.front().length(), map.size());
                }

                for (auto y = 0; y < map.size(); y++)
                {
                    for (auto x = 0; x < map.front().length(); x++)
                    {
                        auto player = TacticalMap::Players.find(map[y][x]);
                        auto monster = TacticalMap::Monsters.find(map[y][x]);

                        if (map[y][x] == TacticalMap::Wall)
                        {
                            Objects[y][x] = TacticalMap::Object::Wall;
                        }
                        else if (map[y][x] == TacticalMap::HotCoals)
                        {
                            Objects[y][x] = TacticalMap::Object::HotCoals;
                        }
                        else if (map[y][x] == TacticalMap::Flee)
                        {
                            Objects[y][x] = TacticalMap::Object::Exit;
                        }
                        else if (map[y][x] == TacticalMap::Passable)
                        {
                            Objects[y][x] = TacticalMap::Object::Passable;
                        }
                        else if (player != std::string::npos && player >= 0 && player < party.Members.size())
                        {
                            if (party.Members[player].Class == Character::Class::Warrior)
                            {
                                Objects[y][x] = TacticalMap::Object::Warrior;
                            }
                            else if (party.Members[player].Class == Character::Class::Sage)
                            {
                                Objects[y][x] = TacticalMap::Object::Sage;
                            }
                            else if (party.Members[player].Class == Character::Class::Trickster)
                            {
                                Objects[y][x] = TacticalMap::Object::Trickster;
                            }
                            else if (party.Members[player].Class == Character::Class::Enchanter)
                            {
                                Objects[y][x] = TacticalMap::Object::Enchanter;
                            }
                            else
                            {
                                Objects[y][x] = TacticalMap::Object::None;
                            }

                            ObjectID[y][x] = player + 1;
                        }
                        else if (monster != std::string::npos && monster >= 0 && monster < monsters.size())
                        {
                            if (monsters[monster].Type == Monster::Type::Barbarian)
                            {
                                Objects[y][x] = TacticalMap::Object::Barbarian;
                            }
                            else
                            {
                                Objects[y][x] = TacticalMap::Object::Monster;
                            }

                            ObjectID[y][x] = monster + 1;
                        }
                        else
                        {
                            Objects[y][x] = TacticalMap::Object::None;
                        }
                    }
                }
            }
        }

        Base()
        {
        }

        Base(int sizex, int sizey)
        {
            Initialize(sizex, sizey);
        }

        Base(std::vector<std::string> map, Party::Base &party, std::vector<Monster::Base> &monsters)
        {
            Map = map;

            if (map.size() > 0)
            {
                auto sizey = map.size();
                auto sizex = map.front().size();

                Initialize(sizex, sizey);

                Convert(map, party, monsters);
            }
        }

        TacticalMap::Object Object(int X, int Y)
        {
            if (X >= 0 && X < SizeX && Y >= 0 && Y < SizeY)
            {
                return Objects[Y][X];
            }
            else
            {
                return TacticalMap::Object::None;
            }
        }
    };
}
#endif