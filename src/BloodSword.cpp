#include "BloodSword.hpp"

#include "interface.hpp"

int main(int argc, const char **argv)
{
    SDL_Window *window = NULL;

    SDL_Renderer *renderer = NULL;

    Graphics::CreateWindow(SDL_INIT_VIDEO | SDL_INIT_AUDIO, &window, &renderer, "Blood Sword");

    Input::InitializeGamePads();

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    TTF_Init();

    if (window && renderer)
    {
        // Main Screen
        auto party = Party::Base();
        auto monsters = std::vector<Monster::Base>();

        std::vector<std::string> map = {
            "#############",
            "#           #",
            "#### ########",
            "#           #",
            "#   ######  #",
            "#   #    #  #",
            "#        #  #",
            "#############"};

        Interface::LoadMapAssets("assets.json");

        // Combat Screen
        Interface::Combat(window, renderer, map, party, monsters);

        Interface::UnloadMapAssets();

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

    // Quit SDL Subsystems

    TTF_Quit();

    IMG_Quit();

    if (SDL_WasInit(SDL_INIT_GAMECONTROLLER))
    {
        SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    }

    if (SDL_WasInit(SDL_INIT_AUDIO))
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    SDL_Quit();

    return 0;
}