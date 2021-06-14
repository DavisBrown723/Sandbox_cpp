#pragma once

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
        
        std::vector<entt::entity> createUnitsForGroup(types::ConfigGroup group, entt::entity owningGroup);

        sigslot::connection OnKilledEHConnection;

        public:

        GroupManager();
        ~GroupManager();

        inline const static std::string UnitEntityIDVar = "sandbox_entityID";

        entt::handle createGroup(types::Faction* faction, types::ConfigGroup group, types::Vector3 position);
        entt::handle createVehicle( const std::string& vehicleClass, types::Faction* faction, types::Vector3 position );

        void spawnGroup(entt::handle entity);
        void despawnGroup(entt::handle entity);

        void spawnVehicle( entt::handle entity );
        void despawnVehicle( entt::handle entity );

        void spawnUnit(entt::entity entity, components::Position3D& pos, intercept::types::group group);
        void despawnUnit(entt::entity entity);

        void groupRemoveUnit(entt::entity groupEntity, entt::entity unitEntity);

        void onUnitKilled(intercept::types::object& unit, intercept::types::object& killer);

        void unregisterUnit(entt::entity entity);
        void unregisterGroup(entt::entity entity);

    };

}