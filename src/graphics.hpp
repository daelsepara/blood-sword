#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include "BloodSword.hpp"

namespace Graphics
{
    SDL_Surface *CreateHeaderButton(SDL_Window *window, const char *font, int font_size, const char *text, SDL_Color color, Uint32 bg, int w, int h, int x);
    SDL_Surface *CreateImage(const char *image);
    SDL_Surface *CreateImage(const char *image, int w, Uint32 bg);
    SDL_Surface *CreateText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style);
    SDL_Surface *CreateTextAndImage(const char *text, const char *image, const char *ttf, int font_size, SDL_Color textColor, Uint32 bg, int wrap, int style, bool bottom);

    std::vector<TextButton> CreateFixedTextButtons(const char **choices, int num, int text_buttonw, int text_buttonh, int textbutton_space, int text_x, int text_buttony);
    std::vector<TextButton> CreateTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_buttony, int window_width, int window_height);

    void CreateWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title);
    void DrawRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color);
    void FillRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color);
    void FillWindow(SDL_Renderer *renderer, Uint32 color);
    void PutText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y);
    void PutTextBox(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y);
    void RenderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int space, int pts);
    void RenderCaption(SDL_Renderer *renderer, Button &control, SDL_Color color, Uint32 bg);
    void RenderImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y);
    void RenderImage(SDL_Renderer *renderer, SDL_Surface *text, int x, int y, int bounds, int offset);
    void RenderText(SDL_Renderer *renderer, SDL_Surface *text, Uint32 bg, int x, int y, int bounds, int offset);
    void RenderText(SDL_Renderer *renderer, SDL_Surface *text, Uint32 bg, int x, int y);
    void RenderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int current, int fontsize, int style);
    void SetWindowIcon(SDL_Window *window, const char *icon);
    void StretchImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h);
    void ThickRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color, int pts);

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

    void DrawRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color)
    {
        SDL_Rect rect;

        rect.w = w;
        rect.h = h;
        rect.x = x;
        rect.y = y;

        SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
        SDL_RenderDrawRect(renderer, &rect);
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

    void FillWindow(SDL_Renderer *renderer, Uint32 color)
    {
        SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));

        SDL_RenderClear(renderer);
    }

    void PutText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y)
    {
        if (renderer)
        {
            TTF_SetFontStyle(font, style);

            auto surface = TTF_RenderText_Blended_Wrapped(font, text, fg, w - 2 * space);

            if (surface)
            {
                auto height = (surface->h + 2 * space) < h ? h : (surface->h + 2 * space);

                if (x < 0)
                {
                    Graphics::FillRect(renderer, w, height, (SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - height) / 2, bg);
                }
                else
                {
                    Graphics::FillRect(renderer, w, height, x, y, bg);
                }

                if (space > 0)
                {
                    if (x < 0)
                    {
                        Graphics::RenderText(renderer, surface, 0, (SCREEN_WIDTH - surface->w) / 2 + space, (SCREEN_HEIGHT - height) / 2 + space, height - 2 * space, 0);
                    }
                    else
                    {
                        Graphics::RenderText(renderer, surface, 0, x + space, y + space, height - 2 * space, 0);
                    }
                }
                else
                {
                    if (x < 0)
                    {
                        Graphics::RenderText(renderer, surface, 0, (SCREEN_WIDTH - surface->w) / 2 + (w - surface->w), (SCREEN_HEIGHT - surface->h) / 2 + (h - surface->h) / 2, height, 0);
                    }
                    else
                    {
                        Graphics::RenderText(renderer, surface, 0, x, y, height, 0);
                    }
                }

                SDL_FreeSurface(surface);

                surface = NULL;
            }
        }
    }

    void PutText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int x, int y)
    {
        if (renderer)
        {
            TTF_SetFontStyle(font, style);

            auto surface = TTF_RenderText_Blended(font, text, fg);

            if (surface)
            {
                if (x < 0)
                {
                    Graphics::FillRect(renderer, surface->w, surface->h, (SCREEN_WIDTH - surface->w) / 2, (SCREEN_HEIGHT - surface->h) / 2, bg);
                }
                else
                {
                    Graphics::FillRect(renderer, surface->w, surface->h, x, y, bg);
                }

                if (space > 0)
                {
                    if (x < 0)
                    {
                        Graphics::RenderText(renderer, surface, 0, (SCREEN_WIDTH - surface->w) / 2 + space, (SCREEN_HEIGHT - surface->h) / 2 + space, surface->h - 2 * space, 0);
                    }
                    else
                    {
                        Graphics::RenderText(renderer, surface, 0, x + space, y + space);
                    }
                }
                else
                {
                    if (x < 0)
                    {
                        Graphics::RenderText(renderer, surface, 0, (SCREEN_WIDTH - surface->w) / 2, (SCREEN_HEIGHT - surface->h) / 2);
                    }
                    else
                    {
                        Graphics::RenderText(renderer, surface, 0, x, y);
                    }
                }

                SDL_FreeSurface(surface);

                surface = NULL;
            }
        }
    }

    void PutTextBox(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y)
    {
        if (renderer)
        {
            TTF_SetFontStyle(font, style);

            auto surface = TTF_RenderText_Blended_Wrapped(font, text, fg, w - 2 * space);

            if (surface)
            {
                auto height = (surface->h + 2 * space) < h ? h : (surface->h + 2 * space);

                if (x < 0)
                {
                    Graphics::FillRect(renderer, w, height, (SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - height) / 2, bg);
                }
                else
                {
                    Graphics::FillRect(renderer, w, height, x, y, bg);
                }

                if (space > 0)
                {
                    if (x < 0)
                    {
                        Graphics::RenderText(renderer, surface, 0, (SCREEN_WIDTH - surface->w) / 2 + space, y + (SCREEN_HEIGHT - surface->h) / 2, height - 2 * space, 0);
                    }
                    else
                    {
                        Graphics::RenderText(renderer, surface, 0, x + space, y + (h - surface->h) / 2, height - 2 * space, 0);
                    }
                }
                else
                {
                    if (x < 0)
                    {
                        Graphics::RenderText(renderer, surface, 0, (SCREEN_WIDTH - surface->w) / 2, y + (SCREEN_HEIGHT - surface->h) / 2, height - 2 * space, 0);
                    }
                    else
                    {
                        Graphics::RenderText(renderer, surface, 0, x + (w - surface->w) / 2, y + (h - surface->h) / 2, height - 2 * space, 0);
                    }
                }

                SDL_FreeSurface(surface);

                surface = NULL;
            }
        }
    }

    void RenderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int space, int pts)
    {
        if (controls.size() > 0)
        {
            auto highlight = -1;

            for (auto i = 0; i < controls.size(); i++)
            {
                Graphics::RenderImage(renderer, controls[i].Surface, controls[i].X, controls[i].Y);

                if (i == current)
                {
                    highlight = i;
                }
            }

            if (highlight >= 0 && highlight < controls.size())
            {
                auto fg = controls[highlight].Highlight;

                auto map_object = (controls[highlight].Type == Control::Type::PLAYER || controls[highlight].Type == Control::Type::ENEMY || controls[highlight].Type == Control::Type::MAP_NONE || controls[highlight].Type == Control::Type::DESTINATION || controls[highlight].Type == Control::Type::MAP_EXIT);

                if (map_object)
                {
                    Graphics::ThickRect(renderer, controls[highlight].W - 4 * pts, controls[highlight].W - 4 * pts, controls[highlight].X + 2 * pts, controls[highlight].Y + 2 * pts, fg, pts);
                }
                else
                {
                    Graphics::ThickRect(renderer, controls[highlight].W, controls[highlight].H, controls[highlight].X, controls[highlight].Y, fg, pts);
                }
            }
        }
    }

    void RenderCaption(SDL_Renderer *renderer, Button &control, SDL_Color color, Uint32 bg)
    {
        auto captionx = control.X - text_space;

        auto captiony = control.Y + control.H + text_space;

        std::string caption = "";

        if (control.Type == Control::Type::ITEMS)
        {
            caption = "Items";
        }
        else if (control.Type == Control::Type::ABILITY)
        {
            caption = "Use Ability";
        }
        else if (control.Type == Control::Type::MOVE)
        {
            caption = "Move";
        }
        else if (control.Type == Control::Type::HEAL)
        {
            caption = "Healing";
        }
        else if (control.Type == Control::Type::ATTACK)
        {
            caption = "Fight";
        }
        else if (control.Type == Control::Type::DEFEND)
        {
            caption = "Defend";
        }
        else if (control.Type == Control::Type::SHOOT)
        {
            caption = "Shoot";
        }
        else if (control.Type == Control::Type::CALL)
        {
            caption = "Call a spell to mind";
        }
        else if (control.Type == Control::Type::CAST)
        {
            caption = "Cast spell in mind";
        }
        else if (control.Type == Control::Type::FLEE)
        {
            caption = "Flee";
        }
        else if (control.Type == Control::Type::BACK)
        {
            caption = "Go back";
        }
        else if (control.Type == Control::Type::QUICKTHINKING)
        {
            caption = "Use quick thinking";
        }
        else if (control.Type == Control::Type::AMBIDEXTROUSNESS)
        {
            caption = "Ambidextrousness";
        }
        else if (control.Type == Control::Type::UNARMED_MARTIAL_ARTS)
        {
            caption = "Unarmed Martial Arts";
        }
        else if (control.Type == Control::Type::ARCHERY)
        {
            caption = "Archery";
        }
        else if (control.Type == Control::Type::DODGING)
        {
            caption = "Dodging";
        }
        else if (control.Type == Control::Type::QUARTERSTAFF)
        {
            caption = "Quarterstaff";
        }
        else if (control.Type == Control::Type::EXORCISM)
        {
            caption = "Exorcism";
        }
        else if (control.Type == Control::Type::ESP)
        {
            caption = "ESP";
        }
        else if (control.Type == Control::Type::PARANORMAL_SIGHT)
        {
            caption = "Paranormal Sight";
        }
        else if (control.Type == Control::Type::LEVITATION)
        {
            caption = "Levitation";
        }
        else if (control.Type == Control::Type::EXIT)
        {
            caption = "Exit";
        }
        else if (control.Type == Control::Type::ENCYCLOPEDIA)
        {
            caption = "See rules for combat and magic";
        }
        else if (control.Type == Control::Type::MAP)
        {
            caption = "View map of Legend";
        }
        else if (control.Type == Control::Type::GAME)
        {
            caption = "Load / Save game";
        }
        else if (control.Type == Control::Type::PARTY)
        {
            caption = "Mange party";
        }
        else if (control.Type == Control::Type::CONTINUE)
        {
            caption = "Continue story";
        }

        if (caption.length() > 0)
        {
            auto captionw = 0;

            auto captionh = 0;

            TTF_SizeText(Fonts::Caption, caption.c_str(), &captionw, &captionh);

            Graphics::PutText(renderer, caption.c_str(), Fonts::Caption, border_pts, color, bg, TTF_STYLE_NORMAL, captionw + 2 * text_space, captionh, captionx, captiony);
        }
    }

    void RenderImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y)
    {
        if (image && renderer)
        {
            SDL_Rect position;

            position.w = image->w;
            position.h = image->h;
            position.x = x;
            position.y = y;

            auto texture = SDL_CreateTextureFromSurface(renderer, image);

            if (texture)
            {
                SDL_Rect src;

                src.w = image->w;
                src.h = image->h;
                src.x = 0;
                src.y = 0;

                SDL_RenderCopy(renderer, texture, &src, &position);

                SDL_DestroyTexture(texture);

                texture = NULL;
            }
        }
    }

    // Render a portion of the image on bounded surface within the specified window
    void RenderImage(SDL_Renderer *renderer, SDL_Surface *text, int x, int y, int bounds, int offset)
    {
        if (renderer)
        {
            if (text && renderer)
            {
                SDL_Rect dst;
                SDL_Rect src;

                src.w = text->w;
                src.h = text->h < bounds ? text->h : bounds;
                src.y = offset;
                src.x = 0;

                dst.w = text->w;
                dst.h = text->h < bounds ? text->h : bounds;
                dst.x = x;
                dst.y = y;

                auto texture = SDL_CreateTextureFromSurface(renderer, text);

                if (texture)
                {
                    SDL_RenderCopy(renderer, texture, &src, &dst);

                    SDL_DestroyTexture(texture);

                    texture = NULL;
                }
            }
        }
    }

    void StretchImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h)
    {
        if (renderer && image)
        {
            SDL_Rect position;

            position.w = w;
            position.h = h;
            position.x = x;
            position.y = y;

            auto texture = SDL_CreateTextureFromSurface(renderer, image);

            if (texture)
            {
                SDL_Rect src;

                src.w = image->w;
                src.h = image->h;
                src.x = 0;
                src.y = 0;

                SDL_RenderCopy(renderer, texture, &src, &position);

                SDL_DestroyTexture(texture);

                texture = NULL;
            }
        }
    }

    // Render a portion of the text (image) on bounded surface within the specified window
    void RenderText(SDL_Renderer *renderer, SDL_Surface *text, Uint32 bg, int x, int y, int bounds, int offset)
    {
        if (renderer)
        {
            if (text && renderer)
            {
                SDL_Rect dst;
                SDL_Rect src;

                src.w = text->w;
                src.h = text->h < bounds ? text->h : bounds;
                src.y = offset;
                src.x = 0;

                dst.w = text->w;
                dst.h = text->h < bounds ? text->h : bounds;
                dst.x = x;
                dst.y = y;

                if (bg != 0)
                {
                    SDL_SetRenderDrawColor(renderer, R(bg), G(bg), B(bg), A(bg));
                    SDL_RenderFillRect(renderer, &dst);
                }

                auto texture = SDL_CreateTextureFromSurface(renderer, text);

                if (texture)
                {
                    SDL_RenderCopy(renderer, texture, &src, &dst);

                    SDL_DestroyTexture(texture);

                    texture = NULL;
                }
            }
        }
    }

    // Render text
    void RenderText(SDL_Renderer *renderer, SDL_Surface *text, Uint32 bg, int x, int y)
    {
        if (renderer)
        {
            if (text && renderer)
            {
                SDL_Rect dst;
                SDL_Rect src;

                src.w = text->w;
                src.h = text->h;
                src.y = 0;
                src.x = 0;

                dst.w = text->w;
                dst.h = text->h;
                dst.x = x;
                dst.y = y;

                if (bg != 0)
                {
                    SDL_SetRenderDrawColor(renderer, R(bg), G(bg), B(bg), A(bg));
                    SDL_RenderFillRect(renderer, &dst);
                }

                auto texture = SDL_CreateTextureFromSurface(renderer, text);

                if (texture)
                {
                    SDL_RenderCopy(renderer, texture, &src, &dst);

                    SDL_DestroyTexture(texture);

                    texture = NULL;
                }
            }
        }
    }

    void RenderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int current, int fontsize, int style)
    {
        if (controls.size() > 0)
        {
            for (auto i = 0; i < controls.size(); i++)
            {
                auto text = Graphics::CreateText(controls[i].Text, ttf, fontsize, controls[i].Fg, controls[i].W, style);

                auto x = controls[i].X + (controls[i].W - text->w) / 2;
                auto y = controls[i].Y + (controls[i].H - text->h) / 2;

                SDL_Rect rect;

                rect.w = controls[i].W;
                rect.h = controls[i].H;
                rect.x = controls[i].X;
                rect.y = controls[i].Y;

                if (i == current)
                {
                    SDL_SetRenderDrawColor(renderer, R(controls[i].Highlight), G(controls[i].Highlight), B(controls[i].Highlight), A(controls[i].Highlight));
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, R(controls[i].Color), G(controls[i].Color), B(controls[i].Color), A(controls[i].Color));
                }

                SDL_RenderFillRect(renderer, &rect);

                if (i == current)
                {
                    Graphics::RenderText(renderer, text, controls[i].Highlight, x, y, 2 * fontsize, 0);
                }
                else
                {
                    Graphics::RenderText(renderer, text, controls[i].Color, x, y, 2 * fontsize, 0);

                    Graphics::DrawRect(renderer, controls[i].W, controls[i].H, controls[i].X, controls[i].Y, controls[i].Highlight);
                }

                SDL_FreeSurface(text);

                text = NULL;
            }
        }
    }

    void SetWindowIcon(SDL_Window *window, const char *icon)
    {
        auto surface = Graphics::CreateImage(icon);

        if (surface)
        {
            SDL_SetWindowIcon(window, surface);

            SDL_FreeSurface(surface);

            surface = NULL;
        }
    }

    void ThickRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color, int pts)
    {
        SDL_Rect rect;

        auto space = 2 * pts;

        for (auto size = pts; size >= 0; size--)
        {
            rect.w = w + 2 * (space - size);
            rect.h = h + 2 * (space - size);
            rect.x = x - space + size;
            rect.y = y - space + size;

            SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));

            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    SDL_Surface *CreateHeaderButton(SDL_Window *window, const char *font, int font_size, const char *text, SDL_Color color, Uint32 bg, int w, int h, int x)
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        auto button = SDL_CreateRGBSurface(0, w, h, 32, bg == 0 ? 0xFF000000 : 0, bg == 0 ? 0x00FF0000 : 0, bg == 0 ? 0x0000FF00 : 0, bg == 0 ? 0x000000FF : 0);
#else
        auto button = SDL_CreateRGBSurface(0, w, h, 32, bg == 0 ? 0x000000FF : 0, bg == 0 ? 0x0000FF00 : 0, bg == 0 ? 0x00FF0000 : 0, bg == 0 ? 0xFF000000 : 0);
