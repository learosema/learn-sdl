#pragma once
#include <SDL3/SDL.h>

class TilesApp {

public:
	~TilesApp();

	SDL_AppResult Iterate();
	
	SDL_AppResult Init();

	SDL_AppResult HandleEvent(const SDL_Event* const event);

	inline const int GetWidth() const { return _width; }
	inline const int GetHeight() const { return _height; }

private:
	int _width = 640;
	int _height = 480;
	bool _resized = true;

	SDL_Window*   _window   = nullptr;
	SDL_Renderer* _renderer = nullptr;
	SDL_Texture*  _tiles    = nullptr;

};
