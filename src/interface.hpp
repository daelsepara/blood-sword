#ifndef __INTERFACE__HPP__
#define __INTERFACE__HPP__

#include "BloodSword.hpp"

#include "graphics.hpp"

namespace Interface
{
    typedef std::pair<TacticalMap::Object, SDL_Surface *> AssetSurface;
    typedef std::pair<TacticalMap::Object, std::string> AssetPath;

    std::vector<Interface::AssetPath> AssetPaths = {};
    std::vector<Interface::AssetSurface> AssetGraphics = {};

    void UnloadMapAssets()
    {
        if (AssetGraphics.size() > 0)
        {
            for (auto i = 0; i < AssetGraphics.size(); i++)
            {
                if (AssetGraphics[i].second && AssetGraphics[i].second)
                {
                    SDL_FreeSurface(AssetGraphics[i].second);

                    AssetGraphics[i].second = NULL;
                }
            }

            AssetGraphics.clear();

            AssetPaths.clear();
        }
    }

    bool LoadMapAssets(const char *file_name)
    {
        Interface::UnloadMapAssets();

        auto result = false;

        AssetPaths.clear();

        AssetGraphics.clear();

        std::ifstream ifs(file_name);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["assets"].is_null() && data["assets"].is_array() && data["assets"].size() > 0)
            {
                for (auto i = 0; i < data["assets"].size(); i++)
                {
                    auto object = !data["assets"][i]["id"].is_null() ? static_cast<TacticalMap::Object>((int)data["assets"][i]["id"]) : TacticalMap::Object::None;
                    auto path = !data["assets"][i]["path"].is_null() ? std::string(data["assets"][i]["path"]) : "";

                    if (!path.empty() && object != TacticalMap::Object::None)
                    {
                        auto surface = Graphics::CreateImage(path.c_str());

                        if (surface)
                        {
                            AssetPaths.push_back(std::make_pair(object, path));

                            AssetGraphics.push_back(std::make_pair(object, surface));
                        }
                    }
                }
            }

            ifs.close();

            result = !AssetPaths.empty() && !AssetGraphics.empty() && (AssetGraphics.size() == AssetPaths.size());
        }
        else
        {
            result = false;
        }

        return result;
    }

    SDL_Surface *GetAsset(TacticalMap::Object object)
    {
        SDL_Surface *surface = NULL;

        if (AssetGraphics.size() > 0)
        {
            for (auto i = 0; i < AssetGraphics.size(); i++)
            {
                if (AssetGraphics[i].first == object)
                {
                    surface = AssetGraphics[i].second;

                    break;
                }
            }
        }

        return surface;
    }

    bool Combat(SDL_Window *window, SDL_Renderer *renderer, std::vector<std::string> &map, Party::Base &party, std::vector<Monster::Base> &monsters)
    {
        auto TacticalMap = TacticalMap::Base();

        TacticalMap.Convert(map, party, monsters);

        // Offsets used to display tactical map
        auto MapX = 0;

        auto MapY = 0;

        auto ObjectSize = 64;

        // Size of viewable grid
        auto SizeX = (SCREEN_WIDTH - 2 * (2 * startx + ObjectSize)) / ObjectSize;

        auto SizeY = (SCREEN_HEIGHT - 2 * (3 * starty + ObjectSize)) / ObjectSize;

        if (SizeX > TacticalMap.SizeX)
        {
            SizeX = TacticalMap.SizeX;
        }

        if (SizeY > TacticalMap.SizeY)
        {
            SizeY = TacticalMap.SizeY;
        }

        auto DrawX = (SCREEN_WIDTH - SizeX * ObjectSize) / 2;

        auto DrawY = (SCREEN_HEIGHT - SizeY * ObjectSize) / 2;

        if (window && renderer && TacticalMap.SizeX > 0 && TacticalMap.SizeY > 0)
        {
            Graphics::FillWindow(renderer, intBE);

            if (MapX < 0)
            {
                MapX = 0;
            }

            if (MapX > TacticalMap.SizeX - SizeX)
            {
                MapX = TacticalMap.SizeX - SizeX;
            }

            if (MapY < 0)
            {
                MapY = 0;
            }

            if (MapY > TacticalMap.SizeY - SizeY)
            {
                MapY = TacticalMap.SizeY - SizeY;
            }

            // Drap Map Window Borders
            Graphics::FillRect(renderer, SCREEN_WIDTH - 4 * startx, ObjectSize, 2 * startx, 3 * starty, intBR);
            Graphics::FillRect(renderer, SCREEN_WIDTH - 4 * startx, ObjectSize, 2 * startx, SCREEN_HEIGHT - 3 * starty - ObjectSize, intBR);
            Graphics::FillRect(renderer, ObjectSize, SCREEN_HEIGHT - 6 * starty - 2 * ObjectSize, 2 * startx, 3 * starty + ObjectSize, intBR);
            Graphics::FillRect(renderer, ObjectSize, SCREEN_HEIGHT - 6 * starty - 2 * ObjectSize, SCREEN_WIDTH - 2 * startx - ObjectSize, 3 * starty + ObjectSize, intBR);

            // Render Objects within the Map Window
            for (auto y = MapY; y < MapY + SizeY; y++)
            {
                for (auto x = MapX; x < MapX + SizeX; x++)
                {
                    if (TacticalMap.Objects[y][x] == TacticalMap::Object::Wall)
                    {
                        Graphics::FillRect(renderer, ObjectSize, ObjectSize, DrawX + (x - MapX) * ObjectSize, DrawY + (y - MapY) * ObjectSize, intBK);
                    }

                    if (TacticalMap.Objects[y][x] == TacticalMap::Object::Warrior)
                    {
                        auto surface = Interface::GetAsset(TacticalMap::Object::Warrior);

                        if (surface)
                        {
                            Graphics::StretchImage(renderer, surface, DrawX + (x - MapX) * ObjectSize, DrawY + (y - MapY) * ObjectSize, ObjectSize, ObjectSize);
                        }
                    }
                }
            }

            Input::WaitForNext(renderer);
        }

        return false;
    }
}
#endif