#include "SDL3/SDL_init.h"
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <cassert>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <nuklear.h>
#include "nuklear-app.hpp"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    NuklearApp* app = new NuklearApp();
    *appstate = app;

    return app->Init();
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (!appstate) return SDL_APP_FAILURE;
    NuklearApp* app = static_cast<NuklearApp*>(appstate);
    return app->HandleEvent(event);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    if (!appstate) return SDL_APP_FAILURE;
    NuklearApp* app = static_cast<NuklearApp*>(appstate);
    return app->Iterate();
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    NuklearApp* app = static_cast<NuklearApp*>(appstate);
    delete app;
}
