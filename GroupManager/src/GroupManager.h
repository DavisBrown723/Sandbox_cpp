#pragma once

#include <string>
#include <tuple>

#include "entt.hpp"
#include "intercept.hpp"
#include "signal.hpp"

#include "Common/src/types/ConfigGroup.h"
#include "Common/src/Vector3.h"
#include "Core/src/components/Position.h"

namespace sandbox {
    namespace types {
        struct Faction;
    }

    class GroupManager {
        private:
        
        void createUnitsForGroup( entt::entity owningGroup, types::ConfigGroup group, Faction* faction, const types::Vector3& position );

        sigslot::connection OnKilledEHConnection;

        public:

        GroupManager();
        ~GroupManager();

        inline const static std::string UnitEntityIDVar = "sandbox_entityID";

        entt::entity createUnit(const std::string& unitClass, entt::entity owningGroup );
        entt::handle createGroup(types::Faction* faction, types::ConfigGroup group, const types::Vector3& position);
        entt::handle createVehicle( const std::string& vehicleClass, types::Faction* faction, const types::Vector3& position );

        void spawnGroup(entt::handle entity);
        void despawnGroup(entt::handle entity);

        void spawnVehicle( entt::handle entity );
        void despawnVehicle( entt::handle entity );

        void spawnUnit(entt::entity entity, components::Position3D& pos, intercept::types::group group);
        void despawnUnit(entt::entity entity);

        void groupRemoveUnit(entt::entity groupEntity, entt::entity unitEntity);

        void assignVehicleToGroup( entt::entity group, entt::entity vehicle );
        void assignGroupToVehicleCargo( entt::entity group, entt::entity vehicle );

        void onUnitKilled(intercept::types::object& unit, intercept::types::object& killer);

        void unregisterUnit(entt::entity entity);
        void unregisterGroup(entt::entity entity);

    };

}