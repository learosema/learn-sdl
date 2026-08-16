#pragma once

#include <memory>

#include <SDL3/SDL.h>

#include "nuklear-config.hpp"
#include <nuklear.h>
#include "nuklear_sdl3_renderer.h"

#include "demo-widget.hpp"

class NuklearApp {

public:
	NuklearApp();

	SDL_AppResult Iterate();

	SDL_AppResult Init();

	SDL_AppResult HandleEvent(SDL_Event* const event);

	inline const int GetWidth() const { return _width; }
	inline const int GetHeight() const { return _height; }

private:
	int _width;
	int _height;
	bool _resized;
	float _devicePixelRatio;

	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>     _window{nullptr, SDL_DestroyWindow};
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer{nullptr, SDL_DestroyRenderer};
	std::unique_ptr<struct nk_context, decltype(&nk_sdl_shutdown)> _ctx{nullptr, nk_sdl_shutdown};  // Nuklear context (the core piece)

    enum nk_anti_aliasing _useAntiAliasing;

	DemoWidget _demoWidget;
};
