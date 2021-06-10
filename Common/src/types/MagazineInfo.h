#pragma once

#include <string>

#include "types/AmmoInfo.h"

namespace sandbox {
    namespace types {

        struct MagazineInfo {
            std::string classname;
            int magazineSize;
            int remainingMagazineSize;
            AmmoInfo ammo;
        };

    }
}