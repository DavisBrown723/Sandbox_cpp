#include "pch.h"

#include "UnitInfo.h"

#include "types/BackpackCargo.h"
#include "types/TargetingSystem.h"
#include "types/WeaponInfo.h"

#include <math.h>

using namespace intercept;

namespace sandbox {
    namespace common {
        namespace __internal {
            std::unordered_map<std::string, types::WeaponInfo> cachedWeaponInfo;
            std::unordered_map<std::string, int> cachedUnitLoadoutInfo;
        }

        std::vector<int> bitflagsToArray(int flags) {
            std::vector<int> bitflags;
            for (int i = 0; i < 23; i++) {
                int flag = std::pow(2, i);

                if (flag > flags) break;

                bitflags.push_back(flags & flag);
            }
            return bitflags;
        }

        types::BackpackCargo getBackpackCargo(const std::string& backpackClass, bool includeItems, bool includeMagazines, bool includeWeapons) {
            auto backpackConfig = sqf::config_entry(sqf::config_file()) >> backpackClass;
            if (!sqf::is_class(backpackConfig))
                throw std::invalid_argument("getBackpackCargo(): Backpack class does not exist - " + backpackClass);

            types::BackpackCargo cargo;
            sqf::config_entry itemConfig;

            if (includeItems) {
                auto backpackItemsConfig = backpackConfig >> "transportItems";
                for (int i = 0; i < backpackItemsConfig.count(); i++) {
                    itemConfig = sqf::select(backpackItemsConfig, i);
                    if (sqf::is_class(itemConfig)) {
                        cargo.items.emplace_back(
                            sqf::get_text(itemConfig >> "item"),
                            sqf::get_number(itemConfig >> "count")
                        );
                    }
                }
            }

            if (includeMagazines) {
                auto backpackItemsConfig = backpackConfig >> "transportMagazines";
                for (int i = 0; i < backpackItemsConfig.count(); i++) {
                    itemConfig = sqf::select(backpackItemsConfig, i);
                    if (sqf::is_class(itemConfig)) {
                        cargo.items.emplace_back(
                            sqf::get_text(itemConfig >> "magazine"),
                            sqf::get_number(itemConfig >> "count")
                        );
                    }
                }
            }

            if (includeWeapons) {
                auto backpackItemsConfig = backpackConfig >> "transportWeapons";
                for (int i = 0; i < backpackItemsConfig.count(); i++) {
                    itemConfig = sqf::select(backpackItemsConfig, i);
                    if (sqf::is_class(itemConfig)) {
                        cargo.items.emplace_back(
                            sqf::get_text(itemConfig >> "weapon"),
                            sqf::get_number(itemConfig >> "count")
                        );
                    }
                }
            }

            return cargo;
        }

        types::WeaponInfo& getWeaponInfo(const std::string& weaponClass) {
            auto existingInfo = __internal::cachedWeaponInfo.find(weaponClass);
            if (existingInfo == __internal::cachedWeaponInfo.end())
                return existingInfo->second;

            auto weaponConfig = sqf::config_entry(sqf::config_file()) >> "CfgWeapons" >> weaponClass;
            auto compatibleMagsArr = sqf::get_array(weaponConfig >> "magazines").to_array();
            auto weaponLockSystem = sqf::get_number(weaponConfig >> "weaponLockSystem");

            std::vector<std::string> compatibleMags{ compatibleMagsArr.begin(), compatibleMagsArr.end() };

            // magazine info section
            //
            //

            std::vector<int> targetingSystemFlags = bitflagsToArray(weaponLockSystem);
            std::vector<types::TargetingSystem> targetingSystems;
            if (weaponLockSystem != 0) {
                targetingSystems.insert(targetingSystems.end(), targetingSystemFlags.begin(), targetingSystemFlags.end() );
            }

            auto newItem = __internal::cachedWeaponInfo.emplace(weaponClass, weaponClass, targetingSystems, compatibleMags );

            return newItem.first->second;
        }

        int getUnitLoadoutInfo(const std::string& unitClass) {
            auto config = sqf::config_entry(sqf::config_file());
            auto cfgVehicles = config >> "CfgVehicles";

            auto existingInfo = __internal::cachedUnitLoadoutInfo.find(unitClass);
            if (existingInfo == __internal::cachedUnitLoadoutInfo.end())
                return existingInfo->second;

            auto unitConfig = config >> unitClass;
            auto weaponsFromConfig = sqf::get_array(unitConfig >> "weapons").to_array();
            auto magazinesFromConfig = sqf::get_array(unitConfig >> "magazines").to_array();

            std::remove_if(weaponsFromConfig.begin(), weaponsFromConfig.end(), [](intercept::types::game_value str) { return str == "Throw" || str == "Put" || str == "Binocular"; });

            std::vector<std::string> weapons{ weaponsFromConfig.begin(), weaponsFromConfig.end() };
            std::vector<std::string> magazines{ magazinesFromConfig.begin(), magazinesFromConfig.end() };

            // add weapons and magazines from backpack

            std::string backpack = sqf::get_text(unitConfig >> "backpack");
            types::BackpackCargo backpackCargo = getBackpackCargo(backpack, false, true, true);

            weapons.insert(weapons.end(), backpackCargo.items.begin(), backpackCargo.items.end());
            magazines.insert(weapons.end(), backpackCargo.magazines.begin(), backpackCargo.magazines.end());

            // get weapon info

            for (auto& weapon : weapons) {
                auto weaponInfo = getWeaponInfo(weapon);
            }

            return -1;
        }

    }
}