#include "Core/Engine.hpp"

#include "Core/Modules/LuaCore.hpp"
#include "Core/Modules/NodeManager.hpp"
#include "Core/Modules/Renderer.hpp"

#include "Core/Components/ModelComponent.hpp"

// SDL2 hack
#undef main

int main(int argc, char **argv) {
  std::string configFile;

  if (argc > 1) {
    configFile = argv[1];
  } else {
    configFile = "config.ini";
  }
    
  Engine *e = new Engine(configFile);

  e->AttachModule(new Renderer(), MODULE_RENDERER);
  e->AttachModule(new LuaCore(), MODULE_LUACORE);
  e->AttachModule(new NodeManager(), MODULE_NODEMANAGER);

  for (int i = 0; i < 1000; i++) {
    Node *node1 = new Node();
    node1->SetVar("size", glm::vec2(100,200));
    node1->AddComponent(new ModelComponent(), 3);

    ((NodeManager*)(e->GetModule(MODULE_NODEMANAGER)))->AttachNode(node1);
  }

  e->MainLoop();

  delete e;

  return 0;
}
