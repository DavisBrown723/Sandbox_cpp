#pragma once

#include "Common/src/Singleton.h"
#include "Common/src/System.h"
#include "VirtualSpace/src/SpawnSystem.h"
#include "GroupManager/src/GroupManager.h"

namespace sandbox {

    class Core: public common::Singleton<Core> {

    private:

        bool m_missionRunning;
        bool m_gamePaused;

        double m_lastFrameTime;

        SpawnSystem m_spawnSystem;
        GroupManager m_groupManager;

        std::vector< std::reference_wrapper<common::System> > m_systems;

    public:

        inline static entt::registry EntityRegistry = entt::registry();
        static entt::handle GetEntityHandle(entt::entity entity);

        Core() : m_missionRunning(false), m_gamePaused(false), m_lastFrameTime(0.0) {}

        void OnPreStart();
        void OnPostStart();
        void OnPreInit();
        void OnPostInit();
        void OnFrame();
        void OnMissionPaused();
        void OnMissionUnpaused();
        void OnMissionEnded();
        void OnPluginUnloaded();

        void Initialize();
        void UnInitialize();

        inline static bool IsMissionRunning() { return Core::Get().m_missionRunning; }
        inline static bool IsGamePaused() { return Core::Get().m_gamePaused; };
    };

}