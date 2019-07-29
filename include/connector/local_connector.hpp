#pragma once

#include "connector/connector.hpp"

namespace Dummy {
namespace Server {
class GameSessionCommunicator;

namespace Response {
class Response;
} // namespace Response
} // namespace Server
} // namespace Dummy

namespace Connector {



class LocalConnector : public Connector {
public:
    LocalConnector(std::shared_ptr<Dummy::Server::GameSessionCommunicator>);
    virtual ~LocalConnector();
    void sendCommand(const Dummy::Server::Command::Command&) override;
private:
    std::shared_ptr<Dummy::Server::GameSessionCommunicator>
        m_gameSessionCommunicator;

};

}; // namespace Connector
