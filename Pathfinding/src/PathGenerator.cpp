#include "pch.h"
#include "PathGenerator.h"

using namespace intercept;
using namespace sandbox;
using namespace sandbox::types;

namespace sandbox {

    void PathGenerator::PostInit() {
        intercept::types::object manProp = sqf::create_vehicle("C_man_1", { 0, 0, 0 }, {}, 0.f, "NONE");
        intercept::types::object motorizedProp = sqf::create_vehicle("C_Offroad_01_F", { 0, 0, 0 }, {}, 0.f, "NONE");
        intercept::types::object mechanizedProp = sqf::create_vehicle("B_APC_Tracked_01_rcws_F", { 0, 0, 0 }, {}, 0.f, "NONE");
        intercept::types::object armoredProp = sqf::create_vehicle("B_MBT_01_cannon_F", { 0, 0, 0 }, {}, 0.f, "NONE");
        intercept::types::object boatProp = sqf::create_vehicle("C_Rubberboat", { 0, 0, 0 }, {}, 0.f, "NONE");

        m_vehicleTypeObjects.emplace(types::VehicleType::Man, manProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Car, motorizedProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Truck, motorizedProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Armored, mechanizedProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::AntiAir, mechanizedProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Tank, armoredProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Artillery, armoredProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Ship, boatProp);

        sqf::set_variable(manProp, "sandbox_pathfinding_manProp", true);
        sqf::add_mission_event
    }

    void PathGenerator::OnFrame(double dt) {

    }

}