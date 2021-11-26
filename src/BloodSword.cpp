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
        // setup party, enemies, and map
        auto party = Party::Base();

        party.Members.push_back(Character::Create(Character::Class::Warrior, 2));
        party.Members.push_back(Character::Create(Character::Class::Trickster, 2));
        party.Members.push_back(Character::Create(Character::Class::Sage, 2));
        party.Members.push_back(Character::Create(Character::Class::Enchanter, 2));

        auto enemies = std::vector<Enemy::Base>();

        enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 1", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
        enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 2", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
        enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 3", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
        enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 4", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));

        Assets::Load();

        auto story = 1;

        auto combat1 = false;

        auto combat2 = false;

        if (argc > 1)
        {
            std::string arg = argv[1];

            if (compare(arg, "combat"))
            {
                combat1 = true;
            }
            else if (compare(arg, "map"))
            {
                combat2 = true;
            }
            else
            {
                story = std::atoi(arg.c_str());
            }
        }

        // combat screen
        if (combat1)
        {
            auto map = Map::Base();

            auto map_text = map.Read("maps/test.map");

            map.Convert(map_text, party, enemies);

            map.Save("maps/test.json");

            Interface::CombatScreen(window, renderer, map, party, enemies);
        }
        else if (combat2)
        {
            auto map = Map::Base();

            auto map_text = map.Read("maps/book1/map003.map");

            map.Convert(map_text, party, enemies);

            map.Save("maps/book1/map003.json");

            map.Put(4, 0, Map::Object::Enemy, 0);
            map.Put(0, 4, Map::Object::Enemy, 1);
            map.Put(4, 8, Map::Object::Enemy, 2);
            map.Put(8, 4, Map::Object::Enemy, 3);

            Interface::CombatScreen(window, renderer, map, party, enemies);
        }
        else
        {
            // Story Screen
            Book1::InitializeStories();

            Interface::StoryScreen(window, renderer, party, {Book::Type::Book1, story});
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