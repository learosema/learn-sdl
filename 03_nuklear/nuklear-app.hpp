#pragma once

#include "demo-widget.hpp"

typedef SDL_Window;
typedef SDL_Renderer;

class NuklearApp {

public:
	NuklearApp();
	~NuklearApp();

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

	SDL_Window* _window;
	SDL_Renderer* _renderer;
    struct nk_context *_ctx;  // Nuklear context (the core piece)

    enum nk_anti_aliasing _useAntiAliasing;

	DemoWidget _demoWidget;
};
