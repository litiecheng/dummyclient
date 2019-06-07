#pragma once

#include <SFML/Network.hpp>
#include "connector/connector.hpp"

namespace Dummy {
namespace Server {
class GameSession;

namespace Response {
class Response;
} // namespace Response
} // namespace Server
} // namespace Dummy

namespace Connector {

class NetworkConnector : public Connector {
public:
    NetworkConnector(const std::string&, unsigned short);
    virtual void sendCommand(const Dummy::Server::Command::Command&) override;

    virtual std::unique_ptr<const Dummy::Server::Response::Response>
    getResponse() override;

    void connect();
private:
    std::string m_host;
    unsigned short m_port;
    sf::TcpSocket m_socket;
    std::uint16_t m_packetSize;
};

} // namespace Connector