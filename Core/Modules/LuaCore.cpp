#include "Core/Modules/LuaCore.hpp"

lua_State * LuaCore::lss = nullptr;
LuaCore* LuaCore::instance = nullptr;

LuaCore::LuaCore() : Module() {
  instance = this;

  log_debug("LuaCore::LuaCore(): LuaCore has created");
  lss = luaL_newstate();
  luaL_openlibs( lss );

  if (lss == NULL) {
    log_debug("LuaCore::LuaCore: Error Initializing lua");
    exit(-1);
  }

  luabridge::getGlobalNamespace(lss)
    .beginNamespace("log")
      .addFunction("put", log_put)
      .addFunction("debug", log_debug)
      .addFunction("warning", log_warning)
      .addFunction("error", log_error)
      .addFunction("critical", log_critical)
    .endNamespace()
    .beginNamespace("engine")
      .addFunction("attachModule", luaModuleAttach)
      .addFunction("detachModule", luaModuleDetach)
    .endNamespace();

  if (fileExists(ini_str("Lua", "initScript"))){
    log_debug("LuaCore::LuaCore: Starting lua init script ... ");
    luaL_dofile( lss,  ini_str("Lua", "initScript").c_str() );
  } else {
    log_warning("LuaCore::LuaCore: initScript is not accessable!");
  }
}

LuaCore::~LuaCore() {
}

void LuaCore::OnAttach() {
  log_debug("LuaCore::OnModuleAttach: LuaCore attached");

  // Add LUA wrapper for any event types
  using namespace std::placeholders;
  std::function<void(Event&)> processEvent = std::bind(&LuaCore::ProcessEvent, (LuaCore*)this, _1);
  Engine::GetInstance()->Subscribe(
    std::pair<Event_t, Event_sub_t>(EVENT_ANY, EVENT_ANY_ANY),
    processEvent
  );
}

void LuaCore::OnDetach() {
  log_debug("LuaCore::OnModuleDetach: LuaCore detached");
}

lua_State * LuaCore::GetLSS() {
  return lss;
}

void LuaCore::Tick(uint64_t time) {
  for (auto i : luaModules) {
    i.second["Tick"]();
  }
}

void LuaCore::ProcessEvent(Event& e) {
}

int LuaCore::luaModuleAttach(luabridge::LuaRef mod, uint64_t id) {
  if (instance->luaModules.find(id) != instance->luaModules.end()) {
    log_warning("LuaCore::luaModuleAttach: Module with id = " + std::to_string(id) + " already exists.");
    return -1;
  }

  instance->luaModules.insert(std::pair<uint16_t, luabridge::LuaRef>(id, mod));
  mod["OnAttach"]();
  return 0;  
}

int LuaCore::luaModuleDetach(uint64_t id) {
  if (instance->luaModules.find(id) == instance->luaModules.end()) {
    log_warning("LuaCore::luaModuleDetach: Module with id = " + std::to_string(id) + " not found.");
    return -1;
  } 
  luabridge::LuaRef mod = (*(instance->luaModules.find(id))).second;
  mod["OnDetach"]();
  instance->luaModules.erase(id);
}