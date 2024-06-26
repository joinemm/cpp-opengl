#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

Camera::Camera(float aspect) { Camera::aspectRatio = aspect; }

void Camera::moveForward(float amount) { pos += front * amount; }

void Camera::moveSideways(float amount) {
  pos += glm::normalize(glm::cross(front, up)) * amount;
}

void Camera::moveUp(float amount) { pos += up * amount; }

void Camera::setPosition(glm::vec3 newPos) { pos = newPos; }

void Camera::readMouse(double xpos, double ypos, float lookSensitivity) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float xOffset = (xpos - lastX) * lookSensitivity;
  // reversed since y-coordinates range from bottom to top
  float yOffset = (lastY - ypos) * lookSensitivity;

  lastX = xpos;
  lastY = ypos;

  yaw += xOffset;
  pitch += yOffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
}

void Camera::changeFov(float amount) {
  vFov += amount;
  if (vFov < fovMin) {
    vFov = fovMin;
  }
  if (vFov > fovMax) {
    vFov = fovMax;
  }
}

glm::mat4 Camera::projection() {
  return glm::perspective(glm::radians(vFov), aspectRatio, 0.1f, 100.0f);
}

glm::mat4 Camera::lookAtView(glm::vec3 target) {
  glm::mat4 view = glm::lookAt(pos, target, up);
  front = glm::normalize(target - pos);
  pitch = glm::degrees(asin(front.y));
  yaw = glm::degrees(-atan2(front.x, front.z)) + 90.0f;
  return view;
}

glm::mat4 Camera::eulerView() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(direction);
  return glm::lookAt(pos, pos + front, up);
}
