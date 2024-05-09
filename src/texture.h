#ifndef TEXTURE_H
#define TEXTURE_H

#include <fstream>

class Texture {
public:
  unsigned int ID;
  int width, height, nrChannels;

  Texture(const std::string &path, unsigned int filter, unsigned int wrap);
  void use();
};

#endif
