#include <cstdlib>

#include "config.hpp"
#include "custom_event_queue.hpp"
#include "game.hpp"
#include "screen/create_character_screen.hpp"
#include "screen/select_character_screen.hpp"
#include "resource_provider.hpp"

Game::Game(const char* account,
           const char* sessionID,
           Connector::Connector& connector,
           const ::Config& config,
           bool isFullscreen,
           std::size_t width,
           std::size_t height,
           std::size_t scaleFactor)
    : m_isFullscreen(isFullscreen),
      m_client(connector, *this, Credentials(account, sessionID)),
      m_window(sf::VideoMode(width, height),
		       "DummyClient",
               sf::Style::Titlebar | sf::Style::Close |
               (m_isFullscreen ?sf::Style::Fullscreen : 0)),
      m_customEventQueue(CustomEventQueue::instance()),
      m_resourceProvider(ResourceProvider::instance()),
      m_config(config),
      m_width(width),
      m_height(height),
      m_scaleFactor(scaleFactor),
      m_isRunning(true)
{
    if (m_isFullscreen) {
        const auto& mode(sf::VideoMode::getFullscreenModes().at(0));
        m_windowWidth = mode.width;
        m_windowHeight = mode.height;
    } else {
        m_windowWidth = m_width;
        m_windowHeight = m_height;
    }

}

int Game::run()
{
    m_client.setScreen(
        std::make_shared<Screen::SelectCharacterScreen>(
            *this, m_client
        )
    );
    sf::Clock clock;
    m_window.setFramerateLimit(Game::FPS);
    while (m_window.isOpen() && m_isRunning)
    {
        sf::Event event;
        m_client.checkResponse();
        m_window.clear();
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();

            //m_client.checkData();
            m_client.screen()->handleEvent(event);
        }

        CustomEvent customEvent;
        m_customEventQueue.pollEvent(customEvent);
        if (customEvent.type() != CustomEvent::None) {
            m_client.screen()->handleCustomEvent(customEvent);
        }

        if (clock.getElapsedTime().asMilliseconds() > 1000/Game::FPS) {
            m_client.screen()->draw(m_window);
            m_window.display();
            clock.restart();
        }
        m_client.screen()->tick();
        sf::sleep(sf::milliseconds(1));
    }

    return EXIT_SUCCESS;
}

void Game::quit() {
    m_isRunning = false;
}
