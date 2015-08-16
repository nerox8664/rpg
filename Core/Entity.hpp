#ifndef __ENTITY__
#define __ENTITY__

#include "Core/Types.hpp"

class Entity {
protected:
	std::map<std::string, Any> vars;

public:

	Entity() {};
  virtual ~Entity() {};

  virtual bool ExistsVar(std::string key) {
		if (vars.find(key) == vars.end()) {
			return false;
		}
    return true;
  }

	virtual Any& GetVar(std::string key) {
		if (vars.find(key) == vars.end()) {
			vars[key] = nullptr;
		}
    return vars[key];
  }

  virtual void SetVar(std::string key, Any value) {
    vars[key] = value;
  }

};

#endif
