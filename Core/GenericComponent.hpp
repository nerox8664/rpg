#ifndef __GENERIC_COMPONENT__
#define __GENERIC_COMPONENT__

#include "Core/Types.hpp"

class Node;

class GenericComponent {
protected:
  Node *parent;

public:
  virtual void OnAttach(Node*) {};
  virtual void OnDetach(Node*) {};

  virtual void Tick(uint64_t time) = 0;
};

#endif
