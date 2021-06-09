#pragma once

#include "ConfigGroupCategory.h"

#include <string>
#include <string_view>
#include <unordered_map>

namespace sandbox {
    namespace types {

        struct Side;

        struct Faction {
            using GroupCategories = std::unordered_map<std::string, ConfigGroupCategory>;

            Faction(std::string_view className_, std::string_view displayName_, std::string_view flag_, std::string_view icon_, Side* side_, GroupCategories categories_ = GroupCategories())
                : className(className_), displayName(displayName_), flag(flag_), icon(icon_), side(side_), groupCategories(categories_)
            {
            }

            std::string className;
            std::string displayName;
            std::string flag;
            std::string icon;
            Side* side;
            GroupCategories groupCategories;
        };

    }
}