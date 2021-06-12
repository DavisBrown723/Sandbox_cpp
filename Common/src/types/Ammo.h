#pragma once

#include <string>
#include <vector>

#include "AmmoUse.h"

namespace sandbox {
    namespace types {

        struct AmmoDetails {
            std::string classname;
            std::vector<types::AmmoUse> uses;
        };

    }
}