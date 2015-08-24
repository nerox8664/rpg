#include <Core/Modules/Camera.hpp>

Camera::Camera() {
  pos = glm::vec3(0, 0, -1);
}

Camera::~Camera() {

}


void Camera::OnAttach() {
  // Add LUA wrapper for any event types
  using namespace std::placeholders;
  std::function<void(Event&)> processEvent = std::bind(&Camera::ProcessEvent, (Camera*)this, _1);
  Engine::GetInstance()->Subscribe(
    std::pair<Event_t, Event_sub_t>(EVENT_TYPE_GENERAL, EVENT_GENERAL_BEFORE_SCENE_RENDER),
    processEvent
  );

  Engine::GetInstance()->Subscribe(
    std::pair<Event_t, Event_sub_t>(EVENT_TYPE_KEYBOARD, EVENT_KEYBOARD_KEYDOWN),
    [this](Event& e) { 
      if (e.data["keyboard.key"]._<char>() == 'q') {
        pos.z-=0.01;
      };
      if (e.data["keyboard.key"]._<char>() == 'e') {
        pos.z+=0.01;
      };
      if (e.data["keyboard.key"]._<char>() == 'w') {
        pos.y+=0.01;
      };
      if (e.data["keyboard.key"]._<char>() == 's') {
        pos.y-=0.01;
      };
    }
  );

}

void Camera::ProcessEvent(Event& e) {
  float visibleH = ini_int("Video", "windowHeight") * fabs(pos.z);
  float visibleW = ini_int("Video", "windowWidth") * fabs(pos.z);

  gluOrtho2D(
    -(GLdouble)visibleW/2,
    (GLdouble)visibleW/2,
    -(GLdouble)visibleH/2,
    (GLdouble)visibleH/2
  );

  glTranslatef(
    -pos.x,
    -pos.y,
    0
  );
}

void Camera::OnDetach() {

}

void Camera::Tick(uint64_t time) {
  
}