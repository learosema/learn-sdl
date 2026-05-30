#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <cassert>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "graph-app.hpp"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    GraphApp* app = new GraphApp();
    *appstate = app;

    return app->Init();
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (!appstate) {
        return SDL_APP_FAILURE;
    }

    GraphApp* app = static_cast<GraphApp*>(appstate);
    return app->HandleEvent(event);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    if (!appstate) return SDL_APP_FAILURE;
    GraphApp* app = static_cast<GraphApp*>(appstate);
    return app->Iterate();
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    GraphApp* app = static_cast<GraphApp*>(appstate);
    delete app;
}
