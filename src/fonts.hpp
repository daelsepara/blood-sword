#ifndef __FONTS__HPP__
#define __FONTS__HPP__

#include <SDL_ttf.h>

#include "globals.hpp"

namespace Fonts
{
    TTF_Font *Caption = NULL;
    TTF_Font *Normal = NULL;

    void Initialize()
    {
        Caption = TTF_OpenFont(FONT_BOOKMAN, 22);
        Normal = TTF_OpenFont(FONT_BOOKMAN, 32);
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
    }
}
#endif
