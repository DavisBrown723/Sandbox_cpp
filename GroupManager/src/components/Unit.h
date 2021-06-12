#pragma once

#include <string>

#include "entt.hpp"
#include "intercept.hpp"

#include "SeatAssignment.h"

namespace sandbox {
    namespace components {

        struct Unit {
            std::string classname;
            entt::entity group;
            intercept::types::object unitObject;
            int damage;
        };

    }
}