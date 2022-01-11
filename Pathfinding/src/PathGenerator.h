#pragma once

#include <unordered_map>
#include <queue>
#include <vector>

#include "intercept.hpp"

#include "Common/src/System.h"
#include "Common/src/types/Vehicle.h"
#include "Core/src/components/Position.h"
#include "types/PathfindingRequest.h"

namespace sandbox {

    namespace sqf_functions {
        game_value OnPathCalculated(game_value_parameter right_arg);
    }

    class PathGenerator : common::System {

        friend game_value sqf_functions::OnPathCalculated(game_value_parameter right_arg);

        private:

        std::unordered_map<types::VehicleType, intercept::types::object> m_vehicleTypeObjects;
        std::queue<types::PathfindingRequest> m_requests;

        public:

        void OnPreStart() override;
        void OnPostInit() override;
        void OnFrame(double dt) override;

        protected:

        void OnPathCalculated(intercept::types::object agent, std::vector<components::Position3D> path);

    };

}