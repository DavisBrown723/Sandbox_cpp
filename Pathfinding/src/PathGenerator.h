#pragma once

#include <unordered_map>
#include <queue>

#include "intercept.hpp"

#include "Common/src/System.h"
#include "Common/src/types/Vehicle.h"
#include "types/PathfindingRequest.h"

namespace sandbox {

    class PathGenerator : common::System {

        private:

        std::unordered_map<types::VehicleType, intercept::types::object> m_vehicleTypeObjects;
        std::queue<PathfindingRequest> m_requestQueue;
        PathfindingRequest m_currentRequest;

        public:

        void PostInit();
        void OnFrame(double dt);

    };

}