#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "party.hpp"

namespace Map
{
    const char HotCoals = '%';
    const char Wall = '#';
    const char Flee = '=';
    const char Passable = ' ';
    const char Stairs = '/';
    const char WindowMystical = '@';

    std::string Players = "1234";
    std::string Enemies = "ABCDEFGHIJ";

    typedef std::pair<int, int> Point;

    enum class Object
    {
        None = 0,
        Player,
        Passable,
        Wall,
        Exit,
        HotCoals,
        Stairs,
        Enemy,
        WindowMystical
    };

    std::map<Map::Object, const char *> Description = {
        {Map::Object::None, "None"},
        {Map::Object::Player, "Player"},
        {Map::Object::Passable, "Passable"},
        {Map::Object::Wall, "Wall"},
        {Map::Object::Exit, "Exit"},
        {Map::Object::HotCoals, "Hot Coals"},
        {Map::Object::Enemy, "Enemy"},
        {Map::Object::Stairs, "Stairs (Exit)"},
        {Map::Object::WindowMystical, "Window"}};

    class Tile
    {
    public:
        Map::Object Type = Map::Object::None;

        Map::Object Occupant = Map::Object::None;

        Assets::Type Asset = Assets::Type::None;

        bool IsPassable = false;

        bool IsPassableToEnemy = false;

        int Id = 0;

        Tile()
        {
        }

        bool IsEnemy()
        {
            return Occupant == Map::Object::Enemy;
        }

        bool IsPlayer()
        {
            return Occupant == Map::Object::Player;
        }

        bool IsExit()
        {
            return (Type == Map::Object::Exit || Type == Map::Object::Stairs);
        }

        bool IsOccupied()
        {
            return Occupant != Map::Object::None;
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

            Exits.clear();
        }

        void Convert(std::vector<std::string> &map, Party::Base &party, std::vector<Enemy::Base> &enemies)
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

                        auto enemy = Map::Enemies.find(map[y][x]);

