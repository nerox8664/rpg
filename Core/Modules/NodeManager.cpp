#include "Core/Modules/NodeManager.hpp"


NodeManager::NodeManager() : Module() {
	log_debug("NodeManager::NodeManager: NodeManager created");
}

NodeManager::~NodeManager() {
}

void NodeManager::OnAttach() {
	log_debug("NodeManager::NodeManager: NodeManager attached");
}

void NodeManager::OnDetach() {
}

void NodeManager::AttachNode(Node* node) {
	uint64_t nodeId = node->GetId();
	if (nodes.find(nodeId) != nodes.end()) {
    log_warning("NodeManager::AttachNode: Node with id = " + std::to_string(nodeId) + " already exists.");
    return;
  } else {
    nodes.insert(std::pair<uint16_t, Node*>(nodeId, node));
    node->OnAttach(nullptr);
  }
}

void NodeManager::DetachNode(uint64_t id) {

}

void NodeManager::Tick(uint64_t time) {
	for(auto i : nodes) {
		i.second->Tick(time);
	}
}