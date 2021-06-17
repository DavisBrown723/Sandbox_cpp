#pragma once

#include <vector>

#include "GroupManager/src/types/Waypoint.h"

namespace sandbox {
    namespace components {

        struct Waypoints {
            std::vector<types::Waypoint> waypoints;
        };

    }
}