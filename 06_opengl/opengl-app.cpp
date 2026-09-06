#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

#include <cmath>

#include "opengl-app.h"
#include "SDL3/SDL_video.h"

#include <glad/glad.h>

#include "shader-utils.h"
#include "quad.vert.glsl.h"
#include "quad.frag.glsl.h"

namespace {
    constexpr float renderScale = 8.0f;


    constexpr float quadVertices[] = {
        -1.0f, -1.0f, // A
         1.0f, -1.0f, // B
        -1.0f,  1.0f, // C
 
        -1.0f,  1.0f, // C
         1.0f, -1.0f, // B
         1.0f,  1.0f, // D

    /* two counter-clockwise triangles that make up a plane 
             
        C___D
        |\  |
        | \ |    
        |  \|    
        A---B
    */
    };
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
    
    // required for macOS core profile, does nothing everywhere else
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
 
    _program = CreateProgramFromShaders(quadVertexShader, quadFragmentShader);
    if (! _program) {
        return SDL_APP_FAILURE;
    }
    
    _loc_utime  = glGetUniformLocation(_program, "uTime");

    if (! InitGeometry()) {
        return SDL_APP_FAILURE;
    }

    SDL_Log("OpenGL Version: %s", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    SDL_Log("GLSL Version:   %s", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    SDL_Log("Renderer:       %s", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
 
    return SDL_APP_CONTINUE;
}

OpenGLApp::~OpenGLApp()
{
    if (_program) glDeleteProgram(_program);
    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (_vao) glDeleteVertexArrays(1, &_vao);
}


bool OpenGLApp::InitGeometry() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
 
    glBindVertexArray(_vao);
 
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
 
    /* location 0 passt zu "layout (location = 0) in vec2 aPos" im Vertex-Shader */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
 
    glBindVertexArray(0);
 
    return true;
}

SDL_AppResult OpenGLApp::Iterate()
{
    if (_resized) {
        Resize();
    }

    const float t = static_cast<float>(SDL_GetTicks()) / 1000.0f;
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    glUseProgram(_program);
    glUniform1f(_loc_utime, t);
 
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);


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
