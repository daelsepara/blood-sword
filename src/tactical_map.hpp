#ifndef __TACTICAL_MAP_HPP__
#define __TACTICAL_MAP_HPP__

#include "BloodSword.hpp"

namespace TacticalMap
{
    const char HotCoals = '%';
    const char Wall = '#';
    const char Flee = '=';
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

    class MapObject
    {
    public:
        TacticalMap::Object Type = TacticalMap::Object::None;

        TacticalMap::Object Occupant = TacticalMap::Object::None;

        Assets::Type Asset = Assets::Type::None;

        bool IsPlayer = false;

        bool IsEnemy = false;

        bool IsPassable = false;

        bool IsPassableToEnemy = false;

        bool IsExit = false;

        int Id = 0;

        MapObject()
        {
        }
    };

    class Base
    {
    public:
        // types of objects in the map
        std::vector<std::vector<TacticalMap::MapObject>> Tiles = {};

        // locations of exits
        std::vector<TacticalMap::Point> Exits = {};

        // map dimensions
        int Width = 0;

        int Height = 0;

        // viewable size
        int SizeX = -1;

        int SizeY = -1;

        // offsets (drawing)
        int DrawX = -1;

        int DrawY = -1;

        // offsets (navigation)
        int MapX = -1;

        int MapY = -1;

        // object size in pixels (width, height)
        int ObjectSize = 0;

        // text coordinates
        int TextX = 0;

        int TextY = 0;

        int TextWidth = 0;

        int TextRightX = 0;

        int TextRightWidth = 0;

        void Initialize(int sizex, int sizey)
        {
            Width = sizex;

            Height = sizey;

            Tiles.clear();

            Tiles.resize(sizey);

            for (auto i = 0; i < sizey; i++)
            {
                Tiles[i] = std::vector<TacticalMap::MapObject>(sizex);
            }
        }

        void Convert(std::vector<std::string> &map, Party::Base &party, std::vector<Monster::Base> &monsters)
        {
            if (map.size() > 0)
            {
                if (Tiles.size() == 0 || Tiles.front().size() == 0)
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
                            Tiles[y][x].Asset = Assets::Type::Wall;
                            Tiles[y][x].Type = TacticalMap::Object::Wall;
                            Tiles[y][x].IsPassable = false;
                        }
                        else if (map[y][x] == TacticalMap::HotCoals)
                        {
                            Tiles[y][x].Asset = Assets::Type::HotCoals;
                            Tiles[y][x].Type = TacticalMap::Object::HotCoals;
                            Tiles[y][x].IsPassable = false;
                            Tiles[y][x].IsPassableToEnemy = true;
                        }
                        else if (map[y][x] == TacticalMap::Flee)
                        {
                            Tiles[y][x].Asset = Assets::Type::MapExit;
                            Tiles[y][x].Type = TacticalMap::Object::Exit;
                            Tiles[y][x].IsPassable = false;
                            Tiles[y][x].IsExit = true;

                            Exits.push_back(std::make_pair(x, y));
                        }
                        else if (map[y][x] == TacticalMap::Passable)
                        {
                            Tiles[y][x].Asset = Assets::Type::Passable;
                            Tiles[y][x].Type = TacticalMap::Object::Passable;
                            Tiles[y][x].IsPassable = true;
                            Tiles[y][x].IsPassableToEnemy = true;
                        }
                        else if (player != std::string::npos && player >= 0 && player < party.Members.size())
                        {
                            Tiles[y][x].Asset = Assets::Type::Passable;
                            Tiles[y][x].Type = TacticalMap::Object::Passable;
                            Tiles[y][x].Occupant = TacticalMap::Object::Player;
                            Tiles[y][x].IsPassable = true;
                            Tiles[y][x].IsPlayer = true;
                            Tiles[y][x].Id = player + 1;
                        }
                        else if (monster != std::string::npos && monster >= 0 && monster < monsters.size())
                        {
                            Tiles[y][x].Asset = Assets::Type::Passable;
                            Tiles[y][x].Type = TacticalMap::Object::Passable;
                            Tiles[y][x].Occupant = TacticalMap::Object::Monster;
                            Tiles[y][x].IsPassable = true;
                            Tiles[y][x].IsEnemy = true;
                            Tiles[y][x].Id = monster + 1;
                        }
                        else
                        {
                            Tiles[y][x].Asset = Assets::Type::Passable;
                            Tiles[y][x].Type = TacticalMap::Object::Passable;
                            Tiles[y][x].IsPassable = true;
                            Tiles[y][x].IsPassableToEnemy = true;
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