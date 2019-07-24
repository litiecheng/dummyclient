#include <set>

#include "protocol/map_update/character_off.hpp"
#include "protocol/map_update/character_on.hpp"
#include "protocol/map_update/character_position.hpp"
#include "protocol/living.hpp"
#include "graphics/foe_player.hpp"
#include "local_map_state.hpp"
#include "map_view.hpp"

LocalMapState::LocalMapState(const MapView& mapView)
    : m_mapView(mapView)
{
    // Instantiate floor states.
    for (const auto& floor: m_mapView.levelViews()) {
        m_localFloorStates.push_back(LocalFloorState(*this));
    }
}

void LocalMapState::setIdleLivings() {

    for (auto& floorState: m_localFloorStates) {
        floorState.setIdleLivings();
    }
}

void LocalMapState::visitMapUpdate(
    const Dummy::Protocol::MapUpdate::CharacterPosition& characterPosition
) {
    Dummy::Server::MapState::visitMapUpdate(characterPosition);
    // Get the character's floor.
    const auto& floor(m_graphicFoesFloor[characterPosition.name()]);
    m_localFloorStates[floor].onCharacterPosition(characterPosition);
}


void LocalMapState::visitMapUpdate(
    const Dummy::Protocol::MapUpdate::CharacterOn& characterOn
) {
    Dummy::Server::MapState::visitMapUpdate(characterOn);

    // Check that its floor is valid.
    const auto& floor(characterOn.floor());

    if (floor >= m_mapView.levelViews().size()) {
        // XXX: throw exception?
    }

    auto& localFloorState(m_localFloorStates[floor]);

    if (localFloorState.containsLiving(characterOn.name())) {
        // XXX: throw exception?
    }

    auto foe = std::make_shared<Graphics::FoePlayer>(
        m_mapView,
        characterOn.chipset(),
        characterOn.name(),
        8 * characterOn.x() * m_mapView.scaleFactor(),
        8 * characterOn.y() * m_mapView.scaleFactor(),
        characterOn.floor(),
        m_mapView.scaleFactor(),
        characterOn.direction()
    );

    localFloorState.addFoe(characterOn.name(), foe);
    m_graphicFoesFloor[characterOn.name()] = floor;
}

void LocalMapState::visitMapUpdate(
    const Dummy::Protocol::MapUpdate::CharacterOff& characterOff
) {
    Dummy::Server::MapState::visitMapUpdate(characterOff);

    // Get the appropriate floor and dispatch.
    const auto& floor(m_graphicFoesFloor[characterOff.name()]);
    m_localFloorStates[floor].removeFoe(characterOff.name());
}

void LocalMapState::tick() {
    for (auto& floorState: m_localFloorStates) {
        floorState.tick();
    }
}

void LocalMapState::syncLivings() {
    for (auto& floorState: m_localFloorStates) {
        floorState.syncLivings();
    }
}
