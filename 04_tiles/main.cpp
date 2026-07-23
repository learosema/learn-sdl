#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <cassert>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "tiles-app.hpp"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    TilesApp* app = new TilesApp();
    *appstate = app;

    return app->Init();
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (!appstate) {
        return SDL_APP_FAILURE;
    }

    TilesApp* app = static_cast<TilesApp*>(appstate);
    return app->HandleEvent(event);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    if (!appstate) return SDL_APP_FAILURE;
    TilesApp* app = static_cast<TilesApp*>(appstate);
    return app->Iterate();
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    TilesApp* app = static_cast<TilesApp*>(appstate);
    delete app;
}
