#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>

unsigned int loadShader(const std::string &path, GLenum shaderType) {
  // read file contents
  std::ifstream shaderFile(path);
  std::ostringstream buffer;
  buffer << shaderFile.rdbuf();
  std::string bufferStr = buffer.str();
  const GLchar *source = bufferStr.c_str();

  // compile shader
  unsigned int shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  // check for compile errors
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "ERROR: \"" << path << "\" SHADER COMPILATION FAILED\n"
              << infoLog << std::endl;
  }

  return shader;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main(void) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // This forces the window to be floating when using tiling wm
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "opengl", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  std::cout << "Using OpenGl " << glGetString(GL_VERSION) << std::endl;

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
  }
  std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

  // load shaders
  unsigned int shaderProgram = glCreateProgram();

  // vertex shader
  unsigned int vertexShader = loadShader("shader.vert.glsl", GL_VERTEX_SHADER);
  glAttachShader(shaderProgram, vertexShader);

  // fragment shader
  unsigned int fragmentShader =
      loadShader("shader.frag.glsl", GL_FRAGMENT_SHADER);
  glAttachShader(shaderProgram, fragmentShader);

  // link the shaders to the program
  glLinkProgram(shaderProgram);

  // delete the shader objects.
  // we don't need them anymore as they are linked to the program
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // check for compile errors
  int success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR: SHADER LINKING FAILED\n" << infoLog << std::endl;
  }

  // define our vertices
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, //
      0.5f,  -0.5f, 0.0f, //
      0.0f,  0.5f,  0.0f  //
  };

  // create vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // create vertex buffer object
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // tell opengl how to interpret our vertex buffer
  // and load the data into location 0 for the shader to use
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // main render loop
  while (!glfwWindowShouldClose(window)) {
    // process any input
    processInput(window);

    // do rendering
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // poll and call events
    glfwPollEvents();

    // swap front and back buffer
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
