#include "../include/shader_gl.h"
#include <stdio.h>

char *readShaderFile(const char *filePath) {
  FILE *file = fopen(filePath, "rb");
  if (!file) {
    printf("Error: Cannot open file\n%s\n", filePath);
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(fileSize + 1);
  if (!buffer) {
    printf("Buffer allocation error");
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, fileSize, file);
  buffer[fileSize] = '\0';
  fclose(file);
  return buffer;
}

void fromShaders(Program *program, Shader *shaders, int count) {
  GLuint id = glCreateProgram();

  for (int i = 0; i < count; i++) {
    glAttachShader(id, shaders[i].id);
  }
  glLinkProgram(id);

  GLint success = 1;
  glGetProgramiv(id, GL_LINK_STATUS, &success);

  if (success == 0) {
    GLint logLength = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 0) {
      char *log = (char *)malloc(logLength);
      glGetProgramInfoLog(id, logLength, NULL, log);
      printf("Program link error:\n%s\n", log);
      free(log);
    } else {
      printf("Program link error but no log available.\n");
    }
  }

  for (int i = 0; i < count; i++) {
    glDetachShader(id, shaders[i].id);
  }
  program->id = id;
}
void programSetUsed(Program *program) { glUseProgram(program->id); }
void programDrop(Program *program) { glDeleteProgram(program->id); }

void shaderFromSource(Shader *shader, const char *source, GLenum kind) {
  if (source == NULL) {
    printf("Error shader source is null\n");
    return;
  }
  GLuint id = glCreateShader(kind);
  glShaderSource(id, 1, &source, NULL);
  glCompileShader(id);
  GLint success = 1;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (success == 0) {
    GLint logLength = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 0) {
      char *log = (char *)malloc(logLength);
      glGetShaderInfoLog(id, logLength, NULL, log);
      printf("Shader compile error:\n%s\n", log);
      free(log);
    } else {
      printf("Shader compile error but no log available.\n");
    }
  }
  shader->id = id;
}
void programTransform(Program *program, const mat4 *transform) {
  unsigned int transformLocation =
      glGetUniformLocation(program->id, "transform");
  glUniformMatrix4fv(transformLocation, 1, GL_FALSE,
                     (const GLfloat *)transform);
}

void shaderDrop(Shader *shader) { glDeleteShader(shader->id); }