                        if (map[y][x] == Map::Wall)
                        {
                            Tiles[y][x].Asset = Assets::Type::Wall;
                            Tiles[y][x].Type = Map::Object::Wall;
                        }
                        else if (map[y][x] == Map::HotCoals)
                        {
                            Tiles[y][x].Asset = Assets::Type::HotCoals;
                            Tiles[y][x].Type = Map::Object::HotCoals;
                            Tiles[y][x].IsPassableToEnemy = true;
                        }
                        else if (map[y][x] == Map::WindowMystical)
                        {
                            Tiles[y][x].Asset = Assets::Type::WindowMystical;
                            Tiles[y][x].Type = Map::Object::WindowMystical;
                            Tiles[y][x].IsPassableToEnemy = true;
                        }
                        else if (map[y][x] == Map::Flee)
                        {
                            Tiles[y][x].Asset = Assets::Type::MapExit;
                            Tiles[y][x].Type = Map::Object::Exit;

                            Exits.push_back(std::make_pair(x, y));
                        }
                        else if (map[y][x] == Map::Stairs)
                        {
                            Tiles[y][x].Asset = Assets::Type::Stairs;
                            Tiles[y][x].Type = Map::Object::Stairs;

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
                            Tiles[y][x].IsPassableToEnemy = true;
                            Tiles[y][x].Id = player + 1;
                        }
                        else if (enemy != std::string::npos && enemy >= 0 && enemy < enemies.size())
                        {
                            Tiles[y][x].Asset = Assets::Type::Passable;
                            Tiles[y][x].Type = Map::Object::Passable;
                            Tiles[y][x].Occupant = Map::Object::Enemy;
                            Tiles[y][x].IsPassable = true;
                            Tiles[y][x].IsPassableToEnemy = true;
                            Tiles[y][x].Id = enemy + 1;
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

        Base(std::vector<std::string> map, Party::Base &party, std::vector<Enemy::Base> &enemies)
        {
            if (map.size() > 0)
            {
                auto sizey = map.size();

                auto sizex = map.front().size();

                Initialize(sizex, sizey);

                Convert(map, party, enemies);
            }
        }

        void Put(int x, int y, Map::Object object, int id)
        {
            if (x >= 0 && x < Width && y >= 0 && y < Height)
            {
                this->Tiles[y][x].Occupant = object;
                this->Tiles[y][x].Id = id + 1;
            }
        }

        void Put(int x, int y, Map::Object type, Assets::Type asset, bool isPassable, bool isPassableToEnemy)
        {
            if (x >= 0 && x < Width && y >= 0 && y < Height)
            {
                this->Tiles[y][x].Occupant = Map::Object::None;
                this->Tiles[y][x].Type = type;
                this->Tiles[y][x].Asset = asset;
                this->Tiles[y][x].IsPassable = isPassable;
                this->Tiles[y][x].IsPassableToEnemy = isPassableToEnemy;
            }
        }

        void Load(const char *filename, Party::Base &party, std::vector<Enemy::Base> &enemies)
        {
            std::vector<std::string> map = {};

            std::fstream file(filename);

            std::string str;

            if (file.is_open())
            {
                while (std::getline(file, str))
                {
                    map.push_back(str);
                }

                file.close();
            }

            if (map.size() > 0)
            {
                auto sizey = map.size();

                auto sizex = map.front().size();

                Initialize(sizex, sizey);

                Convert(map, party, enemies);
            }
        }

        std::vector<std::string> Read(const char *filename)
        {
            std::vector<std::string> map = {};

            std::fstream file(filename);

            std::string str;

            if (file.is_open())
            {
                while (std::getline(file, str))
                {
                    map.push_back(str);
                }

                file.close();
            }

            return map;
        }

        // load from json file
        bool Load(const char *filename)
        {
            auto LoadError = false;

            std::ifstream file(filename);

            if (file.good())
            {
                auto data = nlohmann::json::parse(file);

                this->Width = !data["width"].is_null() ? (int)data["width"] : 0;

                this->Height = !data["height"].is_null() ? (int)data["height"] : 0;

                auto LoadError = false;

                if (this->Width > 0 && this->Height > 0)
                {
                    this->Initialize(this->Width, this->Height);

                    if (!data["tiles"].is_null() && data["tiles"].is_array() && data["tiles"].size() == this->Height)
                    {
                        for (auto y = 0; y < this->Height; y++)
                        {
                            if (!data["tiles"][y].is_null() && data["tiles"][y].is_array() && data["tiles"][y].size() == this->Width)
                            {
                                for (auto x = 0; x < this->Width; x++)
                                {
                                    this->Tiles[y][x].Type = !data["tiles"][y][x]["type"].is_null() ? static_cast<Map::Object>((int)data["tiles"][y][x]["type"]) : Map::Object::None;
                                    this->Tiles[y][x].Occupant = !data["tiles"][y][x]["occupant"].is_null() ? static_cast<Map::Object>((int)data["tiles"][y][x]["occupant"]) : Map::Object::None;
                                    this->Tiles[y][x].Asset = !data["tiles"][y][x]["asset"].is_null() ? static_cast<Assets::Type>((int)data["tiles"][y][x]["asset"]) : Assets::Type::None;
                                    this->Tiles[y][x].IsPassable = !data["tiles"][y][x]["isPassable"].is_null() ? (bool)data["tiles"][y][x]["isPassable"] : false;
                                    this->Tiles[y][x].IsPassableToEnemy = !data["tiles"][y][x]["isPassableToEnemy"].is_null() ? (bool)data["tiles"][y][x]["isPassableToEnemy"] : false;
                                    this->Tiles[y][x].Id = !data["tiles"][y][x]["id"].is_null() ? (int)data["tiles"][y][x]["id"] : 0;
                                }
                            }
                            else
                            {
                                LoadError = true;

                                break;
                            }
                        }
                    }
                    else
                    {
                        LoadError = true;
                    }

                    if (!data["exits"].is_null() && data["exits"].is_array() && data["exits"].size() > 0)
                    {
                        for (auto i = 0; i < data["exits"].size(); i++)
                        {
                            auto x = !data["exits"][i]["x"].is_null() ? (int)data["exits"][i]["x"] : -1;
                            auto y = !data["exits"][i]["y"].is_null() ? (int)data["exits"][i]["y"] : -1;

                            if (x >= 0 && x < this->Width && y >= 0 && y < this->Height)
                            {
                                this->Exits.push_back({x, y});
                            }
                            else
                            {
                                LoadError = true;
                            }
                        }
                    }
                }
                else
                {
                    LoadError = true;
                }

                file.close();
            }
            else
            {
                LoadError = true;
            }

            return !LoadError;
        }

        // save into a json file
        void Save(const char *filename)
        {
            nlohmann::json map;

            map["width"] = this->Width;

            map["height"] = this->Height;

            nlohmann::json tiles;

            for (auto y = 0; y < Height; y++)
            {
                std::vector<nlohmann::json> row;

                for (auto x = 0; x < Width; x++)
                {
                    nlohmann::json tile;

                    tile.emplace("type", this->Tiles[y][x].Type);
                    tile.emplace("occupant", this->Tiles[y][x].Occupant);
                    tile.emplace("asset", this->Tiles[y][x].Asset);
                    tile.emplace("isPassable", this->Tiles[y][x].IsPassable);
                    tile.emplace("isPassableToEnemy", this->Tiles[y][x].IsPassableToEnemy);
                    tile.emplace("id", this->Tiles[y][x].Id);

                    row.push_back(tile);
                }

                map["tiles"].push_back(row);
            }

            for (auto i = 0; i < this->Exits.size(); i++)
            {
                nlohmann::json exit;

                exit.emplace("x", this->Exits[i].first);
                exit.emplace("y", this->Exits[i].second);

                map["exits"].push_back(exit);
            }

            std::ofstream file(filename);

            if (file.is_open())
            {
                file << map.dump();

                file.close();
            }
        }

        void Clean(Party::Base &party, std::vector<Enemy::Base> &enemies)
        {
            for (auto y = 0; y < this->Height; y++)
            {
                for (auto x = 0; x < this->Width; x++)
                {
                    Map::Object &Occupant = Tiles[y][x].Occupant;
                    
                    int &Id =  Tiles[y][x].Id;

                    if (Occupant == Map::Object::Player && Id > party.Members.size())
                    {
                        Occupant = Map::Object::None;

                        Id = 0;
                    }
                    else if (Occupant == Map::Object::Enemy && Id > enemies.size())
                    {
                        Occupant = Map::Object::None;

                        Id = 0;
                    }
                }
            }
        }
    };
}
#endif