#include <Core/Texture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "Core/stb_image.hpp"

Texture::Texture(std::string filename) {
  int w;
  int h;
  int comp;
  unsigned char* image = stbi_load(filename.c_str(), &w, &h, &comp, STBI_rgb_alpha);

  if(image == nullptr) {
    throw(std::string("Failed to load texture"));
  }

  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  if(comp == 3)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  else if(comp == 4)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(image);
}

Texture::~Texture() {
  glDeleteTextures(1, &texture);
}

void Texture::Bind (int layer) {
  this->layer = layer;
  glActiveTexture(GL_TEXTURE0 + layer);
  glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::Unbind () {
  glActiveTexture(GL_TEXTURE0 + layer);
  glBindTexture(GL_TEXTURE_2D, 0);
}