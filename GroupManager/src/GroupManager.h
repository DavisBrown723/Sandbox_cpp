#pragma once

#include "entt.hpp"

#include "Common/src/types/ConfigGroup.h"
#include "Common/src/Vector3.h"

namespace sandbox {
    namespace types {
        struct Faction;
    }

    class GroupManager {
        private:
        
        std::vector<entt::entity> createUnitsForGroup(types::ConfigGroup group, entt::entity owningGroup);

        public:

        entt::handle createGroup(types::Faction* faction, types::ConfigGroup group, types::Vector3 position);
        void createVehicle();

        void spawnGroup(entt::handle entity);
        void despawnGroup(entt::handle entity);

    };

}