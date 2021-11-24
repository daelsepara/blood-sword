#include "interface.hpp"

bool compare(std::string s1, std::string s2)
{
    //convert s1 and s2 into lower case strings
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

    return (s1.compare(s2) == 0);
}

int main(int argc, const char **argv)
{
    SDL_Window *Window = NULL;

    SDL_Renderer *Renderer = NULL;

    Graphics::CreateWindow(SDL_INIT_VIDEO | SDL_INIT_AUDIO, &Window, &Renderer, "Blood Sword");

    Input::InitializeGamePads();

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    TTF_Init();

    Fonts::Initialize();

    if (Window && Renderer)
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

        std::vector<std::string> map = {
            "#####=#######",
            "#    3     C#",
            "#### ########",
            "=4        2 =",
            "#%1%######  #",
            "#% %#%% B#  #",
            "#D       # A#",
            "#############"};

        Assets::Load();

        auto story = 1;
        auto combat = false;

        if (argc > 1)
        {
            std::string arg = argv[1];

            if (compare(arg, "combat"))
            {
                combat = true;
            }
            else
            {
                story = std::atoi(arg.c_str());
            }
        }

        // combat screen
        if (combat)
        {
            Interface::CombatScreen(Window, Renderer, map, party, enemies);
        }
        else
        {
            // Story Screen
            Book1::InitializeStories();

            Interface::StoryScreen(Window, Renderer, party, {Book::Type::Book1, story});
        }

        Assets::Unload();

        if (Renderer)
        {
            SDL_DestroyRenderer(Renderer);

            Renderer = NULL;
        }

        if (Window)
        {
            SDL_DestroyWindow(Window);

            Window = NULL;
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