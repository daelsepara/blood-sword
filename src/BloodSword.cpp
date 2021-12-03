#include "BloodSword.hpp"

bool compare(std::string s1, std::string s2)
{
    //convert s1 and s2 into lower case strings
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

    return (s1.compare(s2) == 0);
}

int main(int argc, const char **argv)
{
    SDL_Window *window = NULL;

    SDL_Renderer *renderer = NULL;

    Graphics::CreateWindow(SDL_INIT_VIDEO | SDL_INIT_AUDIO, &window, &renderer, "Blood Sword");

    Input::InitializeGamePads();

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    TTF_Init();

    Fonts::Initialize();

    if (window && renderer)
    {
        Assets::Load();

        auto story = 1;

        auto combat1 = false;

        auto combat2 = false;

        auto combat3 = false;

        auto convert = false;

        std::string map_file = "";

        if (argc > 1)
        {
            std::string arg = argv[1];

            if (compare(arg, "combat"))
            {
                combat1 = true;
            }
            else if (compare(arg, "shoot"))
            {
                combat2 = true;
            }
            else if (compare(arg, "load"))
            {
                combat3 = true;
            }
            else if (compare(arg, "convert") && argc > 2)
            {
                convert = true;

                map_file = argv[2];
            }
            else
            {
                story = std::atoi(arg.c_str());
            }
        }

        // combat screen
        if (combat1)
        {
            auto party = Interface::CreateParty(window, renderer, Book::Type::Book1);

            if (party.Members.size() > 0)
            {
                auto map = Map::Base();

                auto map_text = map.Read("maps/test.map");

                auto story = Story::Base();

                story.Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 1", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
                story.Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 2", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
                story.Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 3", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
                story.Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 4", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));

                map.Convert(map_text, party, story.Enemies);

                Interface::CombatScreen(window, renderer, &story, map, party);
            }
        }
        else if (combat2)
        {
            auto party = Interface::CreateParty(window, renderer, Book::Type::Book1);

            if (party.Members.size() > 0)
            {
                auto map = Map::Base();

                if (map.Load("maps/book1/map003.json"))
                {
                    auto story = Story::Base();

                    story.Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 1", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin));
                    story.Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 2", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin));
                    story.Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 3", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin));
                    story.Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 4", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin));

                    story.Enemies[0].CanMove = false;
                    story.Enemies[1].CanMove = false;
                    story.Enemies[2].CanMove = false;
                    story.Enemies[3].CanMove = false;
                    story.Enemies[0].CanShoot = true;
                    story.Enemies[1].CanShoot = true;
                    story.Enemies[2].CanShoot = true;
                    story.Enemies[3].CanShoot = true;

                    map.Put(4, 0, Map::Object::Enemy, 0);
                    map.Put(0, 4, Map::Object::Enemy, 1);
                    map.Put(4, 8, Map::Object::Enemy, 2);
                    map.Put(8, 4, Map::Object::Enemy, 3);

                    Interface::CombatScreen(window, renderer, &story, map, party);
                }
            }
        }
        else if (combat3)
        {
            auto party = Interface::CreateParty(window, renderer, Book::Type::Book1);

            if (party.Members.size() > 0)
            {
                auto map = Map::Base();

                if (map.Load("maps/test.json"))
                {
                    auto enemies = std::vector<Enemy::Base>();

                    auto story = Story::Base();

                    story.Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 1", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
                    story.Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 2", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
                    story.Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 3", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
                    story.Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 4", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));

                    Interface::CombatScreen(window, renderer, &story, map, party);
                }
            }
        }
        else if (convert)
        {
            auto party = Party::Base();

            auto map = Map::Base();

            auto map_string = map.Read((map_file).c_str());

            auto enemies = std::vector<Enemy::Base>();

            if (!map_string.empty())
            {
                map.Convert(map_string, party, enemies);

                auto pos = map_file.find_last_of('.');

                if (pos >= 0 && pos < map_file.length())
                {
                    map.Save((map_file.substr(0, pos) + ".json").c_str());
                }
                else
                {
                    map.Save((map_file + ".json").c_str());
                }
            }
        }
        else
        {
            Interface::MainScreen(window, renderer, {Book::Type::Book1, story});
        }

        Assets::Unload();

        if (renderer)
        {
            SDL_DestroyRenderer(renderer);

            renderer = NULL;
        }

        if (window)
        {
            SDL_DestroyWindow(window);

            window = NULL;
        }
    }

    Fonts::Free();

    // Quit SDL Subsystems

    TTF_Quit();

    IMG_Quit();

    if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) & SDL_INIT_GAMECONTROLLER)
    {
        SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    }

    if (SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    if (SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO)
    {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    SDL_Quit();

    return 0;
}
