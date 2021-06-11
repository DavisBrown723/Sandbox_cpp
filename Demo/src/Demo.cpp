#include "pch.h"
#include "Demo.h"

#include "Core/src/Core.h"
#include "Common/src/Config.h"

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
            if (intercept::sqf::time() > 15 && !spawned) {
                auto faction = Config::getFaction("BLU_F");
                Core::GetGroupManager().createGroup(faction, faction->groupCategories["Infantry"].groups["BUS_InfAssault"], { 500, 500, 0 });
                spawned = true;
            }
        }

        void Demo::OnMissionEnded() {

        }

    }
}