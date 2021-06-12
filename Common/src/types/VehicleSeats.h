#pragma once

#include <optional>
#include <vector>

#include "entt.hpp"

#include "TurretPath.h"

namespace sandbox {
    namespace types {

        enum class VehicleSeatType {
            Driver,
            Turret,
            Cargo
        };

        struct SeatPath {
            std::optional<TurretPath> turretPath;
            std::optional<int> cargoIndex;
        };

        struct VehicleSeat {
            SeatPath path;
            std::optional<entt::entity> assignedUnit;
            inline VehicleSeatType type() {
                if (path.turretPath) return VehicleSeatType::Turret;
                if (path.cargoIndex == -1) return VehicleSeatType::Driver;
                return VehicleSeatType::Cargo;
            }
        };

        struct VehicleSeats {
            std::optional<VehicleSeat> driver;
            std::vector<VehicleSeat> turrets;
            std::vector<VehicleSeat> cargo;
        };

    }
}