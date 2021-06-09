#include "pch.h"

#include "GroupManager.h"

#include "components/Group.h"
#include "Common/src/types/Faction.h"
#include "Common/src/types/ConfigGroup.h"
#include "Core/src/Core.h"
#include "Core/src/components/Position.h"
#include "Core/src/components/EntityBase.h"
#include "Core/src/components/Allegiance.h"
#include "Core/src/components/Speed.h"
#include "Core/src/types/EntityTypes.h"
#include "VirtualSpace/src/Components.h"

using namespace sandbox::types;

namespace sandbox {

    std::vector<entt::entity> GroupManager::createUnitsForGroup(types::ConfigGroup group) {
        std::vector<entt::entity> units;
        for (auto& configUnit : group.units) {
            auto entity = Core::EntityRegistry.create();

            units.push_back(entity);
        }

        return units;
    }

    entt::handle GroupManager::createGroup(Faction* faction, types::ConfigGroup group, types::Vector3 position) {
        auto entity = Core::EntityRegistry.create();
        Core::EntityRegistry.emplace<components::EntityBase>(entity, EntityType::Group);
        Core::EntityRegistry.emplace<components::Position3D>(entity, position);
        Core::EntityRegistry.emplace<components::Spawning>(entity,
            false,
            std::bind(&GroupManager::spawnGroup, this, std::placeholders::_1),
            std::bind(&GroupManager::despawnGroup, this, std::placeholders::_1)
        );

        Core::EntityRegistry.emplace<components::Allegiance>(entity, faction->side, faction);
        Core::EntityRegistry.emplace<components::Speed>(entity, 4.3);
        Core::EntityRegistry.emplace<components::Group>(entity,
            intercept::sqf::grp_null(),
            createUnitsForGroup(group),
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