#pragma once

#include <string>
#include <vector>

#include "types/AmmoUse.h"

namespace sandbox {
    namespace types {

        struct AmmoInfo {
            std::string classname;
            std::vector<types::AmmoUse> uses;
        };

    }
}