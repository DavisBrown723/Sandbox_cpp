#pragma once

#include "entt.hpp"

#include "Common/src/types/VehicleSeats.h"

namespace sandbox {
    namespace components {

        struct SeatAssignment {
            entt::entity vehicle;
            types::VehicleSeat seat;
        };

    }
}