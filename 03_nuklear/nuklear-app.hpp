#pragma once

#include <memory>

#include <SDL3/SDL.h>

#include "nuklear-config.hpp"
#include <nuklear.h>
#include "nuklear_sdl3_renderer.h"

#include "demo-widget.hpp"

class NuklearApp {

public:
	NuklearApp() = default;

	SDL_AppResult Iterate();

	SDL_AppResult Init();

	SDL_AppResult HandleEvent(SDL_Event* const event);

private:
	int _width = 640;
	int _height = 480;
	bool _resized = true;
	float _devicePixelRatio = 1.0f;

    enum nk_anti_aliasing _useAntiAliasing = NK_ANTI_ALIASING_ON;
	DemoWidget _demoWidget;

	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>     _window{nullptr, SDL_DestroyWindow};
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer{nullptr, SDL_DestroyRenderer};
	std::unique_ptr<struct nk_context, decltype(&nk_sdl_shutdown)> _ctx{nullptr, nk_sdl_shutdown};  // Nuklear context (the core piece)
};
