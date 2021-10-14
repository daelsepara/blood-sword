#ifndef __GRAPHICS__HPP__
#define __GRAPHICS__HPP__

#include "BloodSword.hpp"

namespace Graphics
{
    // Forward Declarations
    void CreateWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title);
    void FillRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color);

    SDL_Surface *CreateImage(const char *image);
    SDL_Surface *CreateImage(const char *image, int w, Uint32 bg);
    SDL_Surface *CreateText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style);
    SDL_Surface *CreateTextAndImage(const char *text, const char *image, const char *ttf, int font_size, SDL_Color textColor, Uint32 bg, int wrap, int style);

    // ---------------------------------------------------------------------------------------------
    // START - Implementations
    // ---------------------------------------------------------------------------------------------
    void CreateWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title)
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

    void FillRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color)
    {
        SDL_Rect rect;

        rect.w = w;
        rect.h = h;
        rect.x = x;
        rect.y = y;

        SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_Surface *CreateImage(const char *image)
    {
        auto surface = IMG_Load(image);

        if (!surface)
        {
            std::cerr << "Unable to load image " << image << "! SDL Error: " << SDL_GetError() << std::endl;
        }

        return surface;
    }

    SDL_Surface *CreateImage(const char *image, int wrap, Uint32 bg)
    {
        SDL_Surface *surface = NULL;
        SDL_Surface *converted_surface = NULL;

        auto image_surface = Graphics::CreateImage(image);

        if (image_surface)
        {
            auto image_h = image_surface->h;

            if (image_surface->w > wrap)
            {
                auto image_scale = (double)((double)wrap / image_surface->w);

                image_h = (int)(image_surface->h * image_scale);
            }

            Uint32 amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            amask = 0x000000ff;
#else
            amask = 0xff000000;
#endif

            surface = SDL_CreateRGBSurface(0, wrap, image_h, 32, 0, 0, 0, amask);

            SDL_Rect dst;

            dst.w = surface->w;
            dst.h = surface->h;
            dst.x = 0;
            dst.y = 0;

            SDL_FillRect(surface, NULL, bg);

            dst.h = image_h;

            if (image_surface->w > wrap)
            {
                dst.w = surface->w;
            }
            else
            {
                dst.w = image_surface->w;
                dst.h = image_surface->h;
                dst.x = (surface->w - image_surface->w) / 2;
            }

            converted_surface = SDL_ConvertSurface(image_surface, surface->format, 0);

            SDL_BlitScaled(converted_surface, NULL, surface, &dst);
        }

        if (image_surface)
        {
            SDL_FreeSurface(image_surface);

            image_surface = NULL;
        }

        if (converted_surface)
        {
            SDL_FreeSurface(converted_surface);

            converted_surface = NULL;
        }

        return surface;
    }

    // create text image with line wrap limit
    SDL_Surface *CreateText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style)
    {
        SDL_Surface *surface = NULL;

        auto font = TTF_OpenFont(ttf, font_size);

        if (font)
        {
            TTF_SetFontStyle(font, style);

            surface = TTF_RenderText_Blended_Wrapped(font, text, textColor, wrap);

            TTF_CloseFont(font);

            font = NULL;
        }

        return surface;
    }

    // create text and image with line wrap limit
    SDL_Surface *CreateTextAndImage(const char *text, const char *image, const char *ttf, int font_size, SDL_Color textColor, Uint32 bg, int wrap, int style)
    {
        SDL_Surface *surface = NULL;

        SDL_Surface *converted_surface = NULL;

        SDL_Surface *converted_text = NULL;

        auto image_surface = Graphics::CreateImage(image);

        auto text_surface = Glyphs::FormattedText(text, ttf, font_size, textColor, wrap);

        if (image_surface && text_surface)
        {
            auto image_h = image_surface->h;

            if (image_surface->w > wrap)
            {
                auto image_scale = (double)((double)wrap / image_surface->w);

                image_h = (int)(image_surface->h * image_scale);
            }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            surface = SDL_CreateRGBSurface(0, wrap, image_h + text_space + text_surface->h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
#else
            surface = SDL_CreateRGBSurface(0, wrap, image_h + text_space + text_surface->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
#endif

            SDL_Rect dst;

            dst.w = surface->w;
            dst.h = surface->h;
            dst.x = 0;
            dst.y = 0;

            SDL_Rect text_dst;

            text_dst.w = surface->w;
            text_dst.h = surface->h;
            text_dst.x = 0;
            text_dst.y = image_h + text_space;

            SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

            dst.h = image_h;

            if (image_surface->w > wrap)
            {
                dst.w = surface->w;
            }
            else
            {
                dst.w = image_surface->w;
                dst.h = image_surface->h;
                dst.x = (surface->w - image_surface->w) / 2;
            }

            converted_surface = SDL_ConvertSurface(image_surface, surface->format, 0);

            converted_text = SDL_ConvertSurface(text_surface, surface->format, 0);

            SDL_SetSurfaceAlphaMod(converted_surface, SDL_ALPHA_OPAQUE);

            SDL_SetSurfaceAlphaMod(converted_text, SDL_ALPHA_OPAQUE);

            SDL_BlitScaled(converted_surface, NULL, surface, &dst);

            SDL_BlitSurface(converted_text, NULL, surface, &text_dst);
        }

        if (image_surface)
        {
            SDL_FreeSurface(image_surface);

            image_surface = NULL;
        }

        if (text_surface)
        {
            SDL_FreeSurface(text_surface);

            text_surface = NULL;
        }

        if (converted_surface)
        {
            SDL_FreeSurface(converted_surface);

            converted_surface = NULL;
        }

        if (converted_text)
        {
            SDL_FreeSurface(converted_text);

            converted_text = NULL;
        }

        return surface;
    }
}
#endif