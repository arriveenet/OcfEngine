#pragma once
#include <string>
#include <functional>
#include "base/GameObject.h"
#include "input/Keyboard.h"

namespace ocf {

class Event;
class Node;

class EventListener : public GameObject {
public:
    enum class Type {
        Unknown,
        Keyboard,
        Mouse
    };

    EventListener();
    virtual ~EventListener();

    bool init(Type type, const std::function<void(Event*)>& callback);

    virtual std::string getListenerId() = 0;

    void setAssociatedNode(Node* pNode) { m_pNode = pNode; }

    Node* getAssociatedNode() const { return m_pNode; }

    Type m_type;
    Node* m_pNode;
    std::function<void(Event*)> m_onEvent;
};

} // namespace ocf 
