#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>
#include <random>

#include "tiles-app.hpp"
#include "tiles.h"
#include "awesome-font.h"

namespace {
    constexpr int TILE_SIZE = 8;
    constexpr float RENDER_SCALE = 4.0f;
    constexpr double NOISE_ZOOM = 0.08;
    constexpr int FONT_GLYPHS_PER_ROW = 16;

}

SDL_AppResult TilesApp::Init() {
    SDL_SetAppMetadata("Tiles", "1.0", "lgbt.lea.learn-sdl.tiles");
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Tiles", _width, _height, SDL_WINDOW_RESIZABLE, &_window, &_renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    _tiles = IMG_LoadTexture_IO(_renderer, SDL_IOFromConstMem(TILES, TILES_len), true);
    if (!_tiles) {
        SDL_Log("Couldn't load icon: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Surface* fontSurface = IMG_Load_IO(SDL_IOFromConstMem(AWESOME_FONT, AWESOME_FONT_len), true);
    if (!fontSurface) {
        SDL_Log("Couldn't load font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetSurfaceColorKey(fontSurface, true, SDL_MapSurfaceRGB(fontSurface, 0, 0, 0));
    _font = SDL_CreateTextureFromSurface(_renderer, fontSurface);
    SDL_DestroySurface(fontSurface);
    if (!_font) {
        SDL_Log("Couldn't create font texture: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetTextureScaleMode(_font, SDL_SCALEMODE_NEAREST);

    RerollLandscape();

    return SDL_APP_CONTINUE;
}

void TilesApp::RerollLandscape()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> dist(0.0f, 10000.0f);

    _offsetX = dist(rng);
    _offsetY = dist(rng);
}

SDL_FRect TilesApp::PickTerrainTile(double noiseValue, int x, int y) const
{
    int tileX, tileY;

    if (noiseValue < -0.15) {
        tileX = 1; tileY = 4;  // water
    } else if (noiseValue < 0.2) {
        tileX = 1; tileY = 1;  // grass
    } else if (noiseValue < 0.45) {
        tileX = 4; tileY = 1;  // stone
    } else if (noiseValue < 0.65) {
        tileX = 7; tileY = 1;  // sand
    } else {
        tileX = 10; tileY = 1;  // snow
    }

    return SDL_FRect{
        static_cast<float>(tileX * TILE_SIZE),
        static_cast<float>(tileY * TILE_SIZE),
        static_cast<float>(TILE_SIZE),
        static_cast<float>(TILE_SIZE)
    };
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
    if (_font) {
        SDL_DestroyTexture(_font);
        _font = nullptr;
    }
}

void TilesApp::DrawText(std::string_view text, float x, float y, SDL_Color color) const
{
    SDL_SetTextureColorMod(_font, color.r, color.g, color.b);

    for (size_t i = 0; i < text.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        SDL_FRect src{
            static_cast<float>((c % FONT_GLYPHS_PER_ROW) * TILE_SIZE),
            static_cast<float>((c / FONT_GLYPHS_PER_ROW) * TILE_SIZE),
            static_cast<float>(TILE_SIZE),
            static_cast<float>(TILE_SIZE)
        };
        SDL_FRect dst{ x + static_cast<float>(i) * TILE_SIZE, y, static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE) };
        SDL_RenderTexture(_renderer, _font, &src, &dst);
    }
}

void TilesApp::DrawTextWithShadow(std::string_view text, float x, float y, SDL_Color color, SDL_Color shadowColor) const
{
    DrawText(text, x, y, shadowColor);
    DrawText(text, x - 1, y - 1, color);
}

SDL_AppResult TilesApp::Iterate()
{
    if (_resized) {
        SDL_SetRenderLogicalPresentation(_renderer, _width, _height, SDL_LOGICAL_PRESENTATION_STRETCH);
        _resized = false;
    }

    SDL_SetRenderScale(_renderer, RENDER_SCALE, RENDER_SCALE);

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);

    const int cols = static_cast<int>(_width / RENDER_SCALE) / TILE_SIZE + 1;
    const int rows = static_cast<int>(_height / RENDER_SCALE) / TILE_SIZE + 1;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            double n = _noise.Fbm((x + _offsetX) * NOISE_ZOOM, (y + _offsetY) * NOISE_ZOOM);

            SDL_FRect src = PickTerrainTile(n, x, y);
            SDL_FRect dst{
                static_cast<float>(x * TILE_SIZE),
                static_cast<float>(y * TILE_SIZE),
                static_cast<float>(TILE_SIZE),
                static_cast<float>(TILE_SIZE)
            };

            SDL_RenderTexture(_renderer, _tiles, &src, &dst);
        }
    }

    DrawTextWithShadow("PRESS R TO REROLL", 2, 2, SDL_Color{220, 40, 40, 255});

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

    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_R) {
        RerollLandscape();
    }

    return SDL_APP_CONTINUE;
}
