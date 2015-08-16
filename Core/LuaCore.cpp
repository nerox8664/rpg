#include "Core/LuaCore.hpp"

lua_State * LuaCore::lss = nullptr;
LuaCore* LuaCore::instance = nullptr;

LuaCore::LuaCore() : Module() {
  instance = this;

  log_debug("LuaCore::LuaCore(): LuaCore has created");
  lss = luaL_newstate();
  luaL_openlibs( lss );
  if (lss == NULL) {
      log_debug( "LuaCore::LuaCore: Error Initializing lua");
  }

  /*
    luabridge::getGlobalNamespace(lss)

      .beginNamespace("engine")
          .addFunction("subscribe", lua_engine_subscribe)
          .addFunction("unsubscribe", lua_engine_unsubscribe)
          //.addFunction("subscribeToTimer", lua_engine_subscribeToTimer)
         // .addFunction("unsubscribeFromTimer", lua_engine_unsubscribeFromTimer)
      .endNamespace()
  */
  
  if (fileExists(ini_str("Lua", "initScript"))){

    log_debug( "LuaCore::LuaCore: Starting lua ... ");
    luaL_dofile( lss,  ini_str("Lua", "initScript").c_str() );

  }
  else {
    log_warning("LuaCore::LuaCore: initScript is not accessable!");
  }
}

LuaCore::~LuaCore() {
}

void LuaCore::OnAttach() {
  log_debug("LuaCore::OnModuleAttach: LuaCore attached");
}

void LuaCore::OnDetach() {
  log_debug("LuaCore::OnModuleDetach: LuaCore detached");
}

lua_State * LuaCore::GetLSS() {
  return lss;
}

void LuaCore::Tick( uint64_t time) {
  //do nothing
}