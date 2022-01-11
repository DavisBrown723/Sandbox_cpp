#include "pch.h"
#include "PathGenerator.h"

#include "Core/src/Core.h"
#include "Core/src/components/Position.h"
#include "Common/src/SQF.h"

using namespace intercept;
using namespace sandbox;
using namespace sandbox::types;

namespace sandbox {

    namespace sqf_functions {

        game_value OnPathCalculated(game_value_parameter right_arg) {
            intercept::types::object agent = right_arg[0];
            auto path = common::convert::toArray<components::Position3D>(right_arg[1]);

            sandbox::Core::Get().GetPathGenerator().OnPathCalculated(agent, path);

            return game_value();
        }

    }

    void PathGenerator::OnPreStart() {
        intercept::client::host::register_sqf_command(
            "Sandbox_PathGenerator_OnPathCalculated",
            "",
            userFunctionWrapper<sqf_functions::OnPathCalculated>,
            intercept::types::GameDataType::OBJECT,
            intercept::types::GameDataType::ARRAY
        );
    }

    void PathGenerator::OnPostInit() {
        intercept::types::object manProp = sqf::create_vehicle("C_man_1", { 0, 0, 0 }, {}, 0.f, "NONE");
        intercept::types::object motorizedProp = sqf::create_vehicle("C_Offroad_01_F", { 0, 0, 0 }, {}, 0.f, "NONE");
        intercept::types::object mechanizedProp = sqf::create_vehicle("B_APC_Tracked_01_rcws_F", { 0, 0, 0 }, {}, 0.f, "NONE");
        intercept::types::object armoredProp = sqf::create_vehicle("B_MBT_01_cannon_F", { 0, 0, 0 }, {}, 0.f, "NONE");
        intercept::types::object boatProp = sqf::create_vehicle("C_Rubberboat", { 0, 0, 0 }, {}, 0.f, "NONE");

        sqf::set_variable(manProp, "Sandbox_PathGenerator_manProp", true);
        sqf::add_event_handler(manProp, "PathCalculated", "Sandbox_PathGenerator_OnPathCalculated _this");

        sqf::hide_object_global(manProp);
        sqf::hide_object_global(motorizedProp);
        sqf::hide_object_global(mechanizedProp);
        sqf::hide_object_global(armoredProp);
        sqf::hide_object_global(boatProp);

        m_vehicleTypeObjects.emplace(types::VehicleType::Man, manProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Car, motorizedProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Truck, motorizedProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Armored, mechanizedProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::AntiAir, mechanizedProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Tank, armoredProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Artillery, armoredProp);
        m_vehicleTypeObjects.emplace(types::VehicleType::Ship, boatProp);

        sqf::set_variable(manProp, "sandbox_pathfinding_manProp", true);
    }

    void PathGenerator::OnFrame(double dt) {
        if (m_requests.empty())
            return;

        auto& currRequest = m_requests.front();
    }

    void PathGenerator::OnPathCalculated(intercept::types::object agent, std::vector<components::Position3D> path) {
        if (agent.is_null()) {
            sqf::set_pos(agent, { 0,0,0 });
            sqf::do_stop(agent);
        }

        for (auto& pos : path) {
            pos.z = 0;
        }

        // callback

        //m_requests.front().dothecallbackthing();
    }

}