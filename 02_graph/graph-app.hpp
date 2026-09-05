#pragma once
#include <SDL3/SDL.h>
#include <memory>

class GraphApp {

public:
	SDL_AppResult Iterate();

	SDL_AppResult Init();

	SDL_AppResult HandleEvent(const SDL_Event* const event);
	
private:
	int _width = 640;
	int _height = 480;
	bool _resized = true;
	
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>     _window{nullptr, SDL_DestroyWindow};
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer{nullptr, SDL_DestroyRenderer};

};
