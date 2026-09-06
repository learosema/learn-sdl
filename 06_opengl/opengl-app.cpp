#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

#include <cmath>

#include "opengl-app.hpp"
#include "SDL3/SDL_video.h"

#include <glad/glad.h>
// #include "scifi-font.h"

namespace {
    constexpr float RENDER_SCALE = 8.0f;
}

SDL_AppResult OpenGLApp::Init() {
    SDL_SetAppMetadata("OpenGL", "1.0", "lgbt.lea.learn-sdl.framebuffer");
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    /* auf macOS zwingend fuer Core-Profile-Contexte, auf Linux/Windows wirkungslos */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);



    SDL_Window* window = SDL_CreateWindow("OpenGL", _width, _height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    _window.reset(window);
 
    SDL_GLContext gl_context = SDL_GL_CreateContext(_window.get());
    if (!gl_context) {
        SDL_Log("Couldn't create GL context: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    _gl_context.reset(gl_context);
 
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_Log("gladLoadGLLoader failed");
        return SDL_APP_FAILURE;
    }
 
    SDL_GL_SetSwapInterval(1);
 
    SDL_Log("OpenGL Version: %s", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    SDL_Log("GLSL Version:   %s", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    SDL_Log("Renderer:       %s", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
 
    return SDL_APP_CONTINUE;
}


SDL_AppResult OpenGLApp::Iterate()
{
    if (_resized) {
        Resize();
    }

    const float t = static_cast<float>(SDL_GetTicks()) / 1000.0f;
    glClearColor(0.5f + 0.5f * std::sin(t), 0.2f, 0.5f + 0.5f * std::cos(t), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    SDL_GL_SwapWindow(_window.get());

    return SDL_APP_CONTINUE;
}

bool OpenGLApp::Resize() {
    
    glViewport(0, 0, _width, _height);
    _resized = false;
    return true;
}


SDL_AppResult OpenGLApp::HandleEvent(const SDL_Event* const event)
{
	if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; 
    }

    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        _width = event->window.data1;
        _height = event->window.data2;
        _resized = true;
    }

    return SDL_APP_CONTINUE;
}
