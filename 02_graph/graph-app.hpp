#pragma once

enum SDL_AppResult;
union SDL_Event;
class SDL_Window;
class SDL_Renderer;

class GraphApp {

public:
	~GraphApp();

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

};
