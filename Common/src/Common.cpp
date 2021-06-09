#include "pch.h"

#include "Common.h"

#include "types/Side.h"
#include "Config.h"

using namespace intercept;
using namespace sandbox::types;

namespace sandbox {
    namespace common {

        void Common::OnPostStart() {
            Initialize();
        }

        void Common::Initialize() {
            if (m_initialized)
                return;

            CacheSides();

            Config::initialize();

            m_initialized = true;
        }

        void Common::CacheSides() {
            Side::east = std::make_unique<Side>("Opfor", "EAST", "ColorOpfor", 0, sqf::east());
            Side::west = std::make_unique<Side>("Blufor", "WEST", "ColorBlufor", 0, sqf::west());
            Side::independent = std::make_unique<Side>("Independent", "GUER", "ColorIndep", 0, sqf::independent());
            Side::civilian = std::make_unique<Side>("Civilian", "CIV", "ColorCiv", 0, sqf::civilian());

            Side::m_numSideMap = std::unordered_map<int, Side*>();
            Side::m_numSideMap.emplace(0, Side::east.get());
            Side::m_numSideMap.emplace(1, Side::west.get());
            Side::m_numSideMap.emplace(2, Side::independent.get());
            Side::m_numSideMap.emplace(3, Side::civilian.get());

            Side::m_textSideMap = std::unordered_map<std::string, Side*>();
            Side::m_textSideMap.emplace("east", Side::east.get());
            Side::m_textSideMap.emplace("west", Side::west.get());
            Side::m_textSideMap.emplace("guer", Side::independent.get());
            Side::m_textSideMap.emplace("civ", Side::civilian.get());
            Side::m_textSideMap.emplace("unknown", Side::unknown.get());
        }

    }
}