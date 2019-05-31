#include <cmath>
#include <iostream>

#include <boost/range/irange.hpp>

#include "client.hpp"
#include "game.hpp"
#include "graphics/living.hpp"
#include "graphics/living_state/standing_state.hpp"
#include "graphics/living_state/walking_state.hpp"
#include "model/playing_model.hpp"

#include "screen/game_screen.hpp"

namespace Screen {

GameScreen::GameScreen(
    ::Game& game,
    ::Client& client,
    std::unique_ptr<::MapView> mapView
)
    : UIScreen(game, client), m_mapView(std::move(mapView)),
      m_camera(m_client.pixelPosition().first + 48,
               m_client.pixelPosition().second + 64),
      m_player(
          m_client,
          m_client.character()->skin(),
          m_client.character()->name(),
          m_client.character()->position().first,
          m_client.character()->position().second,
          m_client.character()->direction()
      ),
      m_isArrowPressed(false),
      m_direction(sf::Keyboard::Unknown)
{
    m_player.setPixelX(m_client.pixelPosition().first);
    m_player.setPixelY(m_client.pixelPosition().second);

    /*
    std::cerr << "Position: " << 
          m_client.character()->position().first << ", " <<
          m_client.character()->position().second << std::endl;
    */
}

void GameScreen::loaded() {
    m_client.ping();
    m_pingClock.restart();
    m_syncLivingsClock.restart();
}

void GameScreen::notify() {
    std::shared_ptr<Model::PlayingModel> model =
        std::dynamic_pointer_cast<Model::PlayingModel>(m_model);

    const std::pair<std::uint16_t, std::uint16_t>& position(
        m_client.pixelPosition()
    );

    if (model != nullptr) {

        // Synchronize local livings with the model
        for (const auto [name, living]: m_livings) {
            if (model->livings().find(name) == std::end(model->livings())) {
                m_livings.erase(name);
            }
        }

        for(const auto [name, living]: model->livings()) {
            if (m_livings.find(name) == std::end(m_livings)) {
                m_livings[name] = std::make_shared<Graphics::Living>(*living);
                m_livings[name]->setPixelPosition(
                    m_livings[name]->x() * 32,
                    m_livings[name]->y() * 32
                );
                std::cerr << "Found " << name << " at " <<
                    m_livings[name]->x() << ", " << m_livings[name]->y()
                    << std::endl;
                living->setPixelPosition(
                    living->x() * 32,
                    living->y() * 32
                );
            } else {
                // If the living already exists, check the position and
                // set the walking state if needed.
                std::cerr << name << " is at " << living->x() << ", "
                    << living->y() << std::endl;

                if (m_livings[name]->x() != living->x() ||
                    m_livings[name]->y() != living->y())
                {
                    std::cerr << "Need to move!" << std::endl;
                    // Need to move.
                    // Update the model pixel pos.
                    living->setPixelPosition(
                        living->x() * 32,
                        living->y() * 32
                    );
                    
                }
                // Animate the charater.
                m_livings[name]->moveTowards(
                    living->pixelX(), living->pixelY()
                );
            }
        }
    }
}

void GameScreen::handleCustomEvent(const ::CustomEvent& event) {
    switch(event.type()) {
    case CustomEvent::Type::MovementActive:
        m_player.changeState(
            std::make_shared<Graphics::LivingState::WalkingState>(m_player)
        );
        break;
    case CustomEvent::Type::MovementInactive:
        m_player.changeState(
            std::make_shared<Graphics::LivingState::StandingState>(m_player)
        );
        break;
    default:
        break;
    }

}

void GameScreen::handleEvent(const sf::Event& event)
{
    switch(event.type) {
    case sf::Event::KeyPressed:
        //_onKeyPressed(event);
        break;
    case sf::Event::KeyReleased:
        //_onKeyReleased(event);
        break;
    default:
        break;
    }
}

void GameScreen::_onArrowReleased() {
    if (m_isArrowPressed) {
        pushEvent(
            CustomEvent(
                reinterpret_cast<void*>(shared_from_this().get()),
                CustomEvent::MovementInactive,
                reinterpret_cast<void*>(shared_from_this().get())
            )
        );
        m_isArrowPressed = false;
    }
}

void GameScreen::_onArrowPressed() {
    if (!m_isArrowPressed) {
        pushEvent(
            CustomEvent(
                reinterpret_cast<void*>(shared_from_this().get()),
                CustomEvent::MovementActive,
                reinterpret_cast<void*>(shared_from_this().get())
            )
        );
        m_isArrowPressed = true;
    }
}

void GameScreen::_moveCharacter(sf::Keyboard::Key key) {
    switch(key) {
    case sf::Keyboard::Up:
        m_player.setDirection(Dummy::Core::Character::Direction::UP);
        m_client.moveUp(*m_mapView);
        break;
    case sf::Keyboard::Right:
        m_player.setDirection(Dummy::Core::Character::Direction::RIGHT);
        m_client.moveRight(*m_mapView);
        break;
    case sf::Keyboard::Down:
        m_player.setDirection(Dummy::Core::Character::Direction::DOWN);
        m_client.moveDown(*m_mapView); 
        break;
    case sf::Keyboard::Left:
        m_player.setDirection(Dummy::Core::Character::Direction::LEFT);
        m_client.moveLeft(*m_mapView);
        break;
    default:
        break;
    }
    m_camera.setCenter(m_client.pixelPosition().first + 48,
                       m_client.pixelPosition().second + 64);
}

void GameScreen::_onKeyPressed(const sf::Event& event) {
}

void GameScreen::_onKeyReleased(const sf::Event& event) {
    switch(event.key.code) {
    case sf::Keyboard::Up:
    case sf::Keyboard::Right:
    case sf::Keyboard::Down:
    case sf::Keyboard::Left:
        _onArrowReleased();
        break;
    default:
        break;
    }

}

void GameScreen::_drawLayer(::Sprites& sprites) {
    const sf::Vector2u& windowSize(
        m_game.window().getSize()
    );
    std::int16_t x(static_cast<int>(m_camera.centerX()) / 64),
                 y(static_cast<int>(m_camera.centerY()) / 64);
    std::int16_t xStart(std::max(0, static_cast<int>(x - 12))),
                xEnd(std::min(static_cast<uint16_t>(x + 12),
                              m_mapView->width())),
                yStart(std::max(0, static_cast<int>(y - 9))),
                yEnd(std::min(static_cast<uint16_t>(y + 9),
                              m_mapView->height()));
    for(const auto x: boost::irange(xStart, xEnd)) {
        for (const auto y: boost::irange(yStart, yEnd)) {
            std::size_t index = y * m_mapView->width() + x;
            sf::Sprite& sprite = sprites.at(index);
            sprite.setScale(4, 4);
            int windowX = ((x * 64)) + 16 +
                ((windowSize.x / 2) - static_cast<int>(m_camera.centerX()));
            int windowY = ((y * 64)) + 96 +
                ((windowSize.y / 2) - static_cast<int>(m_camera.centerY()));
            sprite.setPosition(sf::Vector2f(windowX, windowY));
            m_game.window().draw(sprite);
        }
    }
}

void GameScreen::_drawCharacter() {
    //m_player.setPixelPosition(m_originX, m_originY);
    /*
    std::cerr << "server pos: " << m_client.serverPosition().first << ", " <<
        m_client.serverPosition().second << std::endl;
    std::cerr << "pixel pos: " << m_client.pixelPosition().first << ", " <<
        m_client.pixelPosition().second << std::endl;
    */
    m_player.setPixelPosition(m_client.pixelPosition());
    m_player.draw(m_game.window(), m_camera);
}

void GameScreen::_drawLivings() {
    const std::pair<std::uint16_t, std::uint16_t>& position(
        m_client.pixelPosition()
    );
    for (auto [name, living]: m_livings) {
        /*
        std::cerr << "Draw " << name << " at " <<
            living->pixelX() << "," <<
            living->pixelY() << std::endl;
        */
        living->draw(m_game.window(), m_camera);
    }
}

void GameScreen::draw() {
    // Draw the map
    _drawLayer(m_mapView->firstLayerSprites());
    _drawLayer(m_mapView->secondLayerSprites());
    _drawLivings();
    _drawCharacter();
    _drawLayer(m_mapView->thirdLayerSprites());
    _drawLayer(m_mapView->fourthLayerSprites());
    // Draw widgets (HUD) if needed.
    UIScreen::draw();
}

void GameScreen::_syncLivings() {

    const std::pair<std::uint16_t, std::uint16_t>& position(
        m_client.pixelPosition()
    );
    std::shared_ptr<Model::PlayingModel> model =
        std::dynamic_pointer_cast<Model::PlayingModel>(m_model);
    for(auto [name, living]: m_livings) {
        std::cerr << "Check for " << name << std::endl;
        if (model->livings().find(name) != std::end(model->livings())) {
            std::cerr << "Living to update. " << std::endl;
            std::shared_ptr<const Graphics::Living> modelLiving =
                model->getLiving(name);

            int dstPixelX = modelLiving->pixelX();
            int dstPixelY = modelLiving->pixelY();

            /*
            std::cerr << "living->pixelX() " << living->pixelX() << std::endl;
            std::cerr << "dstPixelX " << dstPixelX << std::endl;
            */
            // Update pixel position.
            if (living->pixelX() != dstPixelX) {
                living->setPixelX(
                    living->pixelX() + 2 * (living->pixelX() < dstPixelX) - 1
                );
            }
            if (living->pixelY() != dstPixelY) {
                living->setPixelY(
                    living->pixelY() + 2 * (living->pixelY() < dstPixelY) - 1
                );
            }

            // If the pixel positions are equal, sync server positions.
            if (living->pixelX() == dstPixelX &&
                living->pixelY() == dstPixelY)
            {
                // Sync server position.
                std::cerr << "sync server position" << std::endl;
                living->setX(modelLiving->x());
                living->setY(modelLiving->y());
            }
        }
    }
}

void GameScreen::tick() {

    if (m_pingClock.getElapsedTime().asMilliseconds() >= 300) {
        m_client.ping();
        m_pingClock.restart();
    }

    if (m_syncLivingsClock.getElapsedTime().asMicroseconds() >= 1700) {
        _syncLivings();
        m_syncLivingsClock.restart();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        m_direction = sf::Keyboard::Up;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        m_direction = sf::Keyboard::Right;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        m_direction = sf::Keyboard::Down;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        m_direction = sf::Keyboard::Left;
    } else {
        m_direction = sf::Keyboard::Unknown;
    }

    if (m_direction == sf::Keyboard::Unknown) {
        if (m_isArrowPressed) {
            _onArrowReleased();
        }
        return;
    }

    if (!m_isArrowPressed && m_direction != sf::Keyboard::Unknown) {
        _onArrowPressed();
        m_tickMove.restart();
    } else {
        if (m_tickMove.getElapsedTime().asMicroseconds() >= 1700) {
            _moveCharacter(m_direction);
            m_tickMove.restart();
        }
    }
}

} // namespace Screen
