#ifndef __MODEL_COMPONENT__
#define __MODEL_COMPONENT__

#include "Core/GenericComponent.hpp"
#include "Core/Node.hpp"

class ModelComponent: public GenericComponent {
protected:
	float angle;
  glm::vec2 pos;
  glm::vec2 size;
  
public:
  ModelComponent();

  virtual void OnAttach(Node*);
  virtual void OnDetach(Node*);

  virtual void Draw(Event &e);

  virtual void Tick(uint64_t time);
};

#endif
