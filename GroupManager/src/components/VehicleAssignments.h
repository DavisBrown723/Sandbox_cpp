#pragma once

#include <vector>

#include "entt.hpp"

namespace sandbox {
    namespace components {

        struct VehicleAssignments {
            std::vector<entt::handle> vehiclesInCommandOf;
            std::vector<entt::handle> vehiclesInCargoOf;
        };

    }
}