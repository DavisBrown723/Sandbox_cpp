#include "pch.h"

#include "Config.h"
#include "types/Faction.h"
#include "types/Side.h"
#include "types/ConfigGroupCategory.h"
#include "types/ConfigGroup.h"

using namespace intercept;
using namespace sandbox::types;

namespace sandbox {

    std::unordered_map<std::string, std::unique_ptr<types::Faction> > Config::m_factions;

    void Config::initialize() {
        cacheConfigData();
    }

    void Config::cacheConfigData() {
        cacheFactionData();
    }

    void Config::cacheFactionData() {
        sqf::config_entry config = sqf::config_entry(sqf::config_file()) >> "CfgFactionClasses";

        sqf::config_entry configEntry;
        for (int i = 0; i < config.count(); i++) {
            configEntry = sqf::select(config, i);
            if (sqf::is_class(configEntry)) {
                int sideId = (int)sqf::get_number(configEntry >> "side");
                if (sideId < 0 || sideId > 3)
                    continue;

                Side* side = Side::fromNumber(sideId);

                std::string className = sqf::config_name(configEntry);
                std::string displayName = sqf::get_text(configEntry >> "displayName");
                std::string flag = sqf::get_text(configEntry >> "flag");
                std::string icon = sqf::get_text(configEntry >> "icon");

                Config::m_factions.insert({ className, std::make_unique<Faction>(className, displayName, flag, icon, side) });

                cacheFactionGroups(className);
            }
        }
    }

    // #TODO: does not yet account for factions where CfgGroups name != FactionClass name
    void Config::cacheFactionGroups(std::string_view factionClassname) {
        Faction* faction = getFaction(std::string(factionClassname));

        sqf::config_entry config = sqf::config_entry(sqf::config_file()) >> "CfgGroups" >> faction->side->className >> factionClassname;
        sqf::config_entry configEntry;
        for (int i = 0; i != config.count(); i++) {
            configEntry = sqf::select(config, i);
            if (sqf::is_class(configEntry)) {
                ConfigGroupCategory category = parseGroupCategory(configEntry);
                faction->groupCategories.insert({ category.className, category });
            }
        }
    }

    ConfigGroupCategory Config::parseGroupCategory(intercept::sqf::config_entry categoryConfig) {
        ConfigGroupCategory category;
        category.className = sqf::config_name(categoryConfig);

        sqf::config_entry configEntry;
        for (int i = 0; i != categoryConfig.count(); i++) {
            configEntry = sqf::select(categoryConfig, i);
            if (sqf::is_class(configEntry)) {
                int sideId = (int)sqf::get_number(configEntry >> "side");

                ConfigGroup group;
                group.className = sqf::config_name(configEntry);
                group.displayName = sqf::get_text(configEntry >> "name");
                group.icon = sqf::get_text(configEntry >> "icon");
                group.faction = getFaction(sqf::get_text(configEntry >> "faction"));
                group.side = types::Side::fromNumber(sideId);

                sqf::config_entry unitConfigEntry;
                for (int i = 0; i != configEntry.count(); i++) {
                    unitConfigEntry = sqf::select(configEntry, i);
                    if (sqf::is_class(unitConfigEntry)) {
                        ConfigGroupUnit unit;
                        unit.vehicle = sqf::get_text(unitConfigEntry >> "vehicle");
                        unit.rank = sqf::get_text(unitConfigEntry >> "rank");
                        unit.positionOffset = sqf::get_array(unitConfigEntry >> "position");

                        group.units.push_back(unit);
                    }
                }

                category.groups.insert({ group.className, group });
            }
        }

        return category;
    }

    Faction* Config::getFaction(const std::string& str) {
        auto iter = m_factions.find(str);
        if (iter != m_factions.end())
            return iter->second.get();

        return nullptr;
    }

}