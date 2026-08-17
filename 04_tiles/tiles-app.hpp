#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <string_view>

#include "sdl-deleters.hpp"
#include "perlin.hpp"

class TilesApp {

public:
	SDL_AppResult Iterate();

	SDL_AppResult Init();

	SDL_AppResult HandleEvent(const SDL_Event* const event);

	inline const int GetWidth() const { return _width; }
	inline const int GetHeight() const { return _height; }

private:
	void RerollLandscape();
	SDL_FRect PickTerrainTile(double noiseValue, int x, int y) const;
	void DrawText(std::string_view text, float x, float y, SDL_Color color = {255, 255, 255, 255}) const;
	void DrawTextWithShadow(std::string_view text, float x, float y, SDL_Color color, SDL_Color shadowColor = {0, 0, 0, 255}) const;

	int _width = 640;
	int _height = 480;
	bool _resized = true;

	std::unique_ptr<SDL_Window, SDLWindowDeleter> _window;
	std::unique_ptr<SDL_Renderer, SDLRendererDeleter> _renderer;
	std::unique_ptr<SDL_Texture, SDLTextureDeleter> _tiles;
	std::unique_ptr<SDL_Texture, SDLTextureDeleter> _font;

	Perlin _noise;
	float  _offsetX = 0.0f;
	float  _offsetY = 0.0f;

};
