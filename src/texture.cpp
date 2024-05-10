#include <GL/glew.h>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "texture.h"

Texture::Texture(const std::string &path, unsigned int filter,
                 unsigned int wrap) {
  // create a texture object
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);

  // set texture wrap and filter parameters for this texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

  // load texture from file
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  std::cout << path << " channels: " << nrChannels << std::endl;

  if (data) {
    GLenum format = 0;
    switch (nrChannels) {
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      std::cerr << "Texture has " << nrChannels
                << " channels which is unhandled case!" << std::endl;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load texture" << std::endl;
  }

  // free the image memory
  stbi_image_free(data);
}

void Texture::use(unsigned int texture_id) {
  glActiveTexture(texture_id);
  glBindTexture(GL_TEXTURE_2D, ID);
}
