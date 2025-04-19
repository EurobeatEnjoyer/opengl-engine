#include "../include/cglm/cglm.h"
#include "../include/render_gl.h"
#include "../include/shader_gl.h"
#include "../include/texture_gl.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

int _texture_rendering() {
  SDL_InitFlags flags = SDL_INIT_VIDEO;
  if (!SDL_Init(flags)) {
    printf("Init error bro%s\n", SDL_GetError());
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window *window = SDL_CreateWindow("yeag", 800, 600, SDL_WINDOW_OPENGL);
  if (!window) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  SDL_GLContext glContext = SDL_GL_CreateContext(window);
  if (!glContext) {
    printf("OpenGL context could not be created! SDL_Error: %s\n",
           SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }
  /* SDL_GL_SetSwapInterval(0); */
  glewInit();
  glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
  const char *vertLocation = "assets/texture.vs";
  const char *fragLocation = "assets/texture.fs";

  char *vertSource = readShaderFile(vertLocation);
  char *fragSource = readShaderFile(fragLocation);

  Shader vertShader;
  Shader fragShader;
  shaderFromSource(&vertShader, vertSource, GL_VERTEX_SHADER);
  shaderFromSource(&fragShader, fragSource, GL_FRAGMENT_SHADER);
  free(vertSource);
  free(fragSource);

  Program shaderProgram;
  Shader shaders[2] = {vertShader, fragShader};
  fromShaders(&shaderProgram, shaders, sizeof(shaders));

  // VAO VBO EBO PART START
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /* float textureVertices[] = { */
  /*     // positions        // colors         // texture coords */
  /*     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right */
  /*     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right */
  /*     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left */
  /*     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left */
  /* }; */

  float cubeVertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
                          0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  //
                          0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
                          0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
                          -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  //
                          -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
                                                           //
                          -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
                          0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   //
                          0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    //
                          0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    //
                          -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   //
                          -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
                                                           //
                          -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   //
                          -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  //
                          -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
                          -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
                          -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
                          -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   //
                                                           //
                          0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
                          0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
                          0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
                          0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
                          0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   //
                          0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
                                                           //
                          -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
                          0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  //
                          0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   //
                          0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   //
                          -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
                          -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
                                                           //
                          -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  //
                          0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
                          0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
                          0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
                          -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,   //
                          -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

  /* float cubePositions[] = {(vec3)((float[]){0.0f, 0.0f, 0.0f}), */
  /*                          (vec3)(float[]){2.0f, 5.0f, -15.0f}, */
  /*                          (vec3)(float[]){-1.5f, -2.2f, -2.5f}, */
  /*                          (vec3)(float[]){-3.8f, -2.0f, -12.3f}, */
  /*                          (vec3)(float[]){2.4f, -0.4f, -3.5f}, */
  /*                          (vec3)(float[]){-1.7f, 3.0f, -7.5f}, */
  /*                          (vec3)(float[]){1.3f, -2.0f, -2.5f}, */
  /*                          (vec3)(float[]){1.5f, 2.0f, -2.5f}, */
  /*                          (vec3)(float[]){1.5f, 0.2f, -1.5f}, */
  /*                          (vec3)(float[]){-1.3f, 1.0f, -1.5f} */
  /**/
  /* }; */

  vec3 vec = {0.0f, 0.0f, 1.0f};
  vec3 scaling_vector = {0.5f, 0.5f, 0.5f};
  mat4 trans;
  glm_mat4_identity(trans);
  /* for (size_t i = 0; i < 4; i++) { */
  /*   for (size_t j = 0; j < 4; j++) { */
  /*     trans[i][j] = 1; */
  /*   } */
  /* } */
  glm_translate(trans, vec);
  /* for (size_t i = 0; i < 4; i++) { */
  /*   for (size_t j = 0; j < 4; j++) { */
  /*     printf("%f,", trans[i][j]); */
  /*   } */
  /*   printf("\n"); */
  /* } */
  glm_scale(trans, scaling_vector);

  float vertices[] = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f, // right
      0.0f,  0.5f,  0.0f  // top
  };

  float colorVertices[] = {
      // positions         // colors
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top right
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  float eboVertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
  };

  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  VAO vao = vaoInit();
  VBO vbo = vboInit();
  EBO ebo = eboInit();

  /* VBO COMES BEFORE VAO!!!! VBO -> VAO -> EBO */
  vboSet(&vbo, textureVertices, sizeof(textureVertices));
  vaoSet(&vao, 2, textureVertices, sizeof(textureVertices));
  eboSet(&ebo, indices, sizeof(indices));
  /////////////////////////////////////////////////////////////////////////////////////////////////
  // VAO VBO EBO PART END
  /////////////////////////////////////////////////////////////////////////////////////////////////
  // TEXTURE START
  /////////////////////////////////////////////////////////////////////////////////////////////////
  Texture *texture = textureInitWithLocation("assets/wall.jpg");
  textureDefaults2D(texture);
  Uint64 frametime = SDL_GetTicks();
  int frames = 0;

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // TEXTURE END
  int running = 1;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = 0;
      }
    }
    frames++;
    if ((SDL_GetTicks() - frametime) >= 1000) {
      printf("fps: %d\n", frames);
      frametime = SDL_GetTicks();
      frames = 0;
    }
    /* printf("%" PRId64 "\n", SDL_GetTicks()); */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glm_rotate(trans, SDL_GetTicks(), vec);
    programTransform(&shaderProgram, &trans);
    /* unsigned int transformLocation = */
    /*     glGetUniformLocation(shaderProgram.id, "transform"); */
    /* unsigned int attriblocation = glGetAttribLocation(shaderProgram.id,
     * "aPos"); */
    /* printf("Transform location: %d\n", transformLocation); */
    /* glUniformMatrix4fv(transformLocation, 1, GL_FALSE, (const GLfloat
     * *)trans); */
    programSetUsed(&shaderProgram);

    // GL THINGS
    /* glDrawArrays(GL_TRIANGLES, 0, 3); */
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Swap buffers
    SDL_GL_SwapWindow(window);
  }
  vaoDrop(&vao);
  vboDrop(&vbo);
  eboDrop(&ebo);
  SDL_GL_DestroyContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

int main(int argc, char *argv[]) {
  _texture_rendering();
  /* SDL_InitFlags flags = SDL_INIT_VIDEO; */
  /* if (!SDL_Init(flags)) { */
  /*   printf("Init error bro%s\n", SDL_GetError()); */
  /* } */
  /**/
  /* SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); */
  /* SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); */
  /* SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
   * SDL_GL_CONTEXT_PROFILE_CORE); */
  /**/
  /* SDL_Window *window = SDL_CreateWindow("yeag", 800, 600, SDL_WINDOW_OPENGL);
   */
  /* if (!window) { */
  /*   printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
   */
  /*   SDL_Quit(); */
  /*   return -1; */
  /* } */
  /**/
  /* SDL_GLContext glContext = SDL_GL_CreateContext(window); */
  /* if (!glContext) { */
  /*   printf("OpenGL context could not be created! SDL_Error: %s\n", */
  /*          SDL_GetError()); */
  /*   SDL_DestroyWindow(window); */
  /*   SDL_Quit(); */
  /*   return -1; */
  /* } */
  /* glewInit(); */
  /* glClearColor(0.3f, 0.3f, 0.5f, 1.0f); */
  /* const char *vertLocation = "assets/texture.vs"; */
  /* const char *fragLocation = "assets/texture.fs"; */
  /**/
  /* char *vertSource = readShaderFile(vertLocation); */
  /* char *fragSource = readShaderFile(fragLocation); */
  /**/
  /* Shader vertShader; */
  /* Shader fragShader; */
  /* shaderFromSource(&vertShader, vertSource, GL_VERTEX_SHADER); */
  /* shaderFromSource(&fragShader, fragSource, GL_FRAGMENT_SHADER); */
  /* free(vertSource); */
  /* free(fragSource); */
  /**/
  /* Program shaderProgram; */
  /* Shader shaders[2] = {vertShader, fragShader}; */
  /* fromShaders(&shaderProgram, shaders, sizeof(shaders)); */
  /**/
  /* // VAO VBO EBO PART START */
  /* /////////////////////////////////////////////////////////////////////////////////////////////////
   */
  /* float textureVertices[] = { */
  /*     // positions        // colors         // texture coords */
  /*     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right */
  /*     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right */
  /*     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left */
  /*     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left */
  /* }; */
  /**/
  /* vec3 vec = {0.0f, 0.0f, 1.0f}; */
  /* mat4 trans; */
  /* for (size_t i = 0; i < 4; i++) { */
  /*   for (size_t j = 0; j < 4; j++) { */
  /*     trans[i][j] = 1; */
  /*   } */
  /* } */
  /* glm_rotate(trans, 90.0f, vec); */
  /* for (size_t i = 0; i < 4; i++) { */
  /*   for (size_t j = 0; j < 4; j++) { */
  /*     printf("%f,", trans[i][j]); */
  /*   } */
  /*   printf("\n"); */
  /* } */
  /**/
  /* float vertices[] = { */
  /*     -0.5f, -0.5f, 0.0f, // left */
  /*     0.5f,  -0.5f, 0.0f, // right */
  /*     0.0f,  0.5f,  0.0f  // top */
  /* }; */
  /**/
  /* float colorVertices[] = { */
  /*     // positions         // colors */
  /*     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top right */
  /*     0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right */
  /*     -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left */
  /*     -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top left */
  /* }; */
  /**/
  /* float eboVertices[] = { */
  /*     0.5f,  0.5f,  0.0f, // top right */
  /*     0.5f,  -0.5f, 0.0f, // bottom right */
  /*     -0.5f, -0.5f, 0.0f, // bottom left */
  /*     -0.5f, 0.5f,  0.0f  // top left */
  /* }; */
  /**/
  /* unsigned int indices[] = { */
  /*     0, 1, 3, // first triangle */
  /*     1, 2, 3  // second triangle */
  /* }; */
  /**/
  /* VAO *vao = vaoInit(); */
  /* VBO *vbo = vboInit(); */
  /* EBO *ebo = eboInit(); */
  /**/
  // /* VBO COMES BEFORE VAO!!!! VBO -> VAO -> EBO */ */
  /* vboSet(vbo, textureVertices, sizeof(textureVertices)); */
  /* vaoSet(vao, 2, textureVertices, sizeof(textureVertices)); */
  /* eboSet(ebo, indices, sizeof(indices)); */
  /* /////////////////////////////////////////////////////////////////////////////////////////////////
   */
  /* // VAO VBO EBO PART END */
  /* /////////////////////////////////////////////////////////////////////////////////////////////////
   */
  /* // TEXTURE START */
  /* /////////////////////////////////////////////////////////////////////////////////////////////////
   */
  /* Texture *texture = textureInitWithLocation("assets/wall.jpg"); */
  /* textureDefaults2D(texture); */
  /**/
  /* /////////////////////////////////////////////////////////////////////////////////////////////////
   */
  /* // TEXTURE END */
  /* int running = 1; */
  /* SDL_Event event; */
  /* while (running) { */
  /*   while (SDL_PollEvent(&event)) { */
  /*     if (event.type == SDL_EVENT_QUIT) { */
  /*       running = 0; */
  /*     } */
  /*   } */
  /*   glClearColor(0.2f, 0.3f, 0.3f, 1.0f); */
  /*   glClear(GL_COLOR_BUFFER_BIT); */
  /*   unsigned int transformLocation = */
  /*       glGetUniformLocation(shaderProgram.id, "transform"); */
  /*   unsigned int attriblocation = glGetAttribLocation(shaderProgram.id,
   * "aPos"); */
  /*   printf("Transform location: %d, Attribute location: %d\n", */
  /*          transformLocation, attriblocation); */
  /*   programSetUsed(&shaderProgram); */
  /**/
  /*   // GL THINGS */
  //   /* glDrawArrays(GL_TRIANGLES, 0, 3); */
  /*   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); */
  /*   // Swap buffers */
  /*   SDL_GL_SwapWindow(window); */
  /* } */
  /* vaoDrop(vao); */
  /* vboDrop(vbo); */
  /* eboDrop(ebo); */
  /* SDL_GL_DestroyContext(glContext); */
  /* SDL_DestroyWindow(window); */
  /* SDL_Quit(); */
}
