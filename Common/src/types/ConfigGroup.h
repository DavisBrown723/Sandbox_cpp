#pragma once

#include <string>
#include <vector>

#include "Core/src/components/Position.h"

namespace sandbox {
    namespace types {

        struct Side;
        struct Faction;

        struct ConfigGroupUnit {
            std::string vehicle;
            std::string rank;
            components::Position3D positionOffset;
        };

        struct ConfigGroup {
            Faction* faction;
            std::string className;
            std::string displayName;
            std::string icon;
            std::vector<ConfigGroupUnit> units;
            Side* side;
        };

    }
}