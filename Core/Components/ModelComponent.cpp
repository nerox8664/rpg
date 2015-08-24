#include "Core/Components/ModelComponent.hpp"


ModelComponent::ModelComponent( ):shader(nullptr) {

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

  pos = parent->GetVar( "position" )._<glm::vec2>();
  size = parent->GetVar( "size" )._<glm::vec2>();

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

void ModelComponent::AttachShader(Shader* shader) {
  this->shader = shader;
}

void ModelComponent::AttachTexture(Texture* texture, int layer) {
  this->textures[layer] = texture;
}

void ModelComponent::Draw(Event &e) {
  glPushMatrix();
  glTranslatef(pos.x, pos.y, 0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
 

  for (auto i : textures) {
    i.second->Bind(i.first);
  }

  if (shader) {
    shader->Bind();
  }

  glRotatef(angle, 0, 0 , 1);

  
  GLdouble vertices[] = {
    0, 0, 
    0, size.y,
    size.x, 0, 
    size.x, size.y
  };
  GLdouble texVertices[] = {
    1,1, 
    0, 1, 
    1, 0,
    0, 0
  };
 
  glTexCoordPointer(2, GL_DOUBLE, 0, texVertices);
  glVertexPointer(2, GL_DOUBLE, 0, vertices);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


  if (shader) {
    shader->Unbind();
  }

  for (auto i : textures) {
    i.second->Unbind();
  }

  glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
  glDisableClientState(GL_VERTEX_ARRAY);

  glPopMatrix();
}

void ModelComponent::Tick(uint64_t time) {
  angle += 0.001;
  //pos = parent->GetVar( "position" )._<glm::vec2>();
  //size = parent->GetVar( "size" )._<glm::vec2>();
}