#pragma once

#include <string>

namespace sandbox {
    namespace types {

        enum class VehicleType {
            Man,
            Car,
            Truck,
            Armored,
            Tank,
            Artillery,
            AntiAir,
            Helicopter,
            Plane,
            Ship,
            StaticWeapon,
            Vehicle
        };

        enum EngineState : bool {
            EngineOn = true,
            EngineOff = false
        };

        struct Hitpoint {
            std::string name;
            int damage;
        };

    }
}