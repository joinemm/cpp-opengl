#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "shader.h"
#include "texture.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    // toggle between fill and wireframe
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);
    GLint newMode;
    if (polygonMode[0] == GL_LINE) {
      newMode = GL_FILL;
      glEnable(GL_CULL_FACE);
    } else {
      newMode = GL_LINE;
      glDisable(GL_CULL_FACE);
    };

    glPolygonMode(GL_FRONT_AND_BACK, newMode);
  }
}

int main(void) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // This forces the window to be floating when using tiling wm
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "opengl", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Using OpenGl " << glGetString(GL_VERSION) << std::endl;

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSetKeyCallback(window, key_callback);

  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
  }
  std::cout << "Initialized GLEW " << glewGetString(GLEW_VERSION) << std::endl;

  float x = 0.5f;
  float y = 0.5f;
  float z = 0.5f;

  float vertices[] = {
      -x, -y, -z, // a
      -x, -y, z,  // a
      -x, y,  z,  // a
      -x, -y, -z, // a
      -x, y,  z,  // a
      -x, y,  -z, // a
      x,  y,  -z, // b
      -x, -y, -z, // b
      -x, y,  -z, // b
      x,  y,  -z, // b
      x,  -y, -z, // b
      -x, -y, -z, // b
      -x, y,  z,  // c
      -x, -y, z,  // c
      x,  -y, z,  // c
      x,  y,  z,  // c
      -x, y,  z,  // c
      x,  -y, z,  // c
      x,  y,  z,  // d
      x,  -y, -z, // d
      x,  y,  -z, // d
      x,  -y, -z, // d
      x,  y,  z,  // d
      x,  -y, z,  // d
      x,  -y, z,  // e
      -x, -y, -z, // e
      x,  -y, -z, // e
      x,  -y, z,  // e
      -x, -y, z,  // e
      -x, -y, -z, // e
      x,  y,  z,  // f
      x,  y,  -z, // f
      -x, y,  -z, // f
      x,  y,  z,  // f
      -x, y,  -z, // f
      -x, y,  z,  // f

  };

  float blockSize = 16.0f;
  float atlasSize = 256.0f;
  float bw = blockSize / atlasSize;

  float ba[] = {
      12 * bw, 12 * bw, // a
      11 * bw, 12 * bw, // b
      11 * bw, 12 * bw, // c
      12 * bw, 12 * bw, // d
      4 * bw,  15 * bw, // e = bottom
      11 * bw, 13 * bw, // f = top
  };

  float texcoord[]{
      ba[0],       ba[1],       // a
      ba[0] + bw,  ba[1],       // a
      ba[0] + bw,  ba[1] + bw,  // a
      ba[0],       ba[1],       // a
      ba[0] + bw,  ba[1] + bw,  // a
      ba[0],       ba[1] + bw,  // a
      ba[2],       ba[3] + bw,  // b
      ba[2] + bw,  ba[3],       // b
      ba[2] + bw,  ba[3] + bw,  // b
      ba[2],       ba[3] + bw,  // b
      ba[2],       ba[3],       // b
      ba[2] + bw,  ba[3],       // b
      ba[4],       ba[5] + bw,  // c
      ba[4],       ba[5],       // c
      ba[4] + bw,  ba[5],       // c
      ba[4] + bw,  ba[5] + bw,  // c
      ba[4],       ba[5] + bw,  // c
      ba[4] + bw,  ba[5],       // c
      ba[6],       ba[7] + bw,  // d
      ba[6] + bw,  ba[7],       // d
      ba[6] + bw,  ba[7] + bw,  // d
      ba[6] + bw,  ba[7],       // d
      ba[6],       ba[7] + bw,  // d
      ba[6],       ba[7],       // d
      ba[8],       ba[9],       // e
      ba[8] + bw,  ba[9] + bw,  // e
      ba[8],       ba[9] + bw,  // e
      ba[8],       ba[9],       // e
      ba[8] + bw,  ba[9],       // e
      ba[8] + bw,  ba[9] + bw,  // e
      ba[10] + bw, ba[11],      // f
      ba[10] + bw, ba[11] + bw, // f
      ba[10],      ba[11] + bw, // f
      ba[10] + bw, ba[11],      // f
      ba[10],      ba[11] + bw, // f
      ba[10],      ba[11],      // f

  };

  // create vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // create element buffer object
  // unsigned int EBO;
  // glGenBuffers(1, &EBO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

  // create vertex buffer object
  unsigned int VBO, TXVBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &TXVBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // tell opengl how to interpret our vertex buffer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, TXVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);

  Shader simpleShader("shaders/shader.vert.glsl", "shaders/shader.frag.glsl");
  Texture texture("assets/terrain.png", GL_NEAREST, GL_REPEAT);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  simpleShader.use();

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  glm::mat4 projection;
  projection =
      glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  int modelLoc = glGetUniformLocation(simpleShader.ID, "model");
  int viewLoc = glGetUniformLocation(simpleShader.ID, "view");
  int projectionLoc = glGetUniformLocation(simpleShader.ID, "projection");

  // main render loop
  while (!glfwWindowShouldClose(window)) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use texture
    glActiveTexture(GL_TEXTURE0);
    texture.use();

    // matrix transforms
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f),
                        glm::vec3(1.0f, 0.5f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // draw our triangles
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // poll and call events
    glfwPollEvents();

    // swap front and back buffer
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
