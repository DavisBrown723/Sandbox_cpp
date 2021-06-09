#pragma once

#include <vector>

#include "entt.hpp"

#include "Common/src/System.h"
#include "types.h"

namespace sandbox {

    class SpawnSystem: public common::System {
    private:
        std::vector<entt::handle> m_spawnQueue;
        std::vector<entt::handle> m_despawnQueue;

        std::vector<entt::handle> m_activeEntities;

        std::vector<types::SpawnSource> m_spawnSources;

    public:
        SpawnSystem() {}

        void OnFrame(double dt) override;
        void buildSpawnSources();
        void checkSpawnSources();

        inline void addToSpawnQueue(entt::entity entity);
        inline void addToDespawnQueue(entt::entity entity);

        void processSpawnQueue();
        void processDespawnQueue();

        void spawnEntity(entt::handle entity);
        void despawnEntity(entt::handle entity);
    };

}