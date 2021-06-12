#include "pch.h"

#include "UnitInfo.h"

#include "types/BackpackCargo.h"
#include "types/TargetingSystem.h"
#include "types/Weapon.h"
#include "types/Magazine.h"
#include "types/Ammo.h"
#include "types/UnitLoadout.h"

#include <math.h>

using namespace intercept;
using namespace sandbox::types;

namespace sandbox {
    namespace common {
        namespace __internal {
            std::unordered_map<std::string, WeaponDetails> cachedWeaponDetails;
            std::unordered_map<std::string, MagazineDetails> cachedMagazineDetails;
            std::unordered_map<std::string, AmmoDetails> cachedAmmoDetails;
            std::unordered_map<std::string, UnitLoadout> cachedUnitLoadoutDetails;
        }

        std::vector<int> bitflagsToArray(int flags) {
            std::vector<int> bitflags;
            for (int i = 0; i < 23; i++) {
                int flag = (int)std::pow(2, i);

                if (flag > flags) break;

                bitflags.push_back(flags & flag);
            }
            return bitflags;
        }

        BackpackCargo getBackpackCargo(const std::string& backpackClass, bool includeItems, bool includeMagazines, bool includeWeapons) {
            auto backpackConfig = sqf::config_entry(sqf::config_file()) >> backpackClass;
            if (!sqf::is_class(backpackConfig))
                throw std::invalid_argument("getBackpackCargo(): Backpack class does not exist - " + backpackClass);

            BackpackCargo cargo;
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

        const WeaponDetails& getWeaponDetails(const std::string& weaponClass) {
            auto existingInfo = __internal::cachedWeaponDetails.find(weaponClass);
            if (existingInfo == __internal::cachedWeaponDetails.end())
                return existingInfo->second;

            auto weaponConfig = sqf::config_entry(sqf::config_file()) >> "CfgWeapons" >> weaponClass;
            auto compatibleMagsArr = sqf::get_array(weaponConfig >> "magazines").to_array();
            int weaponLockSystem = (int)sqf::get_number(weaponConfig >> "weaponLockSystem");

            std::vector<std::string> compatibleMags{ compatibleMagsArr.begin(), compatibleMagsArr.end() };

            std::vector<int> targetingSystemFlags = bitflagsToArray(weaponLockSystem);
            std::vector<TargetingSystem> targetingSystems;
            std::transform( targetingSystemFlags.begin(), targetingSystemFlags.end(), targetingSystems.begin(), []( int flag ) { 
                return static_cast<TargetingSystem>(flag);
            } );

            auto newItem = __internal::cachedWeaponDetails.insert( { weaponClass, { weaponClass, targetingSystems, compatibleMags } } );

            return newItem.first->second;
        }

        const MagazineDetails& getMagazineDetails( const std::string& magazineClass ) {
            auto existingInfo = __internal::cachedMagazineDetails.find( magazineClass );
            if (existingInfo == __internal::cachedMagazineDetails.end())
                return existingInfo->second;

            auto magazineConfig = sqf::config_entry( sqf::config_file() ) >> "CfgMagazines" >> magazineClass;
            int magazineSize = (int)sqf::get_number( magazineConfig >> "count" );
            std::string magazineAmmo = sqf::get_text( magazineConfig >> "ammo" );

            AmmoDetails ammo = getAmmoDetails( magazineAmmo );

            auto newItem = __internal::cachedMagazineDetails.insert( { magazineClass, { magazineClass, magazineSize, ammo } } );

            return newItem.first->second;
        }

        const AmmoDetails& getAmmoDetails( const std::string& ammoClass ) {
            auto existingInfo = __internal::cachedAmmoDetails.find( ammoClass );
            if (existingInfo == __internal::cachedAmmoDetails.end())
                return existingInfo->second;

            auto ammoConfig = sqf::config_entry( sqf::config_file() ) >> "CfgAmmo" >> ammoClass;

            std::vector<AmmoUse> ammoUses;
            int ammoUsageFlags = (int)sqf::get_number( ammoConfig >> "aiAmmoUsageFlags" );
            if (ammoUsageFlags != 0) {
                std::vector<int> ammoBitflagArr = bitflagsToArray( ammoUsageFlags );
                std::transform( ammoBitflagArr.begin(), ammoBitflagArr.end(), ammoUses.begin(), []( int flag ) {
                    return static_cast<AmmoUse>(flag);
                } );
            } else {
                int airLock = (int)sqf::get_number( ammoConfig >> "airLock" );
                switch (airLock) {
                    case 0:
                    {
                        ammoUses.insert( ammoUses.end(), { AmmoUse::Infantry, AmmoUse::Vehicles, AmmoUse::Armor } );
                        break;
                    }
                    case 1:
                    {
                        ammoUses.insert( ammoUses.end(), { AmmoUse::Vehicles, AmmoUse::Armor, AmmoUse::Air } );
                        break;
                    }
                    case 2:
                    {
                        ammoUses.push_back( AmmoUse::Air );
                        break;
                    }
                }
            }

            auto newItem = __internal::cachedAmmoDetails.insert( { ammoClass, { ammoClass, ammoUses } } );

            return newItem.first->second;
        }

        UnitLoadout getUnitLoadoutDetails(const std::string& unitClass) {
            auto config = sqf::config_entry(sqf::config_file());
            auto cfgVehicles = config >> "CfgVehicles";

            auto existingInfo = __internal::cachedUnitLoadoutDetails.find(unitClass);
            if (existingInfo == __internal::cachedUnitLoadoutDetails.end())
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

            std::transform( backpackCargo.items.begin(), backpackCargo.items.end(), weapons.end(), []( types::ItemWithCount& item ) {
                return item.classname;
            } );

            std::transform( backpackCargo.magazines.begin(), backpackCargo.magazines.end(), magazines.end(), []( types::ItemWithCount& item ) {
                return item.classname;
            } );

            std::vector<Weapon> unitWeapons;
            std::vector<Magazine> unitMagazines;
            std::transform(weapons.begin(), weapons.end(), unitWeapons.end(), [](const std::string& weaponClass) -> Weapon { return { weaponClass }; });
            std::transform(magazines.begin(), magazines.end(), unitMagazines.end(), [](const std::string& magazineClass) -> Magazine {
                MagazineDetails details = getMagazineDetails(magazineClass);
                return { magazineClass, details.size, details.size };
            });

            auto newItem = __internal::cachedUnitLoadoutDetails.insert({ unitClass, { unitWeapons, unitMagazines } });

            return newItem.first->second;
        }

    }
}