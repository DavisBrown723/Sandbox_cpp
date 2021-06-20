#pragma once

#include <functional>

#include "entt.hpp"

namespace sandbox {
    namespace components {

        enum SpawnState : bool {
            Active = true,
            Inactive = false
        };

        struct Spawning {
            bool active;
            std::function<void( entt::handle )> spawn;
            std::function<void( entt::handle )> despawn;
        };

    }
}