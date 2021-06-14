#include "pch.h"

#include "types/VehicleSeats.h"
#include "Vehicles.h"

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

        int findVehicleSpeed( const std::string& vehicleClass ) {
            return (int)get_number( config_entry( config_file() ) >> vehicleClass >> "maxSpeed" );
        }

        types::VehicleType findVehicleType( const std::string& vehicleClass ) {
            if (is_kind_of( vehicleClass, "Man" ))
                return VehicleType::Man;
            else if (is_kind_of( vehicleClass, "Ship" ))
                return VehicleType::Ship;
            else if (is_kind_of( vehicleClass, "Helicopter" ))
                return VehicleType::Helicopter;
            else if (is_kind_of( vehicleClass, "Plane" ))
                return VehicleType::Plane;
            else if (is_kind_of( vehicleClass, "StaticWeapon" ))
                return VehicleType::StaticWeapon;

            VehicleType type;
            if (is_kind_of( vehicleClass, "Tank" ))
                type = VehicleType::Tank;
            else if (is_kind_of( vehicleClass, "Armored" ) || is_kind_of( vehicleClass, "Wheeled_APC_F" ))
                type = VehicleType::Armored;
            else if (is_kind_of( vehicleClass, "Truck" ) || is_kind_of( vehicleClass, "Truck_F" ))
                type = VehicleType::Truck;
            else if (is_kind_of( vehicleClass, "Car" ))
                type = VehicleType::Car;

            auto vehicleConfig = config_entry( config_file() ) >> "CfgVehicles" >> vehicleClass;
            int maxElevation = (int)get_number( vehicleConfig >> "Turrets" >> "MainTurret" >> "maxElev" );
            bool hasArtyScanner = get_number( vehicleConfig >> "artilleryScanner" ) > 0;
            if (maxElevation > 65) {
                if (hasArtyScanner)
                    return VehicleType::Artillery;
                else
                    return VehicleType::AntiAir;
            }

            return type;
        }

    }
}