#ifndef __GUI_ENVIRONMENT__
#define __GUI_ENVIRONMENT__

#include "Core/Module.hpp"
#include "Core/Engine.hpp"
#include "Core/Types.hpp"

class GuiEnvironment: public Module {
protected:
	
public:

  GuiEnvironment();
  ~GuiEnvironment();

  virtual void OnAttach() ;
  virtual void OnDetach() ;

  void Tick(uint64_t time) ;

};

#endif
