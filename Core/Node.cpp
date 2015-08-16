#include "Core/Node.hpp"

Node::Node() {
  static uint64_t last_id = 100;
  id = last_id++;
  vars["id"] = id;
}

Node::~Node() {
  for(auto i: childs) {
    delete i.second;
  }
}

uint64_t Node::GetId() {
  return id;
}

void Node::Attach(Node* child) {

}

void Node::Detach(Node* child) {

}

void Node::OnAttach(Node *parent) {
}

void Node::OnDetach(Node *parent) {
}

void Node::AddComponent(GenericComponent* comp, uint32_t compId) {
  log_debug("Node::AddComponent: Try to adding component to node "+std::to_string(id));
  if(components.find(compId)!=components.end()) {
    log_warning("Node::AddComponent: Component already exists");
    return;
  }
  components[compId] = comp;
  comp->OnAttach(this);
}

void Node::DelComponent(uint32_t comp_id) {
  log_debug("Node::DelComponent: Try to delete node component. Node id="+std::to_string(id) );
  if(components.find(comp_id)==components.end()) {
    log_warning("Node::AddComponent: Component not found.");
    return;
  }
  components[comp_id]->OnDetach(this);
  components.erase(comp_id);
}

GenericComponent* Node::GetComponent(uint32_t id) {
  return components[id];
}

void Node::Tick(uint64_t time) {
  for(auto i: components) {
    i.second->Tick(time);
  }
  for(auto i: childs) {
    i.second->Tick(time);
  }
}