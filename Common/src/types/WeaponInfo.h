#pragma once

#include <string>
#include <vector>

#include "types/TargetingSystem.h"

namespace sandbox {
    namespace types {

        struct WeaponInfo {
            std::string classname;
            std::vector<TargetingSystem> targetingSystems;
            std::vector<std::string> compatibleMagazines;
        };

    }
}