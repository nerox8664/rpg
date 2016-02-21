#include "Core/Shader.hpp"

Shader::Shader(std::string vertex, std::string pixel ) {

  const char *pixelSource;
  const char *vertexSource;

  std::string pString, vString;
  size_t pixelSourceLen, vertexSourceLen;

  vert_shader = glCreateShader(GL_VERTEX_SHADER);
  frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

  // загрузка исходников вертексного шейдера
  vString = readFile(vertex);
  vertexSource = vString.c_str();
  vertexSourceLen = vString.size();

  glShaderSource(vert_shader, 1, &vertexSource, (const GLint*)&vertexSourceLen);

  // загрузка исходников фрагментарного шейдера
  pString = readFile(pixel);
  pixelSource = pString.c_str();
  pixelSourceLen = pString.size();

  glShaderSource(frag_shader, 1, &pixelSource, (const GLint*)&pixelSourceLen);

  // компил¤ци¤ шейдеров
  glCompileShader(vert_shader);
  glCompileShader(vert_shader);

  // создание шейдерной программы
  shader_program = glCreateProgram();

  // присоединение шейдеров к программе
  glAttachShader(shader_program, vert_shader);
  glAttachShader(shader_program, frag_shader);

  // сборка шейдерной программы
  glLinkProgram(shader_program);

  // вывод информации о ходе компил¤ции и сборки
  PrintProgramLog(shader_program);
  
  // если были обнаружены ошибки - выходим
  if (glGetError() == GL_NO_ERROR) {
    log_debug("Shader::Shader: Shaders loaded");
  } else {
    shader_program = 0;
    log_warning("Shader::Shader: Shaders NOT loaded");
  }
}

void Shader::SetConst1i(std::string name, int value) {
  const_uniform1i[name] = value;
}

void Shader::SetConst1f(std::string name, float value) {
  const_uniform1f[name] = value;
}

void Shader::SetUniform1i(std::string name, int value) {
  if(shader_program) {
    glUniform1i(
      glGetUniformLocation(shader_program, name.c_str()),
      value
    );
  } else {
    log_warning( "Shader::SetUniform1i: Attempt to set uniform, but shader program not loaded");
  }
}

void Shader::SetUniform1f(std::string name, float value) {
  if(shader_program) {
    glUniform1f(
      glGetUniformLocation(shader_program, name.c_str()),
      value
    );
  } else {
    log_warning( "Shader::SetUniform1f: Attempt to set uniform, but shader program not loaded");
  }
}

Shader::~Shader() {
  if(shader_program) {
    glDeleteProgram(shader_program);
  }
}

void Shader::Bind() {
  glUseProgram(shader_program);

  for(auto i : const_uniform1f) {
    SetUniform1f(i.first, i.second);
  }

  for(auto i : const_uniform1i) {
    SetUniform1i(i.first, i.second);
  }
}

void Shader::Unbind() {
  glUseProgram(0);
}
