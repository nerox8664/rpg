#ifndef __TEXTURE__
#define __TEXTURE__

#include "Core/Engine.hpp"
#include "Core/Resource.hpp"


class Texture: public Resource {
protected:
  int layer;
  GLuint texture ;

public:
  Texture(std::string image);
  ~Texture();

  void Bind (int layer);
  void Unbind ();

};

#endif