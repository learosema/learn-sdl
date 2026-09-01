#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>

#include "framebuffer-app.hpp"
#include "palette.hpp"
#include "scifi-font.h"

namespace {
    constexpr int TILE_SIZE = 8;
    constexpr float RENDER_SCALE = 4.0f;
    constexpr double NOISE_ZOOM = 0.08;
    constexpr int FONT_GLYPHS_PER_ROW = 16;

}

SDL_AppResult FramebufferApp::Init() {
    SDL_SetAppMetadata("Framebuffer", "1.0", "lgbt.lea.learn-sdl.framebuffer");
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("Framembuffer", _width, _height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    _window.reset(window);
    _renderer.reset(renderer);

    
    
    SDL_Surface* fontSurface = IMG_Load_IO(SDL_IOFromConstMem(SCIFI_FONT, SCIFI_FONT_len), true);
    if (!fontSurface) {
        SDL_Log("Couldn't load font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    _lfb_texture.reset(
        SDL_CreateTexture(
        _renderer.get(), 
        SDL_PIXELFORMAT_INDEX8, 
        SDL_TEXTUREACCESS_STREAMING, 
        _width / 2,
        _height / 2
    ));
    
    _palette.reset(SDL_CreatePalette(16));
    for (int i = 0; i < 16; i++) {
        _palette.get()->colors[0].r = EGA_PALETTE[i * 3];
        _palette.get()->colors[0].g = EGA_PALETTE[i * 3 + 1];
        _palette.get()->colors[0].b = EGA_PALETTE[i * 3 + 2];
        _palette.get()->colors[0].a = 255;
    }

    return SDL_APP_CONTINUE;
}


SDL_AppResult FramebufferApp::Iterate()
{
    if (_resized) {
        SDL_SetRenderLogicalPresentation(_renderer.get(), _width, _height, SDL_LOGICAL_PRESENTATION_STRETCH);
        _resized = false;
    }

    SDL_SetRenderScale(_renderer.get(), RENDER_SCALE, RENDER_SCALE);

    SDL_RenderClear(_renderer.get());

    const int cols = static_cast<int>(_width / RENDER_SCALE) / TILE_SIZE + 1;
    const int rows = static_cast<int>(_height / RENDER_SCALE) / TILE_SIZE + 1;

    SDL_RenderPresent(_renderer.get());

    return SDL_APP_CONTINUE;
}

SDL_AppResult FramebufferApp::HandleEvent(const SDL_Event* const event)
{
	if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; 
    }

    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        _width = event->window.data1;
        _height = event->window.data2;
        _resized = true;
    }

    return SDL_APP_CONTINUE;
}
