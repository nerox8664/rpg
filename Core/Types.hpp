#ifndef __TYPES__
#define __TYPES__

//Modules id's
#define MODULE_RENDERER 0
#define MODULE_LUACORE 8


#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

extern "C" {
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}

#include <LuaBridge/LuaBridge.h>

#include <GL/glew.h>

#include <Box2D/Box2D.h>
#include <glm/vec2.hpp>

#include <map>
#include <vector>
#include <chrono>
#include <cmath>
#include <ctime>
#include <functional>
#include <iostream>
#include <cstdint>

#include <Core/Any.hpp>
#include <Core/Log.hpp>
#include <Core/INIConf.hpp>
#include <Core/Utils.hpp>

enum Event_t {
  EVENT_TYPE_GENERAL,
  EVENT_TYPE_TIMER,
  EVENT_TYPE_MOUSE,
  EVENT_TYPE_KEYBOARD,
  EVENT_TYPE_WINDOW,
  EVENT_TYPE_GUI,
  EVENT_TYPE_AUDIO,
  EVENT_TYPE_PHYSICS,
  EVENT_TYPE_NETWORK,
  EVENT_TYPE_USER_TYPE,
  EVENT_ANY
};

enum Event_sub_t {
  EVENT_GENERAL_BEFORE_RENDER = 1,
  EVENT_GENERAL_AFTER_RENDER = 2,

  EVENT_TIMER_TIMEOUT = 10,

  EVENT_MOUSE_MOVE = 20,
  EVENT_MOUSE_CLICK = 21,
  EVENT_MOUSE_WHEEL = 22,

  EVENT_KEYBOARD_KEYDOWN = 30,
  EVENT_KEYBOARD_KEYUP = 31,
  EVENT_KEYBOARD_TEXT = 32,

  EVENT_WINDOW_MOVED = 40,
  EVENT_WINDOW_RESIZED = 41,

  EVENT_GUI_CLICK = 50,
  EVENT_GUI_MOVE = 51,

  EVENT_AUDIO_PLAY = 62,

  EVENT_PHYSICS_COLLISION = 70,

  EVENT_NETWORK_AVAILABLE = 80,

  EVENT_ANY_ANY = 100
};

struct Event {
  Event_t type;
  Event_sub_t subtype;
  std::map<std::string, Any> data;
};

bool operator==(const Event_t& x, const Event_t& y) ;
bool operator==(const Event_sub_t& x, const Event_sub_t& y) ;

typedef std::pair<Event_t, Event_sub_t> ev_t;

bool isPartOf(Event& part, Event& base);


#endif