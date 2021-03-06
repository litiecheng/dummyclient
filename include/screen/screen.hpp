#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include <dummy/server/response/response_visitor.hpp>
#include "game_element.hpp"
#include "visual.hpp"

class CustomEvent;
class Client;
class Game;

namespace Dummy {
namespace Server {
namespace Response {
class Response;
} // namespace Response
} // namespace Server
} // namespace Dummy

namespace Screen {

class Screen : public ::Visual,
               public Dummy::Server::Response::ResponseVisitor
{
public:
    Screen(::Game&, ::Client&);
    virtual ~Screen();
    virtual void tick();
    virtual void loaded();
    virtual void returned();
    sf::IntRect boundingRect() const override;
    virtual void
    onResponse(const Dummy::Server::Response::Response& response) = 0;

    // XXX: Is accessed by some external screen state.
    ::Game& game() {
        return m_game;
    }
protected:
    ::Game& m_game;
    ::Client& m_client;
};

} // namespace Screen
