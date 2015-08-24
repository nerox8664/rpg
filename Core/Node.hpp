#ifndef __NODE__
#define __NODE__

#include "Core/GenericComponent.hpp"
#include "Core/Module.hpp"
#include "Core/Engine.hpp"
#include "Core/Types.hpp"
#include "Core/Entity.hpp"


class Node : public Entity {
protected:
  uint64_t id;

  std::map<uint64_t, Node*> childs;
  std::map<uint32_t, GenericComponent*> components;

public:
  Node();
  ~Node();

  virtual void OnAttach(Node*);
  virtual void OnDetach(Node*);

  virtual uint64_t GetId();

  virtual void Attach(Node*);
  virtual void Detach(Node*);

  virtual void AddComponent(GenericComponent*, uint32_t);
  virtual void DelComponent(uint32_t);
  virtual GenericComponent* GetComponent(uint32_t);

  virtual void Tick(uint64_t time);
};

#endif
