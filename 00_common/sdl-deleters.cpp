
#include "sdl-deleters.hpp"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_video.h"

void SDLWindowDeleter::operator()(SDL_Window* p) const 
{
    SDL_DestroyWindow(p);
}

void SDLRendererDeleter::operator()(SDL_Renderer* p) const 
{
    SDL_DestroyRenderer(p);
}

void SDLTextureDeleter::operator()(SDL_Texture* p) const 
{
    SDL_DestroyTexture(p);
}

void SDLPaletteDeleter::operator()(SDL_Palette *p) const
{
    SDL_DestroyPalette(p);
}

void SDLGLContextDeleter::operator()(SDL_GLContext p) const {
    SDL_GL_DestroyContext(p);
}