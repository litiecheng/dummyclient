#include "custom_event_queue.hpp"

CustomEvent::CustomEvent(const void* source, Type type) :
    m_eventSource(source), m_type(type)
{
}

CustomEventQueue::CustomEventQueue()
{

}

void CustomEventQueue::pollEvent(CustomEvent& event) {
    if(m_eventsQueue.size() > 0) {
        event = m_eventsQueue.front();
        m_eventsQueue.pop();
    } else {
        event = CustomEvent(nullptr, CustomEvent::None);
    }
}

void CustomEventQueue::pushEvent(const CustomEvent& event) {
    m_eventsQueue.push(event);
}