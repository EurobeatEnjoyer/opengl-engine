#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/texture_gl.h"
Texture *textureInitWithLocation(const char *location) {
  Texture *texture = (Texture *)malloc(sizeof(Texture));
  glGenTextures(1, &texture->id);
  glBindTexture(GL_TEXTURE_2D, texture->id);
  texture->location = location;
  return texture;
}
void textureDefaults2D(Texture *texture) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int w, h, c;
  unsigned char *data = stbi_load(texture->location, &w, &h, &c, 0);

  if (data) {
    printf("Loaded image: %s (%d x %d), channels: %d\n", texture->location, w,
           h, c);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("failed to load the texture%s", texture->location);
  }

  stbi_image_free(data);
}
