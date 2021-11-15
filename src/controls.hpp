#ifndef __CONTROLS__HPP__
#define __CONTROLS__HPP__

#include "BloodSword.hpp"

#if defined(_WIN32)
#undef DELETE
#endif

namespace Control
{
    enum class Type
    {
        NONE = -1,
        ANY = 0,
        SCROLL_UP,
        SCROLL_DOWN,
        MAP_UP,
        MAP_DOWN,
        MAP_LEFT,
        MAP_RIGHT,
        BACK,
        EXIT,
        PLAYER,
        MONSTER,
        DESTINATION,
        MEMORIZE,
        FLEE,
        MOVE,
        HEAL,
        ATTACK,
        DEFEND,
        SHOOT,
        MAGIC
    };

    class Base
    {
    public:
        Control::Type Type = Control::Type::ANY;

        int ID = -1;

        int Left = -1;
        int Right = -1;
        int Up = -1;
        int Down = -1;

        int X = 0;
        int Y = 0;

        int W = 0;
        int H = 0;

        // Background color, mostly used by text buttons
        Uint32 Color;

        // Color of border hightlight if image button, Background color when highlighted otherwise (for textbutton)
        Uint32 Highlight;
    };
}

class TextButton : public Control::Base
{
private:
    void construct(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h)
    {
        ID = id;
        Text = text;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;
        W = w;
        H = h;
    }

public:
    const char *Text = NULL;

    TextButton(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h)
    {
        construct(id, text, left, right, up, down, x, y, w, h);
    }

    TextButton(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h, Control::Type type)
    {
        Type = type;

        construct(id, text, left, right, up, down, x, y, w, h);
    }
};

class Button : public Control::Base
{
private:
    SDL_Surface *createImage(const char *file)
    {
        auto surface = IMG_Load(File);

        if (surface == NULL)
        {
            std::cerr << "Unable to load image " << file << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            W = surface->w;
            H = surface->h;
        }

        return surface;
    }

    void construct(int id, const char *file, int left, int right, int up, int down, int x, int y, Uint32 color, Uint32 highlight)
    {
        ID = id;
        File = file;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;
        Color = color;
        Highlight = highlight;

        Surface = createImage(file);
    }

    void construct(int id, int left, int right, int up, int down, int x, int y, Uint32 highlight)
    {
        ID = id;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;
        Highlight = highlight;
    }

    void construct(int id, const char *file, int left, int right, int up, int down, int x, int y, Uint32 highlight)
    {
        ID = id;
        File = file;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;
        Highlight = highlight;

        Surface = createImage(file);
    }

    void construct(int id, const char *file, int left, int right, int up, int down, int x, int y, Uint32 highlight, Control::Type type)
    {
        ID = id;
        File = file;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;
        Highlight = highlight;
        Type = type;

        Surface = createImage(file);
    }

    void construct(int id, int left, int right, int up, int down, int x, int y, Uint32 color, Uint32 highlight)
    {
        ID = id;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;
        Color = color;
        Highlight = highlight;
    }

    SDL_Surface *convert(const Button &src)
    {
        return SDL_ConvertSurface(src.Surface, src.Surface->format, 0);
    }

    SDL_Surface *convert(SDL_Surface *src)
    {
        return SDL_ConvertSurface(src, src->format, 0);
    }

public:
    const char *File = NULL;

    SDL_Surface *Surface = NULL;

    Button()
    {
    }

    Button(int id, const char *file, int left, int right, int up, int down, int x, int y, Uint32 color, Uint32 highlight)
    {
        construct(id, file, left, right, up, down, x, y, color, highlight);
    }

    Button(int id, const char *file, int left, int right, int up, int down, int x, int y, Control::Type type, Uint32 color, Uint32 highlight)
    {
        Type = type;

        construct(id, file, left, right, up, down, x, y, color, highlight);
    }

    Button(int id, const char *file, int left, int right, int up, int down, int x, int y, Uint32 highlight, Control::Type type)
    {
        Type = type;

        construct(id, file, left, right, up, down, x, y, highlight);
    }

    Button(int id, SDL_Surface *image, int left, int right, int up, int down, int x, int y, const Uint32 highlight, Control::Type type)
    {
        Type = type;

        Surface = convert(image);

        if (Surface)
        {
            W = Surface->w;

            H = Surface->h;
        }

        construct(id, left, right, up, down, x, y, highlight);
    }

    // constructor for making deep copies of Button controls
    Button(const Button &src)
    {
        ID = src.ID;
        Type = src.Type;
        File = src.File;
        Left = src.Left;
        Right = src.Right;
        Up = src.Up;
        Down = src.Down;
        X = src.X;
        Y = src.Y;
        W = src.W;
        H = src.H;
        Color = src.Color;
        Highlight = src.Highlight;

        if (src.Surface)
        {
            if (Surface)
            {
                SDL_FreeSurface(Surface);

                Surface = NULL;
            }

            Surface = convert(src);
        }
    }

    // operator overload for making deep copies of Button controls
    Button &operator=(const Button &src)
    {
        // self-assignment protection
        if (this != &src)
        {
            ID = src.ID;
            Type = src.Type;
            File = src.File;
            Left = src.Left;
            Right = src.Right;
            Up = src.Up;
            Down = src.Down;
            X = src.X;
            Y = src.Y;
            W = src.W;
            H = src.H;
            Color = src.Color;
            Highlight = src.Highlight;

            if (src.Surface)
            {
                if (Surface)
                {
                    SDL_FreeSurface(Surface);

                    Surface = NULL;
                }

                Surface = convert(src);
            }
        }

        return *this;
    }

    // free allocated surface
    ~Button()
    {
        if (Surface)
        {
            SDL_FreeSurface(Surface);

            Surface = NULL;
        }
    }
};
#endif
