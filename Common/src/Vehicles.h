#pragma once

#include <string_view>

#include "types/VehicleSeats.h"

namespace sandbox {
    namespace common {

        types::VehicleSeats findVehicleSeats(std::string_view vehicleClass);

    }
}