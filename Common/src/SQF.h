#pragma once

#include <intercept.hpp>

#include <vector>

namespace sandbox {
    namespace common {

        namespace convert {

            template<typename T>
            std::vector<T> toArray(intercept::types::game_value value);

        }

    }
}