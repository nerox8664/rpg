#include "Core/Modules/Renderer.hpp"


Renderer::Renderer() : Module(), frames(0) {
	int request = SDL_GetDesktopDisplayMode(0, &displayMode);
  int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

  if(ini_int("Video", "fullScreen")) {
    flags = flags | SDL_WINDOW_FULLSCREEN;
  }

  win = SDL_CreateWindow (
    ini_str("Video", "windowTitle").c_str(),
    ini_int("Video", "windowPositionX"),
    ini_int("Video", "windowPositionY"),
    ini_int("Video", "windowWidth"),
    ini_int("Video", "windowHeight"),
    flags
  );

  if (win == nullptr) {
    log_critical("SDL_CreateWindow Error");
    exit(-1);
  }

  context = SDL_GL_CreateContext(win);

  if (context == nullptr) {
    log_critical("SDL_GL_CreateContext Error");
    exit(-1);
  }

  SDL_GL_MakeCurrent(win, context);

  log_debug("Set SDL_GL params");

  SDL_GL_SetSwapInterval(0);


  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, ini_int("Video", "multisampleBuffers") );
  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, ini_int("Video", "multisampleSamples") );

  SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );

  log_debug("Try to init glew");
  
  GLenum glew_status = glewInit();

  if (glew_status != 0) {
    log_critical( "GLEW NOT ok ...");
    log_critical(  (char*)glewGetErrorString(glew_status) );
    exit(-1);
  }

  if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) {
    log_debug("GLSL ok");
  } else {
    log_critical( "GLSL NOT ok ...");
    exit(-1);
  }

  glEnable(GL_MULTISAMPLE);
  glShadeModel( GL_SMOOTH );
  IMG_Load("text.img");
  
  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  

  glEnable(GL_TEXTURE_2D);

  log_debug("InitGL ok");
  log_debug("OpenGL information:");
  log_debug( "GL_VERSION:" + std::string( (char*)(glGetString(GL_VERSION))) );
  log_debug( "GL_VENDOR:" + std::string( (char*)(glGetString(GL_VENDOR))) );
  log_debug( "GL_RENDERER:" + std::string( (char*)(glGetString(GL_RENDERER))) );
  //log_debug( "GL_EXTENSIONS:" + std::string( (char*)(glGetString(GL_EXTENSIONS))) );
  log_debug( "GL_SHADING_LANGUAGE_VERSION:" + std::string( (char*)(glGetString(GL_SHADING_LANGUAGE_VERSION))) );
}

Renderer::~Renderer() {

}

void Renderer::FPSTimer(Event &e) {
	log_debug( "Renderer::FPSTimer: FPS = " + std::to_string(frames) );
  fps = frames;
  frames = 0;
}

void Renderer::OnAttach() {
  using namespace std::placeholders;
  std::function<void(Event&)> fpsTimerCB = std::bind(&Renderer::FPSTimer, (Renderer*)this, _1);
  fpsTimerId = Engine::GetInstance()->SubscribeToTimer(1000, fpsTimerCB);
}

void Renderer::OnDetach() {

}

void Renderer::Tick(uint64_t time) {
  Event beforeRender;
  beforeRender.type = EVENT_TYPE_GENERAL;
  beforeRender.subtype = EVENT_GENERAL_BEFORE_RENDER;
  Engine::GetInstance()->ProvideEvent(beforeRender);

  glClear( GL_COLOR_BUFFER_BIT );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glPushMatrix();

  Event beforeSceneRender;
  beforeSceneRender.type = EVENT_TYPE_GENERAL;
  beforeSceneRender.subtype = EVENT_GENERAL_BEFORE_SCENE_RENDER;
  Engine::GetInstance()->ProvideEvent(beforeSceneRender);

  Event sceneRender;
  sceneRender.type = EVENT_TYPE_GENERAL;
  sceneRender.subtype = EVENT_GENERAL_SCENE_RENDER;
  Engine::GetInstance()->ProvideEvent(sceneRender);

  Event afterSceneRender;
  afterSceneRender.type = EVENT_TYPE_GENERAL;
  afterSceneRender.subtype = EVENT_GENERAL_AFTER_SCENE_RENDER;
  Engine::GetInstance()->ProvideEvent(afterSceneRender);

  Event guiRender;
  guiRender.type = EVENT_TYPE_GENERAL;
  guiRender.subtype = EVENT_GENERAL_GUI_RENDER;
  Engine::GetInstance()->ProvideEvent(guiRender);
  
  glPopMatrix();
  glFlush();
  SDL_GL_SwapWindow(win);

  int maxFPS = ini_int("Engine", "maxFPS");
  SDL_Delay( (1000.0) / maxFPS );

  Event afterRender;
  afterRender.type = EVENT_TYPE_GENERAL;
  afterRender.subtype = EVENT_GENERAL_AFTER_RENDER;
  Engine::GetInstance()->ProvideEvent(afterRender);
	frames++;
}