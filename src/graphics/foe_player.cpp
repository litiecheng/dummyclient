#include "graphics/foe_player.hpp"

namespace Graphics {

FoePlayer::FoePlayer(
    const ::MapView& mapView,
    const std::string& chipset,
    const std::string& name,
    std::int32_t x,
    std::int32_t y,
    std::uint8_t floor,
    std::uint16_t scaleFactor,
    Direction direction
) : Foe(mapView, chipset, name, 24, 32, x, y, floor, scaleFactor, direction) {}


void FoePlayer::draw(sf::RenderWindow& window) {
    Living::draw(window);
}

void FoePlayer::drawHUD(sf::RenderWindow& window, const sf::View& worldView) {
    // XXX: duplicate with Player::drawHUD class.
    Living::drawHUD(window, worldView);
    const sf::Vector2u& windowSize(window.getSize());
    sf::FloatRect textRect = m_displayName.getLocalBounds();
    const auto screenCoords = window.mapCoordsToPixel(
        m_sprite.getPosition(),
        worldView
    );
    const auto characterOrigin(m_sprite.getOrigin());
    m_displayName.setPosition(
        screenCoords.x + m_w/2.0,
        screenCoords.y
    );
    std::cerr << m_w << ", " << m_h << std::endl;
    m_displayName.setOrigin(
        (textRect.left + textRect.width/2.0) - characterOrigin.x,
        textRect.top - characterOrigin.y
    );

    window.draw(m_displayName);
}

} // namespace Graphics

