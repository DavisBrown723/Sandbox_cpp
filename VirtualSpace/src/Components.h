#pragma once

#include <functional>

#include "entt.hpp"

namespace sandbox {
    namespace components {

        struct Spawning {
            bool active;
            std::function<void( entt::handle )> spawn;
            std::function<void( entt::handle )> despawn;
        };

    }
}