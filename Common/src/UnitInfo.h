#pragma once

#include <string>
#include <unordered_map>

#include "types/BackpackCargo.h"
#include "types/WeaponInfo.h"
#include "types/MagazineInfo.h"
#include "types/AmmoInfo.h"

namespace sandbox {
    namespace common {
        namespace __internal {
            extern std::unordered_map<std::string, types::WeaponInfo> cachedWeaponInfo;
            extern std::unordered_map<std::string, types::MagazineInfo> cachedMagazineInfo;
            extern std::unordered_map<std::string, types::AmmoInfo> cachedAmmoInfo;
            extern std::unordered_map<std::string, int> cachedUnitLoadoutInfo;
        }

        std::vector<int> bitflagsToArray(int flags);

        types::BackpackCargo getBackpackCargo(const std::string& backpackClass, bool includeItems, bool includeMagazines, bool includeWeapons);

        types::WeaponInfo getWeaponInfo(const std::string& weaponClass);
        types::MagazineInfo getMagazineInfo( const std::string& magazineClass );
        types::AmmoInfo getAmmoInfo( const std::string& ammoClass );
        int getUnitLoadoutInfo(const std::string& unitClass);

    }
}