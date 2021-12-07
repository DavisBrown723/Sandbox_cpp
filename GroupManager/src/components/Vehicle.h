#pragma once

#include <string>

#include "intercept.hpp"

#include "Common/src/types/Vehicle.h"

namespace sandbox {
    namespace components {

        struct Vehicle {
            std::string classname;
            types::VehicleType vehicleType;
            intercept::types::object vehicleObject;

            bool engineOn;
        };

    }
}