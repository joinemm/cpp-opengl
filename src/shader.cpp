#include <GL/glew.h>
#include <fstream>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

#include "shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // load vertex and fragment shaders from files
  unsigned int vertex = loadShaderPath(vertexPath, GL_VERTEX_SHADER);
  unsigned int fragment = loadShaderPath(fragmentPath, GL_FRAGMENT_SHADER);

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  // check for linking errors
  int success;
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[1024];
    glGetProgramInfoLog(ID, 1024, NULL, infoLog);
    std::cerr << "ERROR: SHADER LINKING FAILED\n" << infoLog << std::endl;
  }

  // delete the shader objects.
  // we don't need them anymore as they are linked to the program
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1,
               glm::value_ptr(value));
}

unsigned int Shader::loadShaderPath(const std::string &path,
                                    GLenum shaderType) {
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
    char infoLog[1024];
    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
    std::cerr << "ERROR: \"" << path << "\" SHADER COMPILATION FAILED\n"
              << infoLog << std::endl;
  }

  return shader;
}
