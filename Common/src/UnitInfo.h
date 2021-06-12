#pragma once

#include <string>
#include <unordered_map>

#include "types/BackpackCargo.h"
#include "types/Weapon.h"
#include "types/Magazine.h"
#include "types/Ammo.h"
#include "types/UnitLoadout.h"

namespace sandbox {
    namespace common {
        namespace __internal {
            extern std::unordered_map<std::string, types::WeaponDetails> cachedWeaponDetails;
            extern std::unordered_map<std::string, types::MagazineDetails> cachedMagazineDetails;
            extern std::unordered_map<std::string, types::AmmoDetails> cachedAmmoDetails;
            extern std::unordered_map<std::string, types::UnitLoadout> cachedUnitLoadoutDetails;
        }

        std::vector<int> bitflagsToArray(int flags);

        types::BackpackCargo getBackpackCargo(const std::string& backpackClass, bool includeItems, bool includeMagazines, bool includeWeapons);

        const types::WeaponDetails& getWeaponDetails(const std::string& weaponClass);
        const types::MagazineDetails& getMagazineDetails( const std::string& magazineClass );
        const types::AmmoDetails& getAmmoDetails( const std::string& ammoClass );
        types::UnitLoadout getUnitLoadoutDetails(const std::string& unitClass);

    }
}