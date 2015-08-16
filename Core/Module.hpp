#ifndef __MODULE__
#define __MODULE__

class Module;
class Engine;

#include "Core/Types.hpp"

class Module {
protected:

public:

  Module() {};
  virtual ~Module() {};

  virtual void OnAttach() {};
  virtual void OnDetach() {};

  virtual void Tick(uint64_t time) = 0;

};

#endif
