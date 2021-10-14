#include "BloodSword.hpp"

void createWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title)
{
    // The window and renderer we'll be rendering to
    *window = NULL;
    *renderer = NULL;

    if (SDL_Init(flags) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        SDL_DisplayMode mode;

        SDL_GetCurrentDisplayMode(0, &mode);

        SCREEN_WIDTH = (mode.w);

        SCREEN_HEIGHT = (mode.h);

        Recompute();

        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), window, renderer);

        if (renderer)
        {
            SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_NONE);
        }

        if (!window || !renderer)
        {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else if (window)
        {
            SDL_SetWindowTitle(*window, title);
        }
    }
}

int main(int argc, const char **argv)
{
    SDL_Window *window = NULL;

    SDL_Renderer *renderer = NULL;

    createWindow(SDL_INIT_VIDEO | SDL_INIT_AUDIO, &window, &renderer, "Blood Sword");

    Input::InitializeGamePads();

    TTF_Init();

    if (window && renderer)
    {
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