#include "pch.h"

#include "GroupManager.h"

#include "signal.hpp"

#include "components/Group.h"
#include "components/Unit.h"
#include "components/Vehicle.h"
#include "components/VehicleAssignments.h"
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
#include "Core/src/EntityTypes.h"
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

    void GroupManager::createUnitsForGroup(entt::entity owningGroup, const std::vector<std::string>& unitClasses, Faction* faction, const types::Vector3& position) {
        for (auto& unitClass : unitClasses) {
            if (sqf::is_kind_of(unitClass, "Man")) {
                createUnit(unitClass, owningGroup);
            } else {
                auto vehicle = createVehicle(unitClass, faction, position);
                assignVehicleToGroup(owningGroup, vehicle);
            }
        }
    }

    void GroupManager::createUnitsForGroup( entt::entity owningGroup, types::ConfigGroup group, Faction* faction, const types::Vector3& position ) {
        createUnitsForGroup(owningGroup, getConfigGroupUnitClasses(group), faction, position);
    }

    std::vector<std::string> GroupManager::getConfigGroupUnitClasses(types::ConfigGroup group) {
        std::vector<std::string> unitClasses;
        unitClasses.reserve(group.units.size());

        for (const auto& unit : group.units)
            unitClasses.push_back(unit.vehicle);

        return unitClasses;
    }

    entt::entity GroupManager::createUnit( const std::string& unitClass, entt::entity owningGroup ) {
        auto entity = Core::EntityRegistry.create();
        Core::EntityRegistry.emplace<Unit>( entity, unitClass, owningGroup, intercept::sqf::obj_null(), 0.f ); // #TODO: cache null values in sandbox::Config
        Core::EntityRegistry.emplace<UnitLoadout>( entity, common::getUnitLoadoutDetails( unitClass ) );

        return entity;
    }

    entt::handle GroupManager::createGroup(std::vector<std::string> unitClasses, types::Faction* faction, const types::Vector3& position) {
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
        Core::EntityRegistry.emplace<VehicleAssignments>(entity);

        Core::EntityRegistry.emplace<Group>(entity,
            intercept::sqf::grp_null(),
            std::vector<entt::entity>(),
            400
        );

        createUnitsForGroup(entity, unitClasses, faction, position);

        return Core::GetEntityHandle(entity);
    }

    entt::handle GroupManager::createGroup(Faction* faction, types::ConfigGroup group, const types::Vector3& position) {
        return entt::handle();
        return createGroup(getConfigGroupUnitClasses(group), faction, position);
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
        Core::EntityRegistry.emplace<Vehicle>( entity, vehicleClass, common::findVehicleType(vehicleClass), intercept::sqf::obj_null(), EngineState::EngineOff );
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

    std::vector<GroupManager::VehicleSeatRef> GroupManager::getVehicleEmptySeats(entt::entity vehicleEntity, EmptyVehicleSeats cargoOnly) {
        auto& seats = Core::EntityRegistry.get<VehicleSeats>(vehicleEntity);
        std::vector<VehicleSeatRef> emptySeats;

        if (!cargoOnly) {
            if (seats.driver && seats.driver->assignedUnit)
                emptySeats.push_back(seats.driver.value());

            for (auto& seat : seats.turrets) {
                if (seat.assignedUnit)
                    emptySeats.push_back(seat);
            }
        }

        for (auto& seat : seats.cargo) {
            if (seat.assignedUnit)
                emptySeats.push_back(seat);
        }

        return emptySeats;
    }

    std::vector<entt::entity> GroupManager::getUnseatedUnits(entt::entity group) {
        auto& groupComponent = Core::EntityRegistry.get<Group>(group);

        std::vector<entt::entity> unseatedUnits;

        for (auto& unit : groupComponent.units) {
            VehicleSeat* vehicleSeat = Core::EntityRegistry.try_get<VehicleSeat>(unit);
            if (vehicleSeat == nullptr)
                unseatedUnits.push_back(unit);
        }

        return unseatedUnits;
    }

    bool GroupManager::isGroupInCommandOfVehicle(entt::entity group, entt::entity vehicle) {
        auto& vehicleAssignments = Core::EntityRegistry.get<VehicleAssignments>(group);

        return std::find(vehicleAssignments.vehiclesInCommandOf.begin(), vehicleAssignments.vehiclesInCommandOf.end(), vehicle) != vehicleAssignments.vehiclesInCommandOf.end();
    }

    bool GroupManager::isGroupInCargoOfVehicle(entt::entity group, entt::entity vehicle) {
        auto& vehicleAssignments = Core::EntityRegistry.get<VehicleAssignments>(group);

        return std::find(vehicleAssignments.vehiclesInCargoOf.begin(), vehicleAssignments.vehiclesInCargoOf.end(), vehicle) != vehicleAssignments.vehiclesInCargoOf.end();
    }

    bool GroupManager::isVehicleAssignedToGroup(entt::entity group, entt::entity vehicle) {
        if (isGroupInCommandOfVehicle(group, vehicle))
            return true;

        if (isGroupInCargoOfVehicle(group, vehicle))
            return true;

        return false;
    }

    void GroupManager::assignUnitToSeat(entt::entity unit, Vehicle& vehicle, VehicleSeatRef seat, bool isActive) {
        seat.get().assignedUnit = unit;

        Core::EntityRegistry.emplace<VehicleSeatRef>(unit, seat);

        if (isActive) {
            auto& unitComponent = Core::EntityRegistry.get<Unit>(unit);
            switch (seat.get().type()) {
                case VehicleSeatType::Cargo:
                {
                    sqf::assign_as_cargo_index(unitComponent.unitObject, vehicle.vehicleObject, seat.get().path.cargoIndex.value());
                }
                case VehicleSeatType::Turret:
                {
                    sqf::assign_as_turret(unitComponent.unitObject, vehicle.vehicleObject, seat.get().path.turretPath.value());
                }
                case VehicleSeatType::Driver:
                {
                    sqf::assign_as_driver(unitComponent.unitObject, vehicle.vehicleObject);
                }
            }

            std::vector<intercept::types::object> units{ unitComponent.unitObject };
            sqf::order_get_in(units, true);
        }
    }

    void GroupManager::assignVehicleToGroup( entt::entity group, entt::entity vehicle ) {
        auto& vehicleAssignments = Core::EntityRegistry.get<VehicleAssignments>(group);

        if (isVehicleAssignedToGroup(group, vehicle))
            return;

        auto& spawning = Core::EntityRegistry.get<Spawning>(group);
        auto& vehicleComponent = Core::EntityRegistry.get<Vehicle>(vehicle);

        auto unseatedUnits = getUnseatedUnits(group);
        std::vector<intercept::types::object> seatedUnits;

        for (auto& seat : getVehicleEmptySeats(vehicle, EmptyVehicleSeats::AllSeats)) {
            entt::entity unit = unseatedUnits.front();
            unseatedUnits.erase(unseatedUnits.begin());

            assignUnitToSeat(unit, vehicleComponent, seat, spawning.active);
        }

        vehicleAssignments.vehiclesInCommandOf.push_back(Core::GetEntityHandle(vehicle));

        // #TODO: set group vehicle type
        // #TODO: calculate new group speed
        // #TODO: determine and cache pathfinding strategy
        // #TODO: update vehicle debug marker color
    }

    void GroupManager::assignGroupToVehicleCargo( entt::entity group, entt::entity vehicle ) {
        auto& vehicleAssignments = Core::EntityRegistry.get<VehicleAssignments>(group);

        if (isVehicleAssignedToGroup(group, vehicle))
            return;

        auto& spawning = Core::EntityRegistry.get<Spawning>(group);
        auto& vehicleComponent = Core::EntityRegistry.get<Vehicle>(vehicle);

        auto unseatedUnits = getUnseatedUnits(group);
        std::vector<intercept::types::object> seatedUnits;

        for (auto& seat : getVehicleEmptySeats(vehicle, EmptyVehicleSeats::CargoOnly)) {
            entt::entity unit = unseatedUnits.front();
            unseatedUnits.erase(unseatedUnits.begin());

            assignUnitToSeat(unit, vehicleComponent, seat, spawning.active);
        }

        vehicleAssignments.vehiclesInCommandOf.push_back(Core::GetEntityHandle(vehicle));

        // #TODO: set group vehicle type
        // #TODO: calculate new group speed
        // #TODO: determine and cache pathfinding strategy
        // #TODO: update vehicle debug marker color
    }

    void GroupManager::unassignUnitFromSeat(entt::entity unit) {
        auto* seatAssignment = Core::EntityRegistry.try_get<SeatAssignment>(unit);
        if (seatAssignment != nullptr) {
            auto& unitComponent = Core::EntityRegistry.get<Unit>(unit);
            auto& vehicle = Core::EntityRegistry.get<Vehicle>(seatAssignment->vehicle);

            if (unitComponent.unitObject != intercept::sqf::obj_null()) // #TODO use cached null value
                sqf::leave_vehicle(unitComponent.unitObject, vehicle.vehicleObject);

            seatAssignment->seat.assignedUnit.reset();
            Core::EntityRegistry.remove<SeatAssignment>(unit);
        }
    }

    void GroupManager::unassignGroupFromVehicle(entt::entity group, entt::entity vehicle) {
        if (!isVehicleAssignedToGroup(group, vehicle))
            return;

        auto& groupComponent = Core::EntityRegistry.get<Group>(group);
        for (auto& unit : groupComponent.units)
            unassignUnitFromSeat(unit);

        // #TODO: set group vehicle type
        // #TODO: calculate new group speed
        // #TODO: determine and cache pathfinding strategy
        // #TODO: update vehicle debug marker color

        auto& vehicleAssignments = Core::EntityRegistry.get<VehicleAssignments>(group);
        auto iter = std::find(vehicleAssignments.vehiclesInCommandOf.begin(), vehicleAssignments.vehiclesInCommandOf.end(), vehicle);
        if (iter != vehicleAssignments.vehiclesInCommandOf.end()) {
            vehicleAssignments.vehiclesInCommandOf.erase(iter);
            return;
        }

        iter = std::find(vehicleAssignments.vehiclesInCargoOf.begin(), vehicleAssignments.vehiclesInCargoOf.end(), vehicle);
        if (iter != vehicleAssignments.vehiclesInCargoOf.end())
            vehicleAssignments.vehiclesInCargoOf.erase(iter);
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