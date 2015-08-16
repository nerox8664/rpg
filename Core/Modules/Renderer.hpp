#ifndef __RENDERER__
#define __RENDERER__

#include "Core/Module.hpp"
#include "Core/Engine.hpp"
#include "Core/Types.hpp"

class Renderer: public Module {
protected:
	SDL_DisplayMode   displayMode;
  SDL_Window*       win;
  SDL_GLContext     context;

  uint64_t fpsTimerId;
  uint32_t fps;
  uint32_t frames;
  
  void FPSTimer(Event &e);

public:

  Renderer();
  ~Renderer();

  virtual void OnAttach() ;
  virtual void OnDetach() ;

  void Tick(uint64_t time) ;

};

#endif
