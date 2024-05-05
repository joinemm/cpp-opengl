#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main(void) {
  std::cout << "Hello window!" << std::endl;
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
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  // main render loop
  while (!glfwWindowShouldClose(window)) {
    // process any input
    processInput(window);

    // do rendering
    glClear(GL_COLOR_BUFFER_BIT);

    // poll and call events
    glfwPollEvents();

    // swap front and back buffer
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
