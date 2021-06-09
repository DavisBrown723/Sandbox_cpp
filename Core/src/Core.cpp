#include "pch.h"

#include "Core.h"

#include "Common/src/Common.h"
#include "Common/src/Config.h"
#include "VirtualSpace/src/SpawnSystem.h"
#include "GroupManager/src/GroupManager.h"

namespace sandbox {

    entt::handle Core::GetEntityHandle(entt::entity entity) {
        return entt::basic_handle(Core::EntityRegistry, entity);
    }

    void Core::OnPreStart() {
        m_systems.emplace_back(common::Common::Get());
        m_systems.emplace_back(m_spawnSystem);

            for (auto& system : m_systems)
                system.get().OnPreStart();
    }

    void Core::OnPostStart() {
        for (auto& system : m_systems)
            system.get().OnPostStart();
    }

    void Core::OnPreInit() {
        m_missionRunning = true;

        Initialize();

        for (auto& system : m_systems)
            system.get().OnPreInit();
    }

    void Core::OnPostInit() {
        for (auto& system : m_systems)
            system.get().OnPostInit();
    }

    void Core::OnFrame() {
        if (Core::Get().IsGamePaused())
            return;

        // calculate elapsed time since last simulation step

        float currFrameTickTime = intercept::sqf::diag_ticktime();
        float accTime = intercept::sqf::acc_time();

        double dt = (currFrameTickTime - Core::Get().m_lastFrameTime) * accTime;

        m_lastFrameTime = currFrameTickTime;

        for (auto& system : m_systems)
            system.get().OnFrame(dt);

        static bool spawned = false;
        if (intercept::sqf::time() > 15 && !spawned) {
            auto faction = Config::getFaction("BLU_F");
            m_groupManager.createGroup(faction, faction->groupCategories["Infantry"].groups["BUS_InfAssault"], { 500, 500, 0 });
            spawned = true;
        }
    }

    void Core::OnMissionPaused() {
        m_gamePaused = true;

        for (auto& system : m_systems)
            system.get().OnMissionPaused();
    }

    void Core::OnMissionUnpaused() {
        m_gamePaused = false;

        for (auto& system : m_systems)
            system.get().OnMissionUnpaused();
    }

    void Core::OnMissionEnded() {
        m_missionRunning = false;

        for (auto& system : m_systems)
            system.get().OnMissionEnded();
    }

    void Core::OnPluginUnloaded() {

    }

    void Core::Initialize() {

    }

    void Core::UnInitialize() {

    }

}