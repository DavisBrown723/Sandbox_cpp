#include "pch.h"
#include "Demo.h"

#include "Core/src/Core.h"
#include "Common/src/Config.h"
#include "Core/src/components/EntityBase.h"
#include "GroupManager/src/components/Group.h"

using namespace sandbox::types;

namespace sandbox {
    namespace __internal {

        Demo::Demo() {

        }

        void Demo::OnPreInit() {

        }

        void Demo::OnPostInit() {

        }

        void Demo::OnFrame(double) {
            static bool spawned = false;
            static entt::handle group;
            if (intercept::sqf::time() > 15) {
                if (!spawned) {
                    auto faction = Config::getFaction("BLU_F");
                    auto group = Core::GetGroupManager().createGroup(faction, faction->groupCategories["Infantry"].groups["BUS_InfTeam_AA"], { 0, 0, 0 });
                    spawned = true;
                } else {
                    auto coreEntity = Core::EntityRegistry.get<components::EntityBase>(group);
                    //coreEntity.type == components::Group
                }
            }
        }

        void Demo::OnMissionEnded() {

        }

    }
}