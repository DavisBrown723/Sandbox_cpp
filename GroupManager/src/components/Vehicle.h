#pragma once

#include <string>

#include "Common/src/types/Vehicle.h"

namespace sandbox {
    namespace components {

        struct Vehicle {
            std::string classname;
            types::VehicleType vehicleType;

            bool engineOn;
        };

    }
}