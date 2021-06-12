#pragma once

#include <string>

#include "Ammo.h"

namespace sandbox {
    namespace types {

        struct MagazineDetails {
            std::string classname;
            int size;
            AmmoDetails ammo;
        };

        struct Magazine {
            std::string magazineClass;
            int size;
            int ammoRemaining;
        };

    }
}