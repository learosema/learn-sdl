#include <SDL3/SDL.h>

#include "nuklear-config.hpp"

#define NK_IMPLEMENTATION
#include <nuklear.h>

#define NK_SDL3_RENDERER_IMPLEMENTATION
#include "nuklear_sdl3_renderer.h"

#include "nuklear-app.hpp"

static char* nk_sdl_dtoa(char *str, double d)
{
    NK_ASSERT(str);
    if (!str) return NULL;
    (void)SDL_snprintf(str, 99999, "%.17g", d);
    return str;
}


NuklearApp::NuklearApp() {
    _width = 640;
	_height = 480;
	_resized = true;
    _devicePixelRatio = 1.0;

	_window = nullptr;
	_renderer = nullptr;
    _ctx = nullptr;  // Nuklear context (the core piece)
    // _bg = {.r=0, .g=0, .b=0.0f, .a=1.0f};     // Background color (RGBA float)
    _useAntiAliasing = NK_ANTI_ALIASING_ON;
}


SDL_AppResult NuklearApp::Init() {
    SDL_SetAppMetadata("Nuklear", "1.0", "lgbt.lea.learn-sdl.nuklear");
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Nuklear", _width, _height, SDL_WINDOW_RESIZABLE, &_window, &_renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // enable VSync
    SDL_SetRenderVSync(_renderer, 1);

    // get device pixel ratio
    const float devicePixelRatio = SDL_GetWindowDisplayScale(_window);
    SDL_SetRenderScale(_renderer, devicePixelRatio, devicePixelRatio);
    _devicePixelRatio = devicePixelRatio;

    // Init Nuklear
    _ctx = nk_sdl_init(_window, _renderer, nk_sdl_allocator());


    // Font Baking
    struct nk_font_atlas *atlas;
    struct nk_font_config config = nk_font_config(0);
    struct nk_font *font;

    // Prepare the atlas
    atlas = nk_sdl_font_stash_begin(_ctx);          
    font = nk_font_atlas_add_default(atlas, 13 * _devicePixelRatio, &config); // Built-in font
    // Or load your own TTF:
    // font = nk_font_atlas_add_from_file(atlas, "MyFont.ttf", 16 * font_scale, &config);
    // Upload texture to GPU
    nk_sdl_font_stash_end(_ctx);                    

    // HiDPI hack: scale the font height back down so layouts are calculated correctly
    font->handle.height /= _devicePixelRatio;

    // Activate the font
    nk_style_set_font(_ctx, &font->handle);         

    // Start first input collection cycle
    nk_input_begin(_ctx);

    return SDL_APP_CONTINUE;
}

NuklearApp::~NuklearApp()
{
    if (_ctx) {
        nk_sdl_shutdown(_ctx);
        _ctx = nullptr;
    }

	if (_renderer) {
		SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;
	}

	if (_window) {
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}
}

SDL_AppResult NuklearApp::Iterate()
{
    // stop collecting input
    nk_input_end(_ctx);

    _demoWidget.Iterate(_ctx);

    if (_resized) {
        SDL_SetRenderLogicalPresentation(_renderer, _width, _height, SDL_LOGICAL_PRESENTATION_STRETCH);
        _resized = false;
    }
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    
    // Win95-like Background :D
    SDL_SetRenderDrawColorFloat(_renderer, 0.0f, 0.5f, 0.5f, 1.0f);
    SDL_RenderClear(_renderer);
    nk_sdl_render(_ctx, _useAntiAliasing);
    nk_sdl_update_TextInput(_ctx);

    SDL_RenderPresent(_renderer);

    // start collecting input again
    nk_input_begin(_ctx);

    return SDL_APP_CONTINUE;
}

SDL_AppResult NuklearApp::HandleEvent(SDL_Event* const event)
{
    switch (event->type) {
        case SDL_EVENT_QUIT: {
            return SDL_APP_SUCCESS;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            _width = event->window.data1;
            _height = event->window.data2;
            _resized = true;
            break;
        }
    }

    // IMPORTANT: convert event coordinates to renderer space
    SDL_ConvertEventToRenderCoordinates(_renderer, event);

    // Forward event to Nuklear
    nk_sdl_handle_event(_ctx, event);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}
