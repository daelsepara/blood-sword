#ifndef __TACTICAL_MAP_HPP__
#define __TACTICAL_MAP_HPP__

#include <map>

#include <string>
#include <vector>

#include "monster.hpp"
#include "party.hpp"

namespace TacticalMap
{
    const char HotCoals = '%';
    const char Wall = '#';
    const char Flee = 'F';
    const char Passable = ' ';

    std::string Players = "1234";
    std::string Monsters = "ABCDEFGHIJ";

    typedef std::pair<int, int> Point;

    enum class Object
    {
        None = 0,
        Player,
        Passable,
        Wall,
        Exit,
        HotCoals,
        Monster
    };

    class Base
    {
    public:
        // Types of objects in the map
        std::vector<std::vector<TacticalMap::Object>> Objects = {};

        // Object ID number
        std::vector<std::vector<int>> ObjectID = {};

        // Tactical Map Dimensions
        int SizeX = 0;

        int SizeY = 0;

        std::vector<TacticalMap::Point> HotCoals = {};

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

                            HotCoals.push_back(std::make_pair(x, y));
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
                            Objects[y][x] = TacticalMap::Object::Player;

                            ObjectID[y][x] = player + 1;
                        }
                        else if (monster != std::string::npos && monster >= 0 && monster < monsters.size())
                        {
                            Objects[y][x] = TacticalMap::Object::Monster;

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
            if (map.size() > 0)
            {
                auto sizey = map.size();
                auto sizex = map.front().size();

                Initialize(sizex, sizey);

                Convert(map, party, monsters);
            }
        }
    };
}
#endif