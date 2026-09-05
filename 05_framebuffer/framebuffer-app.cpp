#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>

#include "framebuffer-app.hpp"
#include "palette.hpp"
// #include "scifi-font.h"

namespace {
    constexpr float RENDER_SCALE = 8.0f;
}

SDL_AppResult FramebufferApp::Init() {
    SDL_SetAppMetadata("Framebuffer", "1.0", "lgbt.lea.learn-sdl.framebuffer");
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("Framebuffer", _width, _height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    _window.reset(window);
    _renderer.reset(renderer);
    
    _palette.reset(SDL_CreatePalette(16));
    for (int i = 0; i < 16; i++) {
        _palette.get()->colors[i].r = EGA_PALETTE[i * 3];
        _palette.get()->colors[i].g = EGA_PALETTE[i * 3 + 1];
        _palette.get()->colors[i].b = EGA_PALETTE[i * 3 + 2];
        _palette.get()->colors[i].a = 255;
    }

    if (!ResizeFramebuffer()) {
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}


SDL_AppResult FramebufferApp::Iterate()
{
    if (_resized) {
        ResizeFramebuffer();
    }


    SDL_RenderClear(_renderer.get());

    uint8_t *pixels;
    int pitch;
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */

    SDL_LockTexture(_lfb_texture.get(), nullptr, (void **)&pixels, &pitch);
    size_t offs = 0;
    for (size_t y = 0; y < _texture_height; offs+=pitch, y++) {
        for (size_t x = 0; x < _texture_width; x++) {
            uint8_t o = round((.5 + .5 * sin(x*.1f) * cos(y * .1f)* sin(now))* 15);
            if (o == 7) o++;
            pixels[offs + x] = o; 
        }
    }
    SDL_UnlockTexture(_lfb_texture.get());

    SDL_RenderTexture(_renderer.get(), _lfb_texture.get(), nullptr, nullptr);
    SDL_RenderPresent(_renderer.get());

    return SDL_APP_CONTINUE;
}

bool FramebufferApp::ResizeFramebuffer() {
    
    SDL_SetRenderLogicalPresentation(_renderer.get(), _width, _height, SDL_LOGICAL_PRESENTATION_STRETCH);
    int new_tex_width = _width / RENDER_SCALE;
    int new_tex_height = _height / RENDER_SCALE;
    if ((_texture_width == new_tex_width) && (_texture_height == new_tex_height)) {
        _resized = false;
        return false;
    }

    
    SDL_Texture *new_texture = SDL_CreateTexture(
        _renderer.get(), 
        SDL_PIXELFORMAT_INDEX8, 
        SDL_TEXTUREACCESS_STREAMING, 
        new_tex_width,
        new_tex_height
    );

    if (! new_texture) {
        SDL_Log("Resize Framebuffer failed: %s", SDL_GetError());
        return false;
    }

    if (!SDL_SetTexturePalette(new_texture, _palette.get())) {
        SDL_Log("Couldn't set palette: %s", SDL_GetError());
    }
    SDL_SetTextureScaleMode(new_texture, SDL_SCALEMODE_NEAREST);
    
    if (_lfb_texture.get() != nullptr) {

        SDL_FRect destRect = {
            .0f, 0.0f, 
            static_cast<float>(_texture_width), 
            static_cast<float>(_texture_height)
        };

        SDL_SetRenderTarget(_renderer.get(), new_texture);
        SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
        SDL_RenderClear(_renderer.get());
        SDL_RenderTexture(_renderer.get(), _lfb_texture.get(), NULL, &destRect);
        SDL_SetRenderTarget(_renderer.get(), nullptr);
        
    }

    _lfb_texture.reset(new_texture); // also destroys the old texture :)
    _texture_width = new_tex_width;
    _texture_height = new_tex_height; 

    _resized = false;
    return true;
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
