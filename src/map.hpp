#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "BloodSword.hpp"

namespace Map
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

    std::map<Map::Object, const char *> Description = {
        {Map::Object::None, "None"},
        {Map::Object::Player, "Player"},
        {Map::Object::Passable, "Passable"},
        {Map::Object::Wall, "Wall"},
        {Map::Object::Exit, "Exit"},
        {Map::Object::HotCoals, "Hot Coals"},
        {Map::Object::Monster, "Monster"}};

    class Tile
    {
    public:
        Map::Object Type = Map::Object::None;

        Map::Object Occupant = Map::Object::None;

        Assets::Type Asset = Assets::Type::None;

        bool IsPlayer = false;

        bool IsEnemy = false;

        bool IsPassable = false;

        bool IsPassableToEnemy = false;

        bool IsExit = false;

        int Id = 0;

        Tile()
        {
        }
    };

    class Base
    {
    public:
        // types of objects in the map
        std::vector<std::vector<Map::Tile>> Tiles = {};

        // locations of exits
        std::vector<Map::Point> Exits = {};

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
                Tiles[i] = std::vector<Map::Tile>(sizex);
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
                        auto player = Map::Players.find(map[y][x]);

                        auto monster = Map::Monsters.find(map[y][x]);

                        if (map[y][x] == Map::Wall)
                        {
                            Tiles[y][x].Asset = Assets::Type::Wall;
                            Tiles[y][x].Type = Map::Object::Wall;
                            Tiles[y][x].IsPassable = false;
                        }
                        else if (map[y][x] == Map::HotCoals)
                        {
                            Tiles[y][x].Asset = Assets::Type::HotCoals;
                            Tiles[y][x].Type = Map::Object::HotCoals;
                            Tiles[y][x].IsPassable = false;
                            Tiles[y][x].IsPassableToEnemy = true;
                        }
                        else if (map[y][x] == Map::Flee)
                        {
                            Tiles[y][x].Asset = Assets::Type::MapExit;
                            Tiles[y][x].Type = Map::Object::Exit;
                            Tiles[y][x].IsPassable = false;
                            Tiles[y][x].IsExit = true;

                            Exits.push_back(std::make_pair(x, y));
                        }
                        else if (map[y][x] == Map::Passable)
                        {
                            Tiles[y][x].Asset = Assets::Type::Passable;
                            Tiles[y][x].Type = Map::Object::Passable;
                            Tiles[y][x].IsPassable = true;
                            Tiles[y][x].IsPassableToEnemy = true;
                        }
                        else if (player != std::string::npos && player >= 0 && player < party.Members.size())
                        {
                            Tiles[y][x].Asset = Assets::Type::Passable;
                            Tiles[y][x].Type = Map::Object::Passable;
                            Tiles[y][x].Occupant = Map::Object::Player;
                            Tiles[y][x].IsPassable = true;
                            Tiles[y][x].IsPlayer = true;
                            Tiles[y][x].Id = player + 1;
                        }
                        else if (monster != std::string::npos && monster >= 0 && monster < monsters.size())
                        {
                            Tiles[y][x].Asset = Assets::Type::Passable;
                            Tiles[y][x].Type = Map::Object::Passable;
                            Tiles[y][x].Occupant = Map::Object::Monster;
                            Tiles[y][x].IsPassable = true;
                            Tiles[y][x].IsEnemy = true;
                            Tiles[y][x].Id = monster + 1;
                        }
                        else
                        {
                            Tiles[y][x].Asset = Assets::Type::Passable;
                            Tiles[y][x].Type = Map::Object::Passable;
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