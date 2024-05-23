#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class Camera {
public:
  float fovMin = 1.0f;
  float fovMax = 90.0f;
  float vFov = 60.0f;
  float yaw = 0.0f;
  float pitch = 0.0f;
  bool firstMouse = true;

  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

  float aspectRatio;

  Camera(float aspectRatio);

  void moveForward(float amount);
  void moveSideways(float amount);
  void moveUp(float amount);
  void changeFov(float amount);
  void readMouse(double xpos, double ypos, float lookSensitivity);
  void setPosition(glm::vec3 newPos);
  void render();
  glm::mat4 projection();
  glm::mat4 lookAtView(glm::vec3 target);
  glm::mat4 eulerView();

private:
  float lastX, lastY;
};

#endif
