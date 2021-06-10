#pragma once

namespace sandbox {
    namespace types {

        enum class AmmoUse {
            Light = 1,
            Marking = 2,
            Concealment = 4,
            Countermeasures = 8,
            Mine = 16,
            Underwater = 32,
            Infantry = 64,
            Vehicles = 128,
            Air = 248,
            Armor = 512
        };

    }
}