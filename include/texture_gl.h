#ifndef TEXTURE_GL_H
#define TEXTURE_GL_H
#include <GL/glew.h>
#include <malloc.h>
typedef struct {
  GLuint id;
  const char *location;
} Texture;

Texture *textureInitWithLocation(const char *location);
void textureDefaults2D(Texture *texture);

#endif // !TEXTURE_GL_H
