#include "visual.hpp"

Visual::Visual()
    : m_resourceProvider(ResourceProvider::instance()),
      m_eventQueue(CustomEventQueue::instance()), m_x(0), m_y(0)
{}

void Visual::setPos(std::uint16_t x, std::uint16_t y) {
    m_x = x;
    m_y = y;
}

void Visual::setSize(std::uint16_t width, std::uint16_t height) {
    m_width = width;
    m_height = height;
}

void Visual::addChild(std::shared_ptr<Visual> child) {
    m_children.emplace(child);
}

void Visual::removeChild(std::shared_ptr<Visual> child) {
    m_children.erase(child);
}
