#include "../include/render_gl.h"
#include <GL/glext.h>
#include <stdio.h>

/*VBO PART */
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
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

void vboDrop(VBO *vbo) {
  _vboUnbind(vbo);
  _vboDelete(vbo);
}
/*VAO PART */
VAO *vaoInit() {
  GLuint id = 0;
  glGenVertexArrays(1, &id);
  VAO *vao;
  vao->id = id;
  return vao;
}

void _vaoBind(VAO *vao) { glBindVertexArray(vao->id); }

void _vaoUnbind() { glBindVertexArray(0); }

void _vaoDelete(VAO *vao) { glDeleteVertexArrays(1, &vao->id); }

void vaoSet(VAO *vao, unsigned int type, float *data, size_t size) {
  _vaoBind(vao);
  switch (type) {
  case 0: {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    break;
  }
  case 1: {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    break;
  }
  case 2: {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    break;
  }
  }
}

void vaoDrop(VAO *vao) {
  _vaoUnbind();
  _vaoDelete(vao);
}

/*EBO PART */
EBO *eboInit() {
  GLuint id = 0;
  glGenBuffers(1, &id);
  EBO *ebo;
  ebo->id = id;
  return ebo;
}

void _eboBind(EBO *ebo) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id); }
void _eboUnbind(EBO *ebo) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
void _eboDelete(EBO *ebo) { glDeleteBuffers(1, &ebo->id); }

void eboSet(EBO *ebo, unsigned int *data, size_t size) {
  _eboBind(ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data,
               GL_STATIC_DRAW);
}
void eboDrop(EBO *ebo) {
  _eboUnbind(ebo);
  _eboDelete(ebo);
}
