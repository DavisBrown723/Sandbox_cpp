#pragma once

#include <string>

#include "intercept.hpp"
#include "entt.hpp"

#include "Core/src/components/Position.h"

namespace sandbox {
    namespace types {

        using WaypointType = intercept::sqf::waypoint::type;
        using WaypointSpeed = intercept::sqf::waypoint::speed;
        using WaypointBehavior = intercept::sqf::waypoint::behaviour;
        using WaypointFormation = intercept::sqf::waypoint::formation;
        using WaypointCombatMode = intercept::sqf::waypoint::combat_mode;

        struct Waypoint {
            components::Position3D position;
            WaypointType type;
            WaypointSpeed speed;
            int completionRadius;
            WaypointBehavior behavior;
            WaypointFormation formation;
            WaypointCombatMode combatMode;
            std::string statements;
            entt::entity attachedVehicle;
            int timeout;
            std::string name;
            std::string description;
        };

    }
}