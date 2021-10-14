#include "BloodSword.hpp"

#include "graphics.hpp"
#include "interface.hpp"

int main(int argc, const char **argv)
{
    SDL_Window *window = NULL;

    SDL_Renderer *renderer = NULL;

    Graphics::CreateWindow(SDL_INIT_VIDEO | SDL_INIT_AUDIO, &window, &renderer, "Blood Sword");

    Input::InitializeGamePads();

    TTF_Init();

    if (window && renderer)
    {
        // Main Screen
        
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

    SDL_Quit();

    return 0;
}