#include <SDL3/SDL.h>
#include "shader-utils.h"



GLuint CompileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        SDL_Log("Shader compile error: %s", log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint LinkProgram(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        SDL_Log("Program link error: %s", log);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}


GLuint CreateProgramFromShaders(const char *vertex_shader_source, const char * fragment_shader_source)
{
    GLuint vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_shader_source);
    if (!vertex_shader) {
        return 0;
    }
 
    GLuint fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_shader_source);
    if (!fragment_shader) {
        glDeleteShader(vertex_shader);
        return 0;
    }
 
    GLuint program = LinkProgram(vertex_shader, fragment_shader);
 
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
 
    if (! program) {
        return 0;
    }
 
    return program;
}
 