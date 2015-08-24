#ifndef __SHADER__
#define __SHADER__

#include "Core/Engine.hpp"
#include "Core/Resource.hpp"

class Shader: public Resource {
protected:
  GLuint vert_shader;
  GLuint frag_shader ;

  GLuint shader_program ;

  std::map<std::string, int> const_uniform1i;
  std::map<std::string, float> const_uniform1f;

public:

  Shader(std::string vertex, std::string pixel);
  ~Shader();

  void SetConst1i(std::string name, int value);
  void SetConst1f(std::string name, float value);

  void SetUniform1i(std::string name, int value);
  void SetUniform1f(std::string name, float value);

  void Bind();
  void Unbind();

};

#endif