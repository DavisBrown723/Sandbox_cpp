#pragma once

#include "ConfigGroup.h"

#include <string>
#include <unordered_map>

namespace sandbox {
    namespace types {

        struct Side;
        struct Faction;

        struct ConfigGroupCategory {
            std::string className;
            std::unordered_map<std::string, ConfigGroup> groups;
        };

    }
}