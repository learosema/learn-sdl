#pragma once
#include <SDL3/SDL.h>
#include <memory>

#include "sdl-deleters.hpp"

class OpenGLApp {

public:
	SDL_AppResult Iterate();

	SDL_AppResult Init();

	SDL_AppResult HandleEvent(const SDL_Event* const event);

    bool Resize();

private:

	int _texture_width = 0;
	int _texture_height = 0; 

	int _width = 640;
	int _height = 480;
	bool _resized = true;

	std::unique_ptr<SDL_Window, SDLWindowDeleter> _window;
	std::unique_ptr<SDL_GLContextState, SDLGLContextDeleter> _gl_context;
	std::unique_ptr<SDL_Texture, SDLTextureDeleter> _lfb_texture;
    std::unique_ptr<SDL_Palette, SDLPaletteDeleter> _palette;


};