#endif

        auto text_surface = Graphics::CreateText(text, font, font_size, color, w, TTF_STYLE_NORMAL);

        SDL_Surface *converted_text = NULL;

        if (button && text_surface)
        {
            SDL_Rect dst;

            dst.w = button->w;
            dst.h = button->h;
            dst.x = 0;
            dst.y = 0;

            dst.x = x < 0 ? (button->w - text_surface->w) / 2 : x;
            dst.y = (button->h - text_surface->h) / 2;

            if (bg != 0)
            {
                SDL_FillRect(button, NULL, bg);

                SDL_BlitSurface(text_surface, NULL, button, &dst);
            }
            else
            {
                SDL_FillRect(button, NULL, SDL_MapRGBA(button->format, 0, 0, 0, 0));

                converted_text = SDL_ConvertSurface(text_surface, button->format, 0);

                SDL_SetSurfaceAlphaMod(converted_text, SDL_ALPHA_OPAQUE);

                SDL_BlitSurface(converted_text, NULL, button, &dst);
            }
        }

        if (text_surface)
        {
            SDL_FreeSurface(text_surface);

            text_surface = NULL;
        }

        if (converted_text)
        {
            SDL_FreeSurface(converted_text);

            converted_text = NULL;
        }

        return button;
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
    SDL_Surface *CreateTextAndImage(const char *text, const char *image, const char *ttf, int font_size, SDL_Color textColor, Uint32 bg, int wrap, int style, bool bottom)
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
            text_dst.y = bottom ? image_h + text_space : 0;

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

            dst.y = bottom ? 0 : text_surface->h + text_space;

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

    std::vector<TextButton> CreateFixedTextButtons(const char **choices, int num, int text_buttonw, int text_buttonh, int textbutton_space, int text_x, int text_buttony)
    {
        auto controls = std::vector<TextButton>();

        if (num > 0)
        {
            for (auto i = 0; i < num; i++)
            {
                auto left = i > 0 ? i - 1 : i;
                auto right = i < num - 1 ? i + 1 : i;
                auto up = i;
                auto down = i;

                auto x = text_x + i * (text_buttonw + textbutton_space);

                auto button = TextButton(i, choices[i], left, right, up, down, x, text_buttony, text_buttonw, text_buttonh);

                controls.push_back(button);
            }
        }

        return controls;
    }

    std::vector<TextButton> CreateTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_buttony, int window_width, int window_height)
    {
        auto controls = std::vector<TextButton>();

        if (num > 0)
        {
            auto margin2 = (2.0 * Margin);
            auto marginleft = (1.0 - margin2);

            auto pixels = (int)(window_width * Margin) / 2;
            auto width = (int)(window_height * marginleft);

            auto text_spacew = width / num;
            auto htext_buttonw = text_spacew - pixels;
            auto text_space = pixels / 2;

            for (auto i = 0; i < num; i++)
            {
                auto left = i > 0 ? i - 1 : i;
                auto right = i < num - 1 ? i + 1 : i;
                auto up = i;
                auto down = i;

                auto x = text_x + i * (htext_buttonw + text_space * 2) + text_space;

                auto button = TextButton(i, choices[i], left, right, up, down, x, text_buttony, htext_buttonw, text_buttonh);

                controls.push_back(button);
            }
        }

        return controls;
    }

}
#endif