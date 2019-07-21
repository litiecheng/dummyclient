#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "server/map_state.hpp"

#include "map_view.hpp"
#include "local_floor_state.hpp"

namespace Graphics {
class Living;
} // namespace Graphics

using GraphicLivingsMap =
    std::map<std::string, std::unique_ptr<Graphics::Living>>;
using LocalFloorStates = std::vector<LocalFloorState>;
using GraphicLivingFloors =
    std::map<std::string, std::uint8_t>;

class LocalMapState : public Dummy::Server::MapState {
public:
    LocalMapState(const MapView&);
    void visitMapUpdate(
        const Dummy::Protocol::MapUpdate::CharacterPosition&
    ) override;

    void visitMapUpdate(
        const Dummy::Protocol::MapUpdate::CharacterOff&
    ) override;

    void visitMapUpdate(
        const Dummy::Protocol::MapUpdate::CharacterOn&
    ) override;

    void tick();

    const LocalFloorStates& localFloorStates() const {
        return m_localFloorStates;
    }

    const LocalFloorState& localFloorState(std::size_t index) const {
        return m_localFloorStates[index];
    }

    int scaleFactor() const {
        return m_mapView.scaleFactor();
    }

    void setIdleLivings();
    void syncLivings();
    void removeIdleLiving(const std::string&);
private:
    const MapView& m_mapView;
    LocalFloorStates m_localFloorStates;
    GraphicLivingFloors m_graphicLivingsFloor;
};
