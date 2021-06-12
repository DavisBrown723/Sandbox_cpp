#include "pch.h"

#include "types/VehicleSeats.h"

using namespace intercept::sqf;
using namespace sandbox::types;

namespace sandbox {
    namespace common {

        VehicleSeats findVehicleSeats(std::string_view vehicleClass) {
            config_entry vehicleConfig = config_entry(config_file()) >> "CfgVehicles" >> vehicleClass;
            if (!is_class(vehicleConfig))
                throw std::invalid_argument("findVehicleSeats(): Vehicle class does not exist - " + std::string(vehicleClass));

            VehicleSeats seats;

            bool hasDriver = get_number(vehicleConfig >> "hasDriver") > 0;
            if (hasDriver) {
                seats.driver = VehicleSeat{ SeatPath{ .cargoIndex = -1 } };
            }

            int cargoCount = (int)get_number(vehicleConfig >> "transportsoldier");
            seats.cargo.reserve(cargoCount);
            for (int i = 0; i < cargoCount; i++)
                seats.cargo.push_back(VehicleSeat{ SeatPath{ .cargoIndex = i } });

            //#todo: lets convert this into C++
            auto turretSeatsReturn = call(
                game_value(vehicleClass),
                compile("[_this,[]] call bis_fnc_getturrets")
            ).to_array();

            seats.turrets.reserve(turretSeatsReturn.size());
            for (auto& seat : turretSeatsReturn)
                seats.turrets.push_back(VehicleSeat{ SeatPath{ .turretPath = rv_turret_path(seat) } });

            return seats;
        }

    }
}