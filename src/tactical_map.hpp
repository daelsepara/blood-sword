#ifndef __TACTICAL_MAP_HPP__
#define __TACTICAL_MAP_HPP__

#include "BloodSword.hpp"

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

    std::map<TacticalMap::Object, const char *> Description = {
        {TacticalMap::Object::None, "None"},
        {TacticalMap::Object::Player, "Player"},
        {TacticalMap::Object::Passable, "Passable"},
        {TacticalMap::Object::Wall, "Wall"},
        {TacticalMap::Object::Exit, "Exit"},
        {TacticalMap::Object::HotCoals, "Hot Coals"},
        {TacticalMap::Object::Monster, "Monster"}};

    class Base
    {
    public:
        // types of objects in the map
        std::vector<std::vector<TacticalMap::Object>> Objects = {};

        // object ID number
        std::vector<std::vector<int>> ObjectID = {};

        // locations of certain objects
        std::vector<TacticalMap::Point> HotCoals = {};

        std::vector<TacticalMap::Point> Exits = {};

        // map dimensions
        int Width = 0;

        int Height = 0;

        // viewable size
        int SizeX = -1;

        int SizeY =-1;

        // offsets (drawing)
        int DrawX = -1;

        int DrawY = -1;

        // offsets (navigation)
        int MapX = -1;

        int MapY = -1;

        // object size in pixels (width, height)
        int ObjectSize = 0;

        void Initialize(int sizex, int sizey)
        {
            Width = sizex;

            Height = sizey;

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

                            Exits.push_back(std::make_pair(x, y));
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
                            Objects[y][x] = TacticalMap::Object::Passable;
                            
                            ObjectID[y][x] = 0;
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