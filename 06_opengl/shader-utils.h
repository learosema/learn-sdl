#ifndef __SHADER_UTILS_H__
#define __SHADER_UTILS_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <glad/glad.h>

/**
 * compiles a shader
 * \param type GLenum::VERTEX_SHADER | GLenum::FRAGMENT_SHADER
 * \param source the GLSL code
 */
GLuint CompileShader(GLenum type, const char* source);

/**
 * creates a program from vertex_shader and fragment_shader handles 
 */
GLuint LinkProgram(GLuint vertex_shader, GLuint fragment_shader);

/**
 * convenience function to create a program from vertex and fragment shader sources
 */
GLuint CreateProgramFromShaders(const char *vertex_shader_source, const char *fragment_shader_source);


#ifdef __cplusplus
}
#endif

#endif