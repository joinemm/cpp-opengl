#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  unsigned int ID;

  Shader(const char *vertexPath, const char *fragmentPath) {
    // load vertex and fragment shaders from files
    unsigned int vertex = loadShaderPath(vertexPath, GL_VERTEX_SHADER);
    unsigned int fragment = loadShaderPath(fragmentPath, GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // check for linking errors
    int success;
    glGetShaderiv(ID, GL_LINK_STATUS, &success);
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

  void use() { glUseProgram(ID); }

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }

  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }

private:
  unsigned int loadShaderPath(const std::string &path, GLenum shaderType) {
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
};

#endif
