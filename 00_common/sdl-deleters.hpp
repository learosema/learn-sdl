#pragma once
#include <SDL3/SDL.h>

struct SDLWindowDeleter 
{
    void operator()(SDL_Window* p) const;
};

struct SDLRendererDeleter 
{
    void operator()(SDL_Renderer* p) const;
};

struct SDLTextureDeleter
{
    void operator()(SDL_Texture* p) const;
};

struct SDLPaletteDeleter
{
    void operator()(SDL_Palette* p) const;
};
