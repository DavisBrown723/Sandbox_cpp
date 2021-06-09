#pragma once

#include <optional>

#include "Common/src/types/Side.h"
#include "Common/src/types/Faction.h"

namespace sandbox {
    namespace components {

        struct Allegiance {
            types::Side* side;
            types::Faction* faction;
        };

    }
}