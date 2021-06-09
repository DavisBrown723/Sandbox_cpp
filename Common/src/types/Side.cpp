#include "pch.h"

#include "Side.h"

namespace sandbox {
    namespace types {

        std::unique_ptr<Side> Side::east;
        std::unique_ptr<Side> Side::west;
        std::unique_ptr<Side> Side::independent;
        std::unique_ptr<Side> Side::civilian;
        std::unique_ptr<Side> Side::unknown;

        std::unordered_map<int, Side*> Side::m_numSideMap;
        std::unordered_map<std::string, Side*> Side::m_textSideMap;


        Side::Side(std::string_view name, std::string_view classNameStr, std::string_view colorStr, int id, intercept::types::side value)
            : displayName(name), className(classNameStr), color(colorStr), numberValue(id), sideValue(value) {
        }

        Side* Side::fromClassname(std::string str) {
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);

            auto iter = m_textSideMap.find(str);
            if (iter != m_textSideMap.end())
                return iter->second;

            return unknown.get();
        }

        Side* Side::fromNumber(int num) {
            auto iter = m_numSideMap.find(num);
            if (iter != m_numSideMap.end())
                return iter->second;

            return unknown.get();
        }

    }
}