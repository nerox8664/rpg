#ifndef __NODE_MANAGER__
#define __NODE_MANAGER__

#include "Core/Module.hpp"
#include "Core/Engine.hpp"
#include "Core/Types.hpp"
#include "Core/Node.hpp"

class NodeManager: public Module {
protected:
  std::map<uint64_t, Node*> nodes;
public:

  NodeManager();
  ~NodeManager();

  virtual void OnAttach();
  virtual void OnDetach();

  void AttachNode(Node* node);
  void DetachNode(uint64_t id);

  void Tick(uint64_t time);

private:
};

#endif
