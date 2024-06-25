#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <fstream>

class Texture {
public:
  unsigned int ID;
  int width, height, nrChannels;

  Texture(const std::string &path, GLint filter, GLint wrap);
  void use(unsigned int texture_id);
};

#endif
