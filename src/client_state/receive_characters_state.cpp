#include <iostream>

#include <boost/range/irange.hpp>

#include "client.hpp"
#include "game.hpp"
#include "client_state/manage_characters_state.hpp"
#include "client_state/receive_characters_state.hpp"
#include "model/characters_list_model.hpp"

namespace ClientState {

ReceiveCharactersState::ReceiveCharactersState(::Client& client)
    : State(client)
{
}

void ReceiveCharactersState::resume() {
    // Nothing to do for now.
    // The client will select the character later on.
    m_model = std::make_shared<Model::CharactersListModel>();
    m_client.game().screen()->setModel(m_model);
    m_model->update();
}

void ReceiveCharactersState::onRead(Dummy::Protocol::IncomingPacket& pkt) {
    // Here, we will get the server answer once the character has been either
    // created or selected.
    auto self(shared_from_this());
    std::uint16_t charactersCount;
    pkt >> charactersCount;
    std::cerr << "[ReceiveCharactersState] got " << charactersCount <<
        " characters. " << std::endl;

    for(auto i: boost::irange(charactersCount))
    {
        i = i;
        std::shared_ptr<Dummy::Core::Character> chr =
            std::make_shared<Dummy::Core::Character>();
        pkt >> *chr;
        m_model->addCharacter(chr);
    }
    m_client.changeState(std::make_shared<ManageCharactersState>(m_client));
}

} // namespace ClientState
