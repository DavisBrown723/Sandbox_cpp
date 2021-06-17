#include "pch.h"
#include "WaypointSystem.h"

#include "Core/src/Core.h"
#include "Core/src/components/Position.h"
#include "Core/src/components/Speed.h"
#include "components/Waypoints.h"
#include "types/Waypoint.h"

using namespace intercept;
using namespace sandbox::components;
using namespace sandbox::types;

namespace sandbox {

    void WaypointSystem::OnFrame(double dt) {
        auto view = Core::EntityRegistry.view<Position3D, Speed, Waypoints>();

        for (auto entity : view) {
            auto& waypoints = Core::EntityRegistry.get<Waypoints>(entity);
            if (waypoints.waypoints.empty())
                continue;

            auto& position = Core::EntityRegistry.get<Position3D>(entity);
            auto& speed = Core::EntityRegistry.get<Speed>(entity);

            
        }
    }

}