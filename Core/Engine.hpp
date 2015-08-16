#ifndef __ENGINE__
#define __ENGINE__


#include "Core/Types.hpp"
#include "Core/Module.hpp"


class Engine {
protected:
  static INIConf*     conf;
  static Log*         log;
  static Engine*      instance;

  std::map<uint16_t, Module*>  modules;

  //Events
  std::map<
    std::pair<Event_t, Event_sub_t>,
    std::map<uint64_t,  std::function<void(Event&)> >
  > mailboxes;
  int eventsPerTick;

  //timers
  std::map<
    uint64_t,
    std::pair<
      SDL_TimerID,
      std::map< uint64_t, std::function<void(Event&)> >
    >
  > timers;
  static unsigned int TimerTick(unsigned int interval, void*);

public:
  Engine(std::string configPath);
  ~Engine();

  static Engine* GetInstance();

  void AttachModule(Module* mod, uint16_t id);
  void DetachModule(uint16_t id);
  Module* GetModule(uint16_t id);

  void ProvideEvent(Event e);
  uint64_t Subscribe( std::pair<Event_t, Event_sub_t> ev, std::function<void(Event&)> act) ;
  void Unsubscribe( std::pair<Event_t, Event_sub_t> ev, uint64_t id) ;
  uint64_t SubscribeToTimer(uint64_t interval, std::function<void(Event&)> act);
  void UnsubscribeFromTimer(uint64_t interval, uint64_t id);

  int MainLoop();
};

#endif
