#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>

#include "tiles-app.hpp"
#include "tiles.h"

SDL_AppResult TilesApp::Init() {
    SDL_SetAppMetadata("Graph", "1.0", "lgbt.lea.learn-sdl.graph");
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Graph", _width, _height, SDL_WINDOW_RESIZABLE, &_window, &_renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    _tiles = IMG_LoadTexture_IO(_renderer, SDL_IOFromConstMem(TILES, TILES_len), true);
    if (!_tiles) {
        SDL_Log("Couldn't load icon: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    return SDL_APP_CONTINUE;
}

TilesApp::~TilesApp()
{
	if (_renderer) {
		SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;
	}
	if (_window) {
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}
    if (_tiles) {
        SDL_DestroyTexture(_tiles);
        _tiles = nullptr;
    }
}

SDL_AppResult TilesApp::Iterate()
{
    if (_resized) {
        SDL_SetRenderLogicalPresentation(_renderer, _width, _height, SDL_LOGICAL_PRESENTATION_STRETCH);
        _resized = false;
    }


    int w = 0, h = 0;
    SDL_FRect dst;
    const float scale = 4.0f;

    /* Center the icon and scale it up */
    SDL_GetRenderOutputSize(_renderer, &w, &h);
    SDL_SetRenderScale(_renderer, scale, scale);
    SDL_GetTextureSize(_tiles, &dst.w, &dst.h);
    dst.x = 0;
    dst.y = 0;

    /* Draw the icon */
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
    SDL_RenderTexture(_renderer, _tiles, NULL, &dst);
    SDL_RenderPresent(_renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult TilesApp::HandleEvent(const SDL_Event* const event)
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
