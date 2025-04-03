#ifndef RENDER_GL_H
#define RENDER_GL_H
#include <GL/gl.h>
typedef struct {
  GLuint id;
} VBO;

typedef struct {
  GLuint id;
} VAO;

typedef struct {
  GLuint id;
} EBO;

VBO vboInit();
void vboSet(VBO *vbo, float *data);
void vboUnbind(VBO *vbo);
void vboDelete(VBO *vbo);

VAO vaoInit();
void vaoSet(VAO *vao, float *data);
void vaoUnbind(VAO *vao);
void vaoDelete(VAO *vao);

#endif // !RENDER_GL_H
