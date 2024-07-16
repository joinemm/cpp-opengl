#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "camera.h"
#include "shader.h"
#include "texture.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool freecam = false;

Camera camera((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // resize viewport too if the window is resized
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (freecam &&
      glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
    float sensitivity = 0.1f;
    camera.readMouse(xpos, ypos, sensitivity);
  }
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
      std::cout << "pew!" << std::endl;
    }
  }
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
    GLenum newMode;
    if (polygonMode[0] == GL_LINE) {
      newMode = GL_FILL;
      glEnable(GL_CULL_FACE);
    } else {
      newMode = GL_LINE;
      glDisable(GL_CULL_FACE);
    };

    glPolygonMode(GL_FRONT_AND_BACK, newMode);

  } else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
    freecam = !freecam;

  } else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    camera.firstMouse = true;
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
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera.moveUp(speed * deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    camera.moveUp(-speed * deltaTime);
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
  glfwSetMouseButtonCallback(window, mouse_button_callback);

  // capture the cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // raw input
  glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  return window;
}

std::vector<float> cubeVertices(float x, float y, float z) {
  std::vector<float> cubeVertices{
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
  return cubeVertices;
};

unsigned int createCube() {
  // float blockSize = 16.0f;
  // float atlasSize = 256.0f;
  float blockSize = 1.0f;
  float atlasSize = 1.0f;
  float bw = blockSize / atlasSize;

  float ba[] = {
      1 * bw, 1 * bw, // a
      1 * bw, 1 * bw, // b
      1 * bw, 1 * bw, // c
      1 * bw, 1 * bw, // d
      1 * bw, 1 * bw, // e = bottom
      1 * bw, 1 * bw, // f = top
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

  float normals[]{
      -1.0f, 0.0f,  0.0f,  //
      -1.0f, 0.0f,  0.0f,  //
      -1.0f, 0.0f,  0.0f,  //
      -1.0f, 0.0f,  0.0f,  //
      -1.0f, 0.0f,  0.0f,  //
      -1.0f, 0.0f,  0.0f,  //
      0.0f,  0.0f,  -1.0f, //
      0.0f,  0.0f,  -1.0f, //
      0.0f,  0.0f,  -1.0f, //
      0.0f,  0.0f,  -1.0f, //
      0.0f,  0.0f,  -1.0f, //
      0.0f,  0.0f,  -1.0f, //
      0.0f,  0.0f,  1.0f,  //
      0.0f,  0.0f,  1.0f,  //
      0.0f,  0.0f,  1.0f,  //
      0.0f,  0.0f,  1.0f,  //
      0.0f,  0.0f,  1.0f,  //
      0.0f,  0.0f,  1.0f,  //
      1.0f,  0.0f,  0.0f,  //
      1.0f,  0.0f,  0.0f,  //
      1.0f,  0.0f,  0.0f,  //
      1.0f,  0.0f,  0.0f,  //
      1.0f,  0.0f,  0.0f,  //
      1.0f,  0.0f,  0.0f,  //
      0.0f,  -1.0f, 0.0f,  //
      0.0f,  -1.0f, 0.0f,  //
      0.0f,  -1.0f, 0.0f,  //
      0.0f,  -1.0f, 0.0f,  //
      0.0f,  -1.0f, 0.0f,  //
      0.0f,  -1.0f, 0.0f,  //
      0.0f,  1.0f,  0.0f,  //
      0.0f,  1.0f,  0.0f,  //
      0.0f,  1.0f,  0.0f,  //
      0.0f,  1.0f,  0.0f,  //
      0.0f,  1.0f,  0.0f,  //
      0.0f,  1.0f,  0.0f,  //

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
  unsigned int VBO, TXVBO, NVBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &TXVBO);
  glGenBuffers(1, &NVBO);

  std::vector<float> vertices = cubeVertices(0.5f, 0.5f, 0.5f);

  // read vertex coordinates into buffer 0
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(float) * vertices.size()),
               &vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // read texture coordinates into buffer 1
  glBindBuffer(GL_ARRAY_BUFFER, TXVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);

  // read normals into buffer 2
  glBindBuffer(GL_ARRAY_BUFFER, NVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(2);

  // return our VAO for rendering
  return VAO;
}

unsigned int createLightsource() {
  // create vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // create vertex buffer object
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  std::vector<float> vertices = cubeVertices(0.2f, 0.2f, 0.2f);

  // read vertex coordinates into buffer 0
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(float) * vertices.size()),
               &vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        static_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // return our VAO for rendering
  return VAO;
}

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};

int main(void) {
  GLFWwindow *window = initOpenGL();

  Shader simpleShader("shaders/standard.vert.glsl", "shaders/phong.frag.glsl");
  Shader lampShader("shaders/standard.vert.glsl", "shaders/lamp.frag.glsl");
  Texture texture("assets/container2.png", GL_NEAREST, GL_REPEAT);

  unsigned int cubeVAO = createCube();
  unsigned int lightVAO = createLightsource();

  Material gold = {
      glm::vec3(0.24725f, 0.1995f, 0.0745f),
      glm::vec3(0.75164f, 0.60648f, 0.22648f),
      glm::vec3(0.628281f, 0.555802f, 0.366065f),
      0.4f,
  };

  Material crate = {
      glm::vec3(0.1f, 0.1f, 0.2f),
      glm::vec3(1.0f),
      glm::vec3(1.0f),
      0.2f,
  };

  std::vector cubePositions = {
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1.3f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.3f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.3f),
  };

  std::vector cubeMaterials = {
      gold,
      crate,
      crate,
      crate,
  };

  camera.setPosition(glm::vec3(4.0f, 4.0f, 4.0f));

  glm::vec3 lightPos = glm::vec3(-0.2f, 3.0f, 3.0f);
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

    // update camera projections
    glm::mat4 view;
    if (freecam) {
      view = camera.eulerView();
    } else {
      view = camera.lookAtView(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    float radius = 3.0f;
    float x = sin(glfwGetTime()) * radius;
    float z = cos(glfwGetTime()) * radius;
    lightPos = glm::vec3(x, 3.0f, z);
    // render light source
    lampShader.use();
    lampShader.setMat4("projection", camera.projection());
    lampShader.setMat4("view", view);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    lampShader.setMat4("model", lightModel);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // render some cubes
    simpleShader.use();
    texture.use(GL_TEXTURE0);

    simpleShader.setMat4("projection", camera.projection());
    simpleShader.setMat4("view", view);

    simpleShader.setVec3("light.ambient", glm::vec3(1.0f));
    simpleShader.setVec3("light.brightness", glm::vec3(1.0f));
    simpleShader.setVec3("light.position", lightPos);

    simpleShader.setVec3("viewPos", camera.pos);
    glBindVertexArray(cubeVAO);

    for (unsigned int i = 0; i < std::size(cubePositions); i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      simpleShader.setMat4("model", model);
      simpleShader.setVec3("material.ambient", cubeMaterials[i].ambient);
      simpleShader.setVec3("material.diffuse", cubeMaterials[i].diffuse);
      simpleShader.setVec3("material.specular", cubeMaterials[i].specular);
      simpleShader.setFloat("material.shininess", cubeMaterials[i].shininess);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // swap front and back buffer
    // aka, show the new frame
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
