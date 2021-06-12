#pragma once

#include <string>
#include <vector>
#include <optional>

#include "TargetingSystem.h"
#include "Magazine.h"

namespace sandbox {
    namespace types {

        struct WeaponDetails {
            std::string classname;
            std::vector<TargetingSystem> targetingSystems;
            std::vector<std::string> compatibleMagazines;
        };

        struct Weapon {
            std::string classname;
            std::optional<Magazine> loadedMagazine;
            inline bool loaded() const { return loadedMagazine.has_value(); }
        };

    }
}