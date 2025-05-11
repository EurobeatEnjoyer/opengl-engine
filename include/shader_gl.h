#ifndef SHADER_GL_H
#define SHADER_GL_H
// dont touch this
#include <GL/glew.h>
// dont touch
#include "cglm/cglm.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

// Shader part
typedef struct {
  GLuint id;
} Shader;

void shaderFromSource(Shader *shader, const char *source, GLenum kind);
void shaderDrop(Shader *shader);

// Program part
typedef struct {
  GLuint id;
} Program;

void fromShaders(Program *program, Shader *shaders, int count);
void programSetUsed(Program *program);
void programDrop(Program *program);
void programTransform(Program *program, const mat4 *transform);

// Shader read part
char *readShaderFile(const char *filePath);

#endif // !SHADER_GL_H
