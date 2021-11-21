#ifndef __ASSETS__HPP__
#define __ASSETS__HPP__
#endif

#include <string>
#include <utility>

#include "graphics.hpp"

namespace Assets
{
    enum class Type
    {
        None = 0,
        Warrior,
        Trickster,
        Sage,
        Enchanter,
        Passable,
        Wall,
        Exit,
        HotCoals,
        Up = 100,
        Left,
        Right,
        Down,
        Back,
        Ability,
        Move,
        Heal,
        Attack,
        Defend,
        Shoot,
        Memorize,
        Magic,
        Flee,
        Items,
        QuickThinking,
        Ok = 150,
        Cancel,
        Dice1,
        Dice2,
        Dice3,
        Dice4,
        Dice5,
        Dice6,
        Cast = 200,
        VolcanoSpray,
        Nighthowl,
        WhiteFire,
        Swordthrust,
        EyeOfTheTiger,
        ImmediateDeliverance,
        MistsOfDeath,
        TheVampireSpell,
        SheetLightning,
        GhastlyTouch,
        NemesisBolt,
        ServileEnthralment,
        Monster = 300,
        Barbarian
    };

    typedef std::pair<Assets::Type, SDL_Surface *> Surface;
    typedef std::pair<Assets::Type, std::string> Location;

    std::vector<Assets::Location> Path = {};
    std::vector<Assets::Surface> Graphics = {};

    void Unload()
    {
        if (Assets::Graphics.size() > 0)
        {
            for (auto i = 0; i < Assets::Graphics.size(); i++)
            {
                if (Assets::Graphics[i].second && Assets::Graphics[i].second)
                {
                    SDL_FreeSurface(Assets::Graphics[i].second);

                    Assets::Graphics[i].second = NULL;
                }
            }

            Assets::Graphics.clear();

            Assets::Path.clear();
        }
    }

    bool Load()
    {
        auto result = false;

        Assets::Unload();

        Assets::Path.clear();

        Assets::Graphics.clear();

        std::ifstream ifs("assets.json");

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["assets"].is_null() && data["assets"].is_array() && data["assets"].size() > 0)
            {
                for (auto i = 0; i < data["assets"].size(); i++)
                {
                    auto object = !data["assets"][i]["id"].is_null() ? static_cast<Assets::Type>((int)data["assets"][i]["id"]) : Assets::Type::None;

                    auto path = !data["assets"][i]["path"].is_null() ? std::string(data["assets"][i]["path"]) : "";

                    if (!path.empty() && object != Assets::Type::None)
                    {
                        auto surface = Graphics::CreateImage(path.c_str());

                        if (surface)
                        {
                            Assets::Path.push_back(std::make_pair(object, path));

                            Assets::Graphics.push_back(std::make_pair(object, surface));
                        }
                    }
                }
            }

            ifs.close();

            result = !Path.empty() && !Graphics.empty() && (Graphics.size() == Path.size());
        }
        else
        {
            result = false;
        }

        return result;
    }

    SDL_Surface *Get(Assets::Type asset, Uint8 alpha)
    {
        SDL_Surface *surface = NULL;

        if (Assets::Graphics.size() > 0)
        {
            for (auto i = 0; i < Graphics.size(); i++)
            {
                if (Assets::Graphics[i].first == asset)
                {
                    // return a copy of the asset
                    surface = Assets::Graphics[i].second;

                    break;
                }
            }
        }

        SDL_SetSurfaceColorMod(surface, alpha, alpha, alpha);

        return surface;
    }

    SDL_Surface *Get(Assets::Type asset)
    {
        return Assets::Get(asset, 0xFF);
    }

    SDL_Surface *Copy(SDL_Surface *Surface)
    {
        return SDL_ConvertSurface(Surface, Surface->format, 0);
    }

    SDL_Surface *Copy(Assets::Type asset)
    {
        return Assets::Copy(Assets::Get(asset));
    }

    SDL_Surface *Copy(Assets::Type asset, Uint8 alpha)
    {
        return Assets::Copy(Assets::Get(asset, alpha));
    }
}