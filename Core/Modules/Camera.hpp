#ifndef __CAMERA__
#define __CAMERA__

#include "Core/Module.hpp"
#include "Core/Engine.hpp"
#include "Core/Types.hpp"

class Camera: public Module {
protected:
  glm::vec3 pos;
  float angle;

public:
  Camera();
  ~Camera();

  virtual void OnAttach() ;
  virtual void OnDetach() ;

  void ProcessEvent(Event& e);

  void Tick(uint64_t time) ;

};

#endif
