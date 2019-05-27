#pragma once

#include <exception>
#include <SFML/Graphics.hpp>

#include "custom_event_queue.hpp"
#include "client.hpp"
#include "resource_provider.hpp"

class Game {
public:
    static const int FPS = 60;
    Game(const char*, const char*, std::size_t = 1280, std::size_t = 960);
    int run();
    void setScreen(std::shared_ptr<Screen::Screen> screen);
    sf::RenderWindow& window() {
        return m_window;
    }
    Client& client() {
        return m_client;
    }

    std::shared_ptr<Screen::Screen> screen() {
        return m_currentScreen;
    }

    std::size_t width() const {
        return m_width;
    }

    std::size_t height() const {
        return m_height;
    }
private:
    Client m_client;
    sf::RenderWindow m_window;
    ::CustomEventQueue& m_customEventQueue;
    ::ResourceProvider& m_resourceProvider;
    std::shared_ptr<Screen::Screen> m_currentScreen;
    std::size_t m_width, m_height;
};
