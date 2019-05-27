#pragma once

#include <exception>
#include <memory>

#include <SFML/Network.hpp>

#include "core/character.hpp"
#include "protocol/outgoing_packet.hpp"

#include "client_state/initial_state.hpp"
#include "credentials.hpp"
#include "screen/screen.hpp"

class ClientError : public std::exception {

};

class ConnectionError : public ClientError {
public:
    virtual const char* what() const noexcept override {
        return "cannot connect to server";
    }
};

class ReceiveError : public ClientError {
public:
    virtual const char* what() const noexcept override {
        return "cannot receive data from server";
    }
};

class MapView;

class Client {
public:
    Client(::Game&, const Credentials&&);

    const Credentials& credentials() const {
        return m_credentials;
    }

    sf::TcpSocket& socket() {
        return m_socket;
    }

    void checkData();
    void connect(const char* host, unsigned short port);
    void authenticate();
    void changeState(std::shared_ptr<ClientState::State>);
    void update();
    void send(const std::uint8_t*, std::size_t);
    void send(const Dummy::Protocol::OutgoingPacket&);

    ::Game& game() {
        return m_game;
    }

    const std::string& account() const {
        return m_credentials.account();
    }

    std::shared_ptr<Dummy::Core::Character> character() const {
        return m_character;
    }

    const std::pair<std::uint16_t, std::uint16_t> pixelPosition() const {
        return m_pixelPosition;
    }

    void moveLeft(const MapView&);
    void moveUp(const MapView&);
    void moveRight(const MapView&);
    void moveDown(const MapView&);

    void setCharacter(std::shared_ptr<Dummy::Core::Character>);

private:
    ::Game& m_game;
    sf::TcpSocket m_socket;
    std::uint16_t m_packetSize;
    Credentials m_credentials;
    std::shared_ptr<ClientState::State> m_state;
    std::shared_ptr<Dummy::Core::Character> m_character;
    std::pair<std::uint16_t, std::uint16_t> m_pixelPosition;
};
