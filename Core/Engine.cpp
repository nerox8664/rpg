#include "Core/Engine.hpp"

Engine*   Engine::instance  = nullptr;
INIConf*  Engine::conf      = nullptr;
Log*      Engine::log       = nullptr;

Engine* Engine::GetInstance() {
  return instance;
}

Engine::Engine(std::string configPath) {
  conf  = new INIConf (configPath);
  log   = new Log (
    conf->getString("Engine", "log"),
    conf->getBool("Engine", "echo"),
    (Log::logLevels)conf->getInt("Engine", "logLevel")
  );

  log_debug("Hello :3");

  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    log_critical( "SDL_Init Error");
    exit(-1);
  }

  log_debug("SDL initialization success");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, conf->getInt("Video", "glMajorVersion"));
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, conf->getInt("Video", "glMinorVersion"));
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  log_debug("SDL GL set attributes success");

  if (SDLNet_Init() == -1) {
    log_critical("SDLNet_Init Error");
    exit(-1);
  }

  log_debug("SDL_net initialization success");

  instance = this;
}

Engine::~Engine() {
}

void Engine::AttachModule(Module* mod, uint16_t id) {
  if (modules.find(id) != modules.end()) {
    log_warning("Engine::AttachModule: Module with id = " + std::to_string(id) + " already exists.");
    return;
  } else {
    modules.insert(std::pair<uint16_t, Module*>(id, mod));
    mod->OnAttach();
  }
}

void Engine::DetachModule(uint16_t id) {
  if (modules.find(id) == modules.end()) {
    log_warning("Engine::DetachModule: Module with id = " + std::to_string(id) + " not found.");
    return;
  } else {
    modules[id]->OnDetach();
    modules.erase(id);
  }
}

uint64_t Engine::Subscribe( std::pair<Event_t, Event_sub_t> ev, std::function<void(Event&)> act) {
  static uint64_t evLastId = 0;
  mailboxes[ev][evLastId++]=act;
  return evLastId - 1;
};

void Engine::Unsubscribe(std::pair<Event_t, Event_sub_t> ev, uint64_t id) {
  mailboxes[ev].erase(id);
}

void Engine::ProvideEvent(Event e) {
  for ( auto i : mailboxes[std::pair<Event_t, Event_sub_t>(e.type, e.subtype)]) {
    i.second(e);
  }
}

int Engine::MainLoop() {
  log_debug("Run main loop");

  if (modules.size() == 0) {
    log_warning("Modules not found. Nothing to do.");
    exit(0);
  }

  SDL_Event SDLevent;
  bool quit = false;

  while(!quit) {

    while(SDL_PollEvent(&SDLevent)) {

      if(SDLevent.type == SDL_QUIT) {
        quit = true;
      }

      if(SDLevent.type == SDL_KEYUP) {
        Event keybEvent;
        keybEvent.type = EVENT_TYPE_KEYBOARD;
        keybEvent.subtype = EVENT_KEYBOARD_KEYUP;
        keybEvent.data["keyboard.key"] = SDL_GetScancodeFromKey(SDLevent.key.keysym.sym);
        ProvideEvent(keybEvent);
      }

      if(SDLevent.type == SDL_KEYDOWN) {
        Event keybEvent;
        keybEvent.type = EVENT_TYPE_KEYBOARD;
        keybEvent.subtype = EVENT_KEYBOARD_KEYDOWN;
        keybEvent.data["keyboard.key"] = SDL_GetScancodeFromKey(SDLevent.key.keysym.sym);
        ProvideEvent(keybEvent);
      }

    }

    Event beforeRender;
    beforeRender.type = EVENT_TYPE_GENERAL;
    beforeRender.subtype = EVENT_GENERAL_BEFORE_RENDER;
    ProvideEvent(beforeRender);

    for(auto i: modules) {
      i.second->Tick(SDL_GetTicks());
    }

    Event afterRender;
    afterRender.type = EVENT_TYPE_GENERAL;
    afterRender.subtype = EVENT_GENERAL_AFTER_RENDER;
    ProvideEvent(afterRender);

    int maxFPS = ini_int("Engine", "maxFPS");
    SDL_Delay( (1000.0) / maxFPS );

  }

  return 0;
}