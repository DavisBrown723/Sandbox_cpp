#pragma once

#include <vector>

#include "Weapon.h"
#include "Magazine.h"

namespace sandbox {
    namespace types {

        struct UnitLoadout {
            std::vector<Weapon> weapons;
            std::vector<Magazine> magazines;
        };

    }
}