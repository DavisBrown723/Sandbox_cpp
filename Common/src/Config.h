#pragma once

#include "intercept.hpp"

#include "Common.h"
#include "types/Faction.h"
#include "types/ConfigGroupCategory.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>

namespace sandbox {

    class Config {

        friend class common::Common;

        private:

        static std::unordered_map<std::string, std::unique_ptr<types::Faction> > m_factions;

        static void cacheConfigData();
        static void cacheFactionData();
        static void cacheFactionGroups(std::string_view factionClassname);

        static types::ConfigGroupCategory parseGroupCategory(intercept::sqf::config_entry categoryConfig);

        protected:

        static void initialize();

        public:

        static types::Faction* getFaction(const std::string& str);

    };

}