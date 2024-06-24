#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class Shader {
public:
  unsigned int ID;

  Shader(const char *vertexPath, const char *fragmentPath);

  void use();
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, glm::mat4 value) const;
  void setVec3(const std::string &name, glm::vec3 value) const;

private:
  unsigned int loadShaderPath(const std::string &path, GLenum shaderType);
};

#endif
