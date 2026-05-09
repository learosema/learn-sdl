/* clear.c ... */

/*
 * This example code creates an SDL window and renderer, and then clears the
 * window to a different color every frame, so you'll effectively get a window
 * that's smoothly fading between colors.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */


#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <fmt/core.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static int g_width = 640;
static int g_height = 480;
static bool g_size_changed = false;


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "lgbt.lea.renderer-clear");
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", g_width, g_height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, g_width, g_height, SDL_LOGICAL_PRESENTATION_STRETCH);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        g_width = event->window.data1;
        g_height = event->window.data2;
        g_size_changed = true;
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    if (g_size_changed) {
        SDL_SetRenderLogicalPresentation(renderer, g_width, g_height, SDL_LOGICAL_PRESENTATION_STRETCH);
        g_size_changed = false;
    }
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    
    SDL_SetRenderDrawColorFloat(renderer,0,0,0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
    
    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColorFloat(renderer,0,0,0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
    for (int i = 0; i < 360; i++){
        double a = 1.0 + sin(now * .1) * 2.0;
        double b = 2.0 + sin(now * .1) * 2.0;
        double c = 3.0 + sin(now * .1) * 2.0;
        double d = 4.0 + sin(now * .1) * 2.0;
        double aR = double(g_width) / double(g_height);
        int cx = (g_width / 2);
        int cy = (g_height / 2);
        int w  = (double(cx) / aR - 10.0);
        int h  = cy - 10;
        int x1 = cx - w * cos(a * double(i) * SDL_PI_D / 180.0);
        int y1 = cy - h * sin(b * double(i) * SDL_PI_D / 180.0);
        int x2 = cx + w * cos(c * double(i) * SDL_PI_D / 180.0);
        int y2 = cy + h * sin(d * double(i) * SDL_PI_D / 180.0);

        const float red = (float) (0.5 + 0.5 * SDL_sin(now + double(i) * 0.1));
        const float green = (float) (0.5 + 0.5 * SDL_sin(now + double(i) * 0.1 + SDL_PI_D * 2 / 3));
        const float blue = (float) (0.5 + 0.5 * SDL_sin(now + double(i) * 0.1 + SDL_PI_D * 4 / 3));
        SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }


    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
