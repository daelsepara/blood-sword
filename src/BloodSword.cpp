#include "BloodSword.hpp"

#include "interface.hpp"

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
        // Setup party, monsters, and map
        auto party = Party::Base();

        party.Members.push_back(Character::Create(Character::Class::Warrior, 8));

        auto monsters = std::vector<Monster::Base>();

        monsters.push_back(Monster::Base(Monster::Type::Barbarian, "BARBARIAN", 8, 5, 7, 12, 1, 2, 1));

        std::vector<std::string> map = {
            "#############",
            "#           #",
            "#### ########",
            "#   1  A    #",
            "#% %######  #",
            "#% %#%%  #  #",
            "#        #  #",
            "#############"};

        Assets::Load("assets.json");

        // Combat Screen
        Interface::CombatScreen(Window, Renderer, map, party, monsters);

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