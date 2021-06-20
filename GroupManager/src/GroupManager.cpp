#include "pch.h"

#include "GroupManager.h"

#include "signal.hpp"

#include "components/Group.h"
#include "components/Unit.h"
#include "components/Vehicle.h"
#include "Common/src/types/Faction.h"
#include "Common/src/types/ConfigGroup.h"
#include "Common/src/types/UnitLoadout.h"
#include "Common/src/types/VehicleSeats.h"
#include "Common/src/types/Vehicle.h"
#include "Common/src/UnitInfo.h"
#include "Common/src/Vehicles.h"
#include "Core/src/Core.h"
#include "Core/src/components/Position.h"
#include "Core/src/components/EntityBase.h"
#include "Core/src/components/Allegiance.h"
#include "Core/src/components/Speed.h"
#include "Core/src/types/EntityTypes.h"
#include "VirtualSpace/src/Components.h"

using namespace sandbox::components;
using namespace sandbox::types;
using namespace intercept;

namespace sandbox {

    GroupManager::GroupManager() {
        OnKilledEHConnection = Core::OnKilledEH.connect(&GroupManager::onUnitKilled, this);
    }

    GroupManager::~GroupManager() {
        OnKilledEHConnection.disconnect();
    }

    void GroupManager::createUnitsForGroup( entt::entity owningGroup, types::ConfigGroup group, Faction* faction, const types::Vector3& position ) {
        for (auto& configUnit : group.units) {
            if (sqf::is_kind_of( configUnit.vehicle, "Man" )) {
                createUnit( configUnit.vehicle, owningGroup );
            } else {
                createVehicle( configUnit.vehicle, faction, position );
                // assign to group
            }
        }
    }

    entt::entity GroupManager::createUnit( const std::string& unitClass, entt::entity owningGroup ) {
        auto entity = Core::EntityRegistry.create();
        Core::EntityRegistry.emplace<Unit>( entity, unitClass, owningGroup, intercept::sqf::obj_null(), 0.f ); // #TODO: cache null values in sandbox::Config
        Core::EntityRegistry.emplace<UnitLoadout>( entity, common::getUnitLoadoutDetails( unitClass ) );

        return entity;
    }

    entt::handle GroupManager::createGroup(Faction* faction, types::ConfigGroup group, const types::Vector3& position) {
        auto entity = Core::EntityRegistry.create();
        Core::EntityRegistry.emplace<EntityBase>(entity, EntityType::Group);
        Core::EntityRegistry.emplace<Position3D>(entity, position);
        Core::EntityRegistry.emplace<Spawning>(entity,
            SpawnState::Inactive,
            std::bind(&GroupManager::spawnGroup, this, std::placeholders::_1),
            std::bind(&GroupManager::despawnGroup, this, std::placeholders::_1)
        );

        Core::EntityRegistry.emplace<Allegiance>(entity, faction->side, faction);
        Core::EntityRegistry.emplace<Speed>(entity, 4.3);

        std::vector<ConfigGroupUnit> units;
        std::vector<ConfigGroupUnit> vehicles;

        Core::EntityRegistry.emplace<Group>(entity,
            intercept::sqf::grp_null(),
            std::vector<entt::entity>(),
            400
        );

        createUnitsForGroup( entity, group, faction, position );

        return Core::GetEntityHandle(entity);
    }

    entt::handle GroupManager::createVehicle( const std::string& vehicleClass, types::Faction* faction, const types::Vector3& position ) {
        auto entity = Core::EntityRegistry.create();
        Core::EntityRegistry.emplace<EntityBase>( entity, EntityType::Vehicle );
        Core::EntityRegistry.emplace<Position3D>( entity, position );
        Core::EntityRegistry.emplace<Spawning>( entity,
            SpawnState::Inactive,
            std::bind( &GroupManager::spawnVehicle, this, std::placeholders::_1 ),
            std::bind( &GroupManager::despawnVehicle, this, std::placeholders::_1 )
        );
        Core::EntityRegistry.emplace<Allegiance>( entity, faction->side, faction );
        Core::EntityRegistry.emplace<Speed>( entity, common::findVehicleSpeed( vehicleClass ) );
        Core::EntityRegistry.emplace<Vehicle>( entity, vehicleClass, common::findVehicleType(vehicleClass), EngineState::EngineOff );
        Core::EntityRegistry.emplace<VehicleSeats>( entity, common::findVehicleSeats( vehicleClass ) );

        return Core::GetEntityHandle( entity );
    }

    void GroupManager::spawnGroup(entt::handle entity) {
        auto& group = Core::EntityRegistry.get<Group>(entity);
        auto& pos = Core::EntityRegistry.get<Position3D>(entity);
        auto& allegiance = Core::EntityRegistry.get<Allegiance>(entity);

        group.group = sqf::create_group(allegiance.side->sideValue, true);
        for (auto& unit : group.units) {
            spawnUnit(unit, pos, group.group);
        }
        intercept::sqf::system_chat("WE SPAWNED BABY");
    }

    void GroupManager::despawnGroup(entt::handle entity) {
        auto& group = Core::EntityRegistry.get<Group>(entity);

        for (auto& unit : group.units) {
            despawnUnit(unit);
        }

        sqf::delete_group(group.group);

        intercept::sqf::system_chat("OK WE DESPAWNED FUCKKKK");
    }

    void GroupManager::spawnVehicle( entt::handle entity ) {

    }

    void GroupManager::despawnVehicle( entt::handle entity ) {

    }

    void GroupManager::spawnUnit(entt::entity entity, Position3D& pos, intercept::types::group group) {
        auto& unit = Core::EntityRegistry.get<Unit>(entity);

        unit.unitObject = sqf::create_unit(group, unit.classname, pos, {}, 0.f, "FORM");
        sqf::set_damage(unit.unitObject, unit.damage);

        auto x = GroupManager::UnitEntityIDVar;

        sqf::system_chat(std::string("ENTITY ID = ") + std::to_string((int)entity));
        sqf::set_variable(unit.unitObject, GroupManager::UnitEntityIDVar, game_value((int)entity));
    }

    void GroupManager::despawnUnit(entt::entity entity) {
        auto& unit = Core::EntityRegistry.get<Unit>(entity);

        unit.damage = sqf::damage(unit.unitObject);
        
        sqf::delete_vehicle(unit.unitObject);
    }

    void GroupManager::groupRemoveUnit(entt::entity groupEntity, entt::entity unitEntity) {
        auto& group = Core::EntityRegistry.get<Group>(groupEntity);
        
        auto iter = std::find(group.units.begin(), group.units.end(), unitEntity);
        if (iter != group.units.end())
            group.units.erase(iter);
    }

    void GroupManager::assignVehicleToGroup( entt::entity group, entt::entity vehicle ) {

    }

    void GroupManager::assignGroupToVehicleCargo( entt::entity group, entt::entity vehicle ) {

    }

    void GroupManager::onUnitKilled(intercept::types::object& unit, intercept::types::object& killer) {
        int entityID = sqf::get_variable(unit, GroupManager::UnitEntityIDVar, -1);
        if (entityID != -1) {
            unregisterUnit(entt::entity(entityID));
        }
    }

    void GroupManager::unregisterUnit(entt::entity entity) {
        auto& unit = Core::EntityRegistry.get<Unit>(entity);
        auto& group = Core::EntityRegistry.get<Group>(unit.group);

        groupRemoveUnit(unit.group, entity);

        bool deleteGroup = group.units.empty();
        entt::entity groupToDelete = unit.group;

        Core::EntityRegistry.destroy(entity);

        if (deleteGroup)
            unregisterGroup(groupToDelete);
    }

    void GroupManager::unregisterGroup(entt::entity entity) {
        auto& group = Core::EntityRegistry.get<Group>(entity);
        auto& spawning = Core::EntityRegistry.get<Spawning>(entity);

        for (auto& unitEntity : group.units) {
            if (spawning.active) {
                auto& unit = Core::EntityRegistry.get<Unit>(unitEntity);
                sqf::delete_vehicle(unit.unitObject);
            }
            unregisterUnit(unitEntity);
        }

        if (spawning.active)
            sqf::delete_group(group.group);

        Core::EntityRegistry.destroy(entity);
    }

}