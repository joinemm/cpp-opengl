#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.h"
#include "shader.h"
#include "texture.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // resize viewport too if the window is resized
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  float sensitivity = 0.1f;
  camera.readMouse(xpos, ypos, sensitivity);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.changeFov(-(float)yoffset);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);

  } else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
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

void processInput(GLFWwindow *window) {
  float speed = 3.0f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.moveForward(speed * deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.moveForward(-speed * deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.moveSideways(-speed * deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.moveSideways(speed * deltaTime);
  }
}

GLFWwindow *initOpenGL() {
  glfwInit();

  // set version hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // don't allow resizing
  // This forces the window to be floating when using tiling wm
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  // create window
  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "opengl", NULL, NULL);

  if (window) {
    glfwMakeContextCurrent(window);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  } else {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
  }

  // init GLEW bindings
  GLenum status = glewInit();
  if (status != GLEW_OK) {
    std::cerr << "Error: " << glewGetErrorString(status) << std::endl;
  }

  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Using OpenGl " << glGetString(GL_VERSION) << std::endl;

  // set callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetKeyCallback(window, key_callback);

  // capture the cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  return window;
}

unsigned int createCube() {
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

  // read vertex coordinates into buffer 0
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // read texture coordinates into buffer 1
  glBindBuffer(GL_ARRAY_BUFFER, TXVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);

  // return our VAO for rendering
  return VAO;
}

int main(void) {
  GLFWwindow *window = initOpenGL();

  Shader simpleShader("shaders/shader.vert.glsl", "shaders/shader.frag.glsl");
  Texture texture("assets/terrain.png", GL_NEAREST, GL_REPEAT);

  unsigned int cubeVAO = createCube();

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, -1.0f, 0.0f),
      glm::vec3(1.0f, -1.0f, 0.0f),
      glm::vec3(2.0f, -1.0f, 0.0f),
      glm::vec3(2.0f, 0.0f, 0.0f),
  };

  // main render loop
  while (!glfwWindowShouldClose(window)) {

    // frame timing
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // std::cout << "FPS: " << (int)(1.0f / deltaTime + 0.5f) << std::endl;

    // get input and process callbacks
    processInput(window);
    glfwPollEvents();

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use shader
    simpleShader.use();

    // use texture
    texture.use(GL_TEXTURE0);

    // move camera and update shader matrices
    camera.render();
    simpleShader.setMat4("projection", camera.projection());
    simpleShader.setMat4("view", camera.view());

    // render some cubes
    glBindVertexArray(cubeVAO);

    for (unsigned int i = 0; i < std::size(cubePositions); i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      simpleShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // swap front and back buffer
    // aka, show the new frame
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
