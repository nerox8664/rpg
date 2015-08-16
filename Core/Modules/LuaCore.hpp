#ifndef __LUA_CORE__
#define __LUA_CORE__

#include "Core/Module.hpp"
#include "Core/Engine.hpp"
#include "Core/Types.hpp"

class LuaCore: public Module {
protected:
  static lua_State * lss;
  static LuaCore * instance;

  std::map<uint64_t, luabridge::LuaRef> luaModules;
public:

  LuaCore();
  ~LuaCore();

  //! \return pointer to Lua-machine
  lua_State * GetLSS();

  virtual void OnAttach() ;
  virtual void OnDetach() ;

  void Tick(uint64_t time) ;
  void ProcessEvent(Event& e);

private:
	static int luaModuleAttach(luabridge::LuaRef v, uint64_t id);
  static int luaModuleDetach(uint64_t id);
};

#endif
