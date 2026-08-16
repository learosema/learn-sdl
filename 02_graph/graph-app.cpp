#include <SDL3/SDL.h>
#include <cmath>

#include "graph-app.hpp"

SDL_AppResult GraphApp::Init() {
    SDL_SetAppMetadata("Graph", "1.0", "lgbt.lea.learn-sdl.graph");
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("Graph", _width, _height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    _window.reset(window);
    _renderer.reset(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult GraphApp::Iterate()
{
    if (_resized) {
        SDL_SetRenderLogicalPresentation(_renderer.get(), _width, _height, SDL_LOGICAL_PRESENTATION_STRETCH);
        _resized = false;
    }
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */

    SDL_SetRenderDrawColorFloat(_renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(_renderer.get());

    SDL_SetRenderDrawColorFloat(_renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
    for (int i = 0; i < 360; i++){
        double a = 1.0 + sin(now * .1) * 2.0;
        double b = 2.0 + sin(now * .1) * 2.0;
        double c = 3.0 + sin(now * .1) * 2.0;
        double d = 4.0 + sin(now * .1) * 2.0;
        double aR = double(_width) / double(_height);
        int cx = (_width / 2);
        int cy = (_height / 2);
        int w  = (double(cx) / aR - 10.0);
        int h  = cy - 10;
        int x1 = cx - w * cos(a * double(i) * SDL_PI_D / 180.0);
        int y1 = cy - h * sin(b * double(i) * SDL_PI_D / 180.0);
        int x2 = cx + w * cos(c * double(i) * SDL_PI_D / 180.0);
        int y2 = cy + h * sin(d * double(i) * SDL_PI_D / 180.0);

        const float red = (float) (0.5 + 0.5 * SDL_sin(now + double(i) * 0.1));
        const float green = (float) (0.5 + 0.5 * SDL_sin(now + double(i) * 0.1 + SDL_PI_D * 2 / 3));
        const float blue = (float) (0.5 + 0.5 * SDL_sin(now + double(i) * 0.1 + SDL_PI_D * 4 / 3));
        SDL_SetRenderDrawColorFloat(_renderer.get(), red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);

        SDL_RenderLine(_renderer.get(), x1, y1, x2, y2);
    }


    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(_renderer.get());

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult GraphApp::HandleEvent(const SDL_Event* const event)
{
	if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        _width = event->window.data1;
        _height = event->window.data2;
        _resized = true;
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}
