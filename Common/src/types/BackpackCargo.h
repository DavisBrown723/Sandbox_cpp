#pragma once

#include <vector>
#include <string>

namespace sandbox {
    namespace types {

        struct ItemWithCount {
            std::string classname;
            int count;
        };

        struct BackpackCargo {
            std::vector<ItemWithCount> items;
            std::vector<ItemWithCount> magazines;
            std::vector<ItemWithCount> weapons;
        };

    }
}