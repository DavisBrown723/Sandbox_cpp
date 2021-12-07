#pragma once

#include <string_view>

#include "types/VehicleSeats.h"
#include "types/Vehicle.h"

namespace sandbox {
    namespace common {

        types::VehicleSeats findVehicleSeats(std::string_view vehicleClass);

        double findVehicleSpeed( const std::string& vehicleClass );
        types::VehicleType findVehicleType( const std::string& vehicleClass );

    }
}