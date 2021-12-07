#pragma once

#include <string>
#include <vector>
#include <functional>

#include "entt.hpp"
#include "intercept.hpp"
#include "signal.hpp"

#include "Common/src/types/ConfigGroup.h"
#include "Common/src/types/Vehicle.h"
#include "Common/src/types/VehicleSeats.h"
#include "Common/src/Vector3.h"
#include "VirtualSpace/src/types.h"
#include "Core/src/components/Position.h"
//#include "components/Unit.h"
#include "components/Vehicle.h"

namespace sandbox {
    namespace types {
        struct Faction;
    }

    class GroupManager {
        private:

        struct SortedUnits {
            std::vector<std::string> units;
            std::vector<std::string> vehicles;
        };
        
        void createUnitsForGroup(entt::entity owningGroup, const std::vector<std::string>& unitClasses, types::Faction* faction, const types::Vector3& position);
        void createUnitsForGroup(entt::entity owningGroup, types::ConfigGroup group, types::Faction* faction, const types::Vector3& position );

        std::vector<std::string> getConfigGroupUnitClasses(types::ConfigGroup group);

        sigslot::connection OnKilledEHConnection;

        public:

        using VehicleSeatRef = std::reference_wrapper<types::VehicleSeat>;

        enum EmptyVehicleSeats : bool {
            CargoOnly = true,
            AllSeats = false
        };

        GroupManager();
        ~GroupManager();

        inline const static std::string UnitEntityIDVar = "sandbox_entityID";

        entt::entity createUnit(const std::string& unitClass, entt::entity owningGroup );
        entt::handle createGroup(std::vector<std::string> unitClasses, types::Faction* faction, const types::Vector3& position);
        entt::handle createGroup(types::Faction* faction, types::ConfigGroup group, const types::Vector3& position);
        entt::handle createVehicle( const std::string& vehicleClass, types::Faction* faction, const types::Vector3& position );

        void spawnGroup(entt::handle entity);
        void despawnGroup(entt::handle entity);

        void spawnVehicle( entt::handle entity );
        void despawnVehicle( entt::handle entity );

        void spawnUnit(entt::entity entity, components::Position3D& pos, intercept::types::group group);
        void despawnUnit(entt::entity entity);

        void groupRemoveUnit(entt::entity groupEntity, entt::entity unitEntity);

        std::vector<VehicleSeatRef> getVehicleEmptySeats(entt::entity vehicleEntity, EmptyVehicleSeats cargoOnly = EmptyVehicleSeats::AllSeats);
        std::vector<entt::entity> getUnseatedUnits(entt::entity group);

        bool isGroupInCommandOfVehicle(entt::entity group, entt::entity vehicle);
        bool isGroupInCargoOfVehicle(entt::entity group, entt::entity vehicle);
        bool isVehicleAssignedToGroup(entt::entity group, entt::entity vehicle);

        void assignUnitToSeat(entt::entity unit, components::Vehicle& vehicle, VehicleSeatRef seat, bool isActive);
        void assignVehicleToGroup( entt::entity group, entt::entity vehicle );
        void assignGroupToVehicleCargo( entt::entity group, entt::entity vehicle );

        void unassignUnitFromSeat(entt::entity unit);
        void unassignGroupFromVehicle(entt::entity group, entt::entity vehicle);

        void onUnitKilled(intercept::types::object& unit, intercept::types::object& killer);

        void unregisterUnit(entt::entity entity);
        void unregisterGroup(entt::entity entity);

    };

}