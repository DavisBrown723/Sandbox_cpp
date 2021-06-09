#pragma once

#include <vector>

#include "intercept.hpp"
#include "entt.hpp"

namespace sandbox {
    namespace components {

        struct Group {
            intercept::types::group group;
            std::vector<entt::entity> units;
            int sightRange;
        };

    }
}