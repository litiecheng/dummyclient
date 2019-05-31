#include "graphics/living.hpp"
#include "graphics/living_state/standing_state.hpp"

namespace Graphics {

Living::Living(const std::string& chipset,
               const std::string& name,
               std::size_t w,
               std::size_t h,
               std::size_t x,
               std::size_t y,
               Direction direction)
    : Entity(chipset, w, h, x, y),
      m_name(name),
      m_direction(direction),
      m_state(std::make_unique<LivingState::StandingState>(*this))
{}

Living& Living::setPosition(std::uint16_t x, std::uint16_t y) {
    m_x = x;
    m_y = y;
    return *this;
}

Living& Living::setDirection(Direction direction) {
    m_direction = direction;
    return *this;
}

Living& Living::changeState(std::unique_ptr<LivingState::State> state) {
    m_state = std::move(state);
    return *this;
}

void Living::draw(sf::RenderWindow& window) {
    m_state->draw(window);
}

} // namespace Graphics
