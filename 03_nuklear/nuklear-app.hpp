#pragma once
#include <SDL3/SDL.h>
#include <nuklear.h>

class NuklearApp {

public:
	~NuklearApp();

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
    struct nk_context *ctx;  // Nuklear context (the core piece)
    struct nk_colorf _bg = {.r=0, .g=0, .b=0.0f, .a=1.0f};     // Background color (RGBA float)
    enum nk_anti_aliasing _AA = NK_ANTI_ALIASING_ON;
};
