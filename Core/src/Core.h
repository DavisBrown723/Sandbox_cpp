#pragma once

#include "signal.hpp"
#include "intercept.hpp"

#include "Common/src/Singleton.h"
#include "Common/src/System.h"
#include "VirtualSpace/src/SpawnSystem.h"
#include "GroupManager/src/GroupManager.h"
#include "Pathfinding/src/PathGenerator.h"

#ifdef INCLUDE_DEMO
    #include "Demo/src/Demo.h"
#endif

namespace sandbox {

    class Core: public common::Singleton<Core> {

    private:

        bool m_missionRunning;
        bool m_gamePaused;

        double m_lastFrameTime;

        SpawnSystem m_spawnSystem;
        GroupManager m_groupManager;
        PathGenerator m_pathGenerator;
        #ifdef INCLUDE_DEMO
            __internal::Demo m_demo;
        #endif

        std::vector< std::reference_wrapper<common::System> > m_systems;

    public:

        inline static entt::registry EntityRegistry = entt::registry();
        static entt::handle GetEntityHandle(entt::entity entity);

        inline static sigslot::signal<intercept::types::object&, intercept::types::object&> OnKilledEH;

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

        // Event handlers
        void OnKilled(intercept::types::object& unit, intercept::types::object& killer);

        void Initialize();
        void UnInitialize();

        inline static bool IsMissionRunning() { return Core::Get().m_missionRunning; }
        inline static bool IsGamePaused() { return Core::Get().m_gamePaused; }

        inline static GroupManager GetGroupManager() { return Core::Get().m_groupManager; }
        inline static PathGenerator GetPathGenerator() { return Core::Get().m_pathGenerator; }
    };

}