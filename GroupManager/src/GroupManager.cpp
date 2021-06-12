#include "pch.h"

#include "GroupManager.h"

#include "components/Group.h"
#include "components/Unit.h"
#include "Common/src/types/Faction.h"
#include "Common/src/types/ConfigGroup.h"
#include "Common/src/types/UnitLoadout.h"
#include "Common/src/types/VehicleSeats.h"
#include "Common/src/UnitInfo.h"
#include "Core/src/Core.h"
#include "Core/src/components/Position.h"
#include "Core/src/components/EntityBase.h"
#include "Core/src/components/Allegiance.h"
#include "Core/src/components/Speed.h"
#include "Core/src/types/EntityTypes.h"
#include "VirtualSpace/src/Components.h"

using namespace sandbox::components;
using namespace sandbox::types;

namespace sandbox {

    std::vector<entt::entity> GroupManager::createUnitsForGroup(types::ConfigGroup group, entt::entity owningGroup) {
        std::vector<entt::entity> units;
        for (auto& configUnit : group.units) {
            auto entity = Core::EntityRegistry.create();
            Core::EntityRegistry.emplace<Unit>(entity, configUnit.vehicle, owningGroup, intercept::sqf::obj_null(), 0); // #TODO: cache null values in sandbox::Config
            Core::EntityRegistry.emplace<UnitLoadout>(entity, common::getUnitLoadoutDetails(configUnit.vehicle));

            units.push_back(entity);
        }

        return units;
    }

    entt::handle GroupManager::createGroup(Faction* faction, types::ConfigGroup group, types::Vector3 position) {
        auto entity = Core::EntityRegistry.create();
        Core::EntityRegistry.emplace<EntityBase>(entity, EntityType::Group);
        Core::EntityRegistry.emplace<Position3D>(entity, position);
        Core::EntityRegistry.emplace<Spawning>(entity,
            false,
            std::bind(&GroupManager::spawnGroup, this, std::placeholders::_1),
            std::bind(&GroupManager::despawnGroup, this, std::placeholders::_1)
        );

        Core::EntityRegistry.emplace<Allegiance>(entity, faction->side, faction);
        Core::EntityRegistry.emplace<Speed>(entity, 4.3);
        Core::EntityRegistry.emplace<Group>(entity,
            intercept::sqf::grp_null(),
            createUnitsForGroup(group, entity),
            400
        );

        return Core::GetEntityHandle(entity);
    }

    void GroupManager::createVehicle() {

    }

    void GroupManager::spawnGroup(entt::handle entity) {
        intercept::sqf::system_chat("WE SPAWNED BABY");
    }

    void GroupManager::despawnGroup(entt::handle entity) {
        intercept::sqf::system_chat("OK WE DESPAWNED FUCKKKK");
    }



}