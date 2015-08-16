#include "Core/Components/ModelComponent.hpp"


ModelComponent::ModelComponent( ) {
}

void ModelComponent::OnAttach(Node* node) {
  parent = node;

  if (!parent->ExistsVar("angle")) {
    parent->SetVar("angle", (float) 0.0);
    log_debug("ModelComponent::OnAttach: angle is null. Set it.");
  }
  if (!parent->ExistsVar("position")) {
    parent->SetVar("position", glm::vec2(0,0));
    log_debug("ModelComponent::OnAttach: position is null. Set it.");
  }
  if (!parent->ExistsVar("size")) {
    parent->SetVar("size", glm::vec2(0,0));
    log_debug("ModelComponent::OnAttach: size is null. Set it.");
  }

  using namespace std::placeholders;
  std::function<void(Event &e)> drawEvent = std::bind(&ModelComponent::Draw, (ModelComponent*)this, _1);
  Engine::GetInstance()->Subscribe(
    std::pair<Event_t, Event_sub_t>(EVENT_TYPE_GENERAL, EVENT_GENERAL_SCENE_RENDER),
    drawEvent
  );

  log_debug("ModelComponent::OnAttach attached");
}

void ModelComponent::OnDetach(Node*) {
}

void ModelComponent::Draw(Event &e) {
  glPushMatrix();
  glTranslatef(pos.x, pos.y, 0);

  glColor4ub(255,0,0, 255);
  glRotatef(angle, 0, 0 , 1);

  glBegin( GL_TRIANGLE_STRIP);
    glVertex2f(0, 0);
    glVertex2f(size.x, 0);
    glVertex2f(0, size.y);
    glVertex2f(size.x, size.y);
  glEnd();
  glPopMatrix();
}

void ModelComponent::Tick(uint64_t time) {
  angle += 0.1;
  pos = parent->GetVar( "position" )._<glm::vec2>();
  size = parent->GetVar( "size" )._<glm::vec2>();
}