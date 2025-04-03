#include "../include/render_gl.h"
#include "../include/shader_gl.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
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
  glewInit();
  glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
  const char *vertLocation = "./src/triangle.vert";
  const char *fragLocation = "./src/triangle.frag";

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
  fromShaders(&shaderProgram, shaders, 2);

  // VAO VBO EBO PART START
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f, // right
      0.0f,  0.5f,  0.0f  // top
  };

  VBO *vbo = vboInit();
  VAO *vao = vaoInit();

  vboSet(vbo, vertices, sizeof(vertices));
  vaoSet(vao, vertices, sizeof(vertices));
  // VAO VBO EBO PART END
  int running = 1;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = 0;
      }
    }

    // GL THINGS
    glClear(GL_COLOR_BUFFER_BIT);
    programSetUsed(&shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // Swap buffers
    SDL_GL_SwapWindow(window);
  }
  SDL_GL_DestroyContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
