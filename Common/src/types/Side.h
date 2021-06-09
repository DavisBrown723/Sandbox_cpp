#pragma once

#include "intercept.hpp"

#include "Common/src/Common.h"

#include <memory>
#include <string>
#include <string_view>
#include <memory>

namespace sandbox {
    namespace types {

            struct Side {
                friend class sandbox::common::Common;

                std::string displayName;
                std::string className;
                std::string color;
                int numberValue;
                intercept::types::side sideValue;

                Side(std::string_view name, std::string_view textStr, std::string_view colorStr, int id, intercept::types::side);

                static Side* fromClassname(std::string str);
                static Side* fromNumber(int num);

                static std::unique_ptr<Side> east;
                static std::unique_ptr<Side> west;
                static std::unique_ptr<Side> independent;
                static std::unique_ptr<Side> civilian;
                static std::unique_ptr<Side> unknown;

                private:

                static std::unordered_map<int, Side*> m_numSideMap;
                static std::unordered_map<std::string, Side*> m_textSideMap;
            };

    }
}