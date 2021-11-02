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

    bool UnloadMapAssets()
    {
        auto result = true;

        try
        {
            for (auto i = 0; Interface::AssetGraphics.size(); i++)
            {
                if (AssetGraphics[i].second)
                {
                    SDL_FreeSurface(AssetGraphics[i].second);

                    AssetGraphics[i].second = NULL;
                }
            }

            AssetGraphics.clear();

            AssetPaths.clear();
        }
        catch (std::exception &ex)
        {
            result = false;
        }

        return result;
    }

    bool LoadMapAssets(const char *file_name)
    {
        bool result = true;

        if (!AssetGraphics.empty())
        {
            if (!UnloadMapAssets())
            {
                result = false;
            }
        }

        if (result)
        {
            AssetPaths.clear();

            AssetGraphics.clear();

            std::ifstream ifs(file_name);

            if (ifs.good())
            {
                auto data = nlohmann::json::parse(ifs);

                if (!data["assets"].is_null() && data["assets"].is_array() && data["assets"].size() > 0)
                {
                    for (auto i = 0; i < data["members"].size(); i++)
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
        }

        return result;
    }

    bool Combat(SDL_Window *window, SDL_Renderer *renderer, std::vector<std::string> &map, Party::Base &party, std::vector<Monster::Base> &monsters)
    {
        auto TacticalMap = TacticalMap::Base();

        TacticalMap.Convert(map, party, monsters);

        // Offsets used to display tactical map
        auto MapX = 0;

        auto MapY = 0;

        // Size of viewable grid
        auto SizeX = 10;

        auto SizeY = 10;

        if (window && renderer)
        {
            Graphics::FillWindow(renderer, intBE);

            if (MapX < 0)
            {
                MapX = 0;
            }

            if (MapX > (TacticalMap.SizeX - 1) - SizeX)
            {
                MapX = (TacticalMap.SizeX - 1) - SizeX;
            }

            if (MapY < 0)
            {
                MapY = 0;
            }

            if (MapY > (TacticalMap.SizeY - 1) - SizeX)
            {
                MapY = (TacticalMap.SizeY - 1) - SizeY;
            }

            Input::WaitForNext(renderer);
        }

        return false;
    }
}
#endif