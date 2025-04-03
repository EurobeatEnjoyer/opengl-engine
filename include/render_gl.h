#ifndef RENDER_GL_H
#define RENDER_GL_H
#include <GL/glew.h>
// dont touch
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

VBO *vboInit();
void vboSet(VBO *vbo, float *data, size_t size);
void vboDrop(VBO *vbo);

VAO *vaoInit();
void vaoSet(VAO *vao, float *data, size_t size);
void vaoDrop(VAO *vao);

EBO *eboInit();
void eboSet(EBO *ebo, float *data, size_t size);
void eboDrop(EBO *ebo);

void _eboSetup(EBO *ebo);
void _eboUnbind(EBO *ebo);
void _eboDelete(EBO *ebo);

void _vaoSetup(VAO *vao);
void _vaoUnbind(VAO *vao);
void _vaoDelete(VAO *vao);

void _vboBind(VBO *vbo);
void _vboUnbind(VBO *vbo);
void _vboDelete(VBO *vbo);

#endif // !RENDER_GL_H
