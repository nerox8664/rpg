#include "Core/Renderer.hpp"


Renderer::Renderer() : Module() {
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
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 0 );

  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, ini_int("Video", "multisampleBuffers") );
  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, ini_int("Video", "multisampleSamples") );

  SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );

  log_debug("Try to init glew");
  glewExperimental = GL_TRUE;
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

  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable( GL_TEXTURE_2D );

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

void Renderer::OnAttach() {
	using namespace std::placeholders;
  std::function<void(Event&)> cb = std::bind(&Renderer::BeforeRenderer, (Renderer*)this, _1);
  Engine::GetInstance()->Subscribe(
    std::pair<Event_t, Event_sub_t>(EVENT_TYPE_GENERAL, EVENT_GENERAL_BEFORE_RENDER),
    cb
  );

  std::function<void(Event&)> cb_after = std::bind(&Renderer::AfterRenderer, (Renderer*)this, _1);
  Engine::GetInstance()->Subscribe(
    std::pair<Event_t, Event_sub_t>(EVENT_TYPE_GENERAL, EVENT_GENERAL_AFTER_RENDER),
    cb_after
  );
  std::cout<<"Renderer attached";
}

void Renderer::OnDetach() {

}

void Renderer::BeforeRenderer(Event &e) {
	glClear( GL_COLOR_BUFFER_BIT );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glPushMatrix();
}

void Renderer::AfterRenderer(Event &e) {
	glPopMatrix();

  glFlush();
  SDL_GL_SwapWindow(win);
}

void Renderer::Tick(uint64_t time) {
	
}