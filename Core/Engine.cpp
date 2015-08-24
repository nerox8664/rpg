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

  // SDL additional modules

  // SDL_net
  if (SDLNet_Init() == -1) {
    log_critical("SDLNet_Init Error");
    exit(-1);
  }

  log_debug("SDL_net initialization success");
  // SDL_net

  // SDL_image
  int flags = IMG_INIT_JPG | IMG_INIT_PNG;
  int initted = IMG_Init(flags);
  if(initted&flags != flags) {
    log_critical("IMG_Init Error");
    exit(-1);
  }
  // SDL_image

  instance = this;
}

Engine::~Engine() {
  IMG_Quit();
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

Module* Engine::GetModule(uint16_t id) {
  return modules[id];
}

uint64_t Engine::Subscribe( std::pair<Event_t, Event_sub_t> ev, std::function<void(Event&)> act) {
  static uint64_t evLastId = 0;
  mailboxes[ev][evLastId++]=act;
  return evLastId - 1;
};

void Engine::Unsubscribe(std::pair<Event_t, Event_sub_t> ev, uint64_t id) {
  mailboxes[ev].erase(id);
}

uint64_t Engine::SubscribeToTimer(uint64_t interval, std::function<void(Event&)> act) {
  static uint64_t timerLastId = 1;
  static uint64_t cbLastId = 0;

  if (timers.find(interval) == timers.end()) {
    timers[interval].first = SDL_AddTimer( interval, &Engine::TimerTick, (void*)timerLastId++);
  }

  timers[interval].second[cbLastId++] = act;
  return cbLastId - 1;
}

void Engine::UnsubscribeFromTimer(uint64_t interval, uint64_t id) {
  timers[interval].second.erase(id);
  if (timers[interval].second.size() == 0) {
    SDL_RemoveTimer(timers[interval].first);
  }
}

unsigned int Engine::TimerTick(unsigned int interval, void* data) {
  Event ev;
  ev.type = EVENT_TYPE_TIMER;
  ev.subtype = EVENT_TIMER_TIMEOUT;
  ev.data["timer.interval"] = interval;
  ev.data["timer.timerId"] = (uint64_t)data;
  instance->ProvideEvent(ev);

  for(auto i : instance->timers[interval].second ) {
    i.second(ev);
  }

  return interval;
}

void Engine::ProvideEvent(Event e) {
  eventsPerTick++;
  for (auto i : mailboxes[std::pair<Event_t, Event_sub_t>(e.type, e.subtype)]) {
    i.second(e);
  }
  if (e.subtype != EVENT_ANY_ANY) {
    // For all observers in category
    for (auto i : mailboxes[std::pair<Event_t, Event_sub_t>(e.type, EVENT_ANY_ANY)]) {
      i.second(e);
    }
  }
  if (e.type != EVENT_ANY) {
    // For global observers
    for (auto i : mailboxes[std::pair<Event_t, Event_sub_t>(EVENT_ANY, EVENT_ANY_ANY)]) {
      i.second(e);
    }
  }
}

int Engine::MainLoop() {
  #ifdef DEBUG
    log_debug("Run main loop");

    Subscribe(
    std::pair<Event_t, Event_sub_t>(EVENT_TYPE_KEYBOARD, EVENT_KEYBOARD_KEYDOWN),
    [this](Event& e) { 
      if (e.data["keyboard.key"]._<char>() == 's') {
        log_debug("Events :" + std::to_string(eventsPerTick));
      };
    });
  #endif

  if (modules.size() == 0) {
    log_warning("Modules not found. Nothing to do.");
    exit(0);
  }

  SDL_StartTextInput(); 

  SDL_Event SDLevent;
  bool quit = false;

  while(!quit) {
    eventsPerTick = 0;
    
    while(SDL_PollEvent(&SDLevent)) {
      Event sdlEvent;

      switch (SDLevent.type) {

        case SDL_QUIT:
          sdlEvent.type = EVENT_TYPE_GENERAL;
          sdlEvent.subtype = EVENT_GENERAL_EXIT;
          quit = true;
        break;

        case SDL_KEYUP:
          sdlEvent.type = EVENT_TYPE_KEYBOARD;
          sdlEvent.subtype = EVENT_KEYBOARD_KEYUP;
          sdlEvent.data["keyboard.key"] = SDLevent.key.keysym.sym;
        break;

        case SDL_KEYDOWN:
          sdlEvent.type = EVENT_TYPE_KEYBOARD;
          sdlEvent.subtype = EVENT_KEYBOARD_KEYDOWN;
          sdlEvent.data["keyboard.key"] = SDLevent.key.keysym.sym;
        break;

        case SDL_TEXTINPUT :
          sdlEvent.type = EVENT_TYPE_KEYBOARD;
          sdlEvent.subtype = EVENT_KEYBOARD_TEXT;
          sdlEvent.data["text.text"] = SDLevent.text.text;
        break;

        case SDL_MOUSEMOTION:
          sdlEvent.type = EVENT_TYPE_MOUSE;
          sdlEvent.subtype = EVENT_MOUSE_MOTION;
          sdlEvent.data["motion.x"] = SDLevent.motion.x;
          sdlEvent.data["motion.y"] = SDLevent.motion.y;
        break;

        case SDL_MOUSEBUTTONDOWN:
          sdlEvent.type = EVENT_TYPE_MOUSE;
          sdlEvent.subtype = EVENT_MOUSE_DOWN;
          sdlEvent.data["button.button"] = (uint8_t)SDLevent.button.button;
        break;
      }

      ProvideEvent(sdlEvent);
    }
    
    Event beforeTick;
    beforeTick.type = EVENT_TYPE_GENERAL;
    beforeTick.subtype = EVENT_GENERAL_BEFORE_TICK;
    ProvideEvent(beforeTick);

    for(auto i: modules) {
      i.second->Tick(SDL_GetTicks());
    }

    Event afterTick;
    afterTick.type = EVENT_TYPE_GENERAL;
    afterTick.subtype = EVENT_GENERAL_AFTER_TICK;
    ProvideEvent(afterTick);
  }

  return 0;
}