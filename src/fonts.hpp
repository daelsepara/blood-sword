#ifndef __FONTS_HPP__
#define __FONTS_HPP__

#include <SDL_ttf.h>

#include "globals.hpp"

namespace Fonts
{
    TTF_Font *Caption = NULL;
    TTF_Font *Normal = NULL;
    TTF_Font *Fixed = NULL;

    void Initialize()
    {
        Caption = TTF_OpenFont(FONT_BOOKMAN, 22);
        Normal = TTF_OpenFont(FONT_BOOKMAN, 24);
        Fixed = TTF_OpenFont(FONT_BOOKMAN, 24);

        TTF_SetFontKerning(Fixed, 0);
    }

    void Free()
    {
        if (Caption)
        {
            TTF_CloseFont(Caption);

            Caption = NULL;
        }

        if (Normal)
        {
            TTF_CloseFont(Normal);

            Normal = NULL;
        }

        if (Fixed)
        {
            TTF_CloseFont(Fixed);

            Fixed = NULL;
        }
    }
}
#endif
