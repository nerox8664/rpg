#include "Core/Engine.hpp"
#include "Core/Modules/LuaCore.hpp"
#include "Core/Modules/Renderer.hpp"

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
  e->MainLoop();

  delete e;

  return 0;
}
