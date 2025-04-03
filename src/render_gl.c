#include "../include/render_gl.h"

VBO *vboInit() {
  GLuint id = 0;
  glGenBuffers(1, &id);
  VBO *vbo;
  vbo->id = id;
  return vbo;
}

void _vboBind(VBO *vbo) { glBindBuffer(GL_ARRAY_BUFFER, vbo->id); }

void _vboUnbind(VBO *vbo) { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void _vboDelete(VBO *vbo) { glDeleteBuffers(1, &vbo->id); }

void vboSet(VBO *vbo, float *data, size_t size) {
  _vboBind(vbo);
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(data[0]), data, GL_STATIC_DRAW);
}

VAO *vaoInit() {
  GLuint id = 0;
  glGenVertexArrays(1, &id);
  VAO *vao;
  vao->id = id;
  return vao;
}

void _vaoBind(VAO *vao) { glBindVertexArray(vao->id); }

void _vaoUnbind(VAO *vao) { glBindVertexArray(0); }

void _vaoDelete(VAO *vao) { glDeleteVertexArrays(1, &vao->id); }

void vaoSet(VAO *vao, float *data, size_t size) {
  _vaoBind(vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(data[0]), NULL);
}
