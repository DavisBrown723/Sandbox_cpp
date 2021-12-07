#include "pch.h"

#include "SpawnSystem.h"

#include "Common/src/Vector3.h"
#include "Core/src/components/Position.h"
#include "Components.h"
#include "Core/src/Core.h"

using namespace sandbox::components;
using namespace sandbox::types;

namespace sandbox {

    void SpawnSystem::OnFrame(double dt) {
        if (!Core::IsMissionRunning())
            return;

        buildSpawnSources();
        checkSpawnSources();

        processSpawnQueue();
        processDespawnQueue();
    }

    void SpawnSystem::buildSpawnSources() {
        m_spawnSources.clear();

        auto players = intercept::sqf::all_players();
        for (auto& player : players) {
            m_spawnSources.emplace_back(
                intercept::sqf::get_pos(player),
                1500
            );
        }
    }

    void SpawnSystem::checkSpawnSources() {
        auto view = Core::EntityRegistry.view<Position3D, Spawning>();

        for (auto entity : view) {
            auto [position, spawning] = view.get(entity);

            for (auto& source : m_spawnSources) {
                if (position.distance_2d(source.position) <= source.radius) {
                    if (!spawning.active)
                        addToSpawnQueue(entity);
                }
            }
        }

        auto iter = m_activeEntities.begin();
        while (iter != m_activeEntities.end()) {
            if (!Core::EntityRegistry.valid(*iter)) {
                iter = m_activeEntities.erase(iter);
                continue;
            }

            auto entity = *iter;
            auto position = Core::EntityRegistry.get<Position3D>(entity);

            for (auto& source : m_spawnSources) {
                if (position.distance_2d(source.position) > source.radius) {
                    addToDespawnQueue(entity);
                }
            }

            iter++;
        }
    }

    void SpawnSystem::addToSpawnQueue(entt::entity entity) {
        auto handle = entt::basic_handle(Core::EntityRegistry, entity);
        auto iter = std::find(m_spawnQueue.begin(), m_spawnQueue.end(), handle);
        if (iter == m_spawnQueue.end())
            m_spawnQueue.push_back(handle);
    }

    void SpawnSystem::addToDespawnQueue(entt::entity entity) {
        auto handle = entt::basic_handle(Core::EntityRegistry, entity);
        auto iter = std::find(m_despawnQueue.begin(), m_despawnQueue.end(), handle);
        if (iter == m_despawnQueue.end())
            m_despawnQueue.push_back(handle);
    }

    void SpawnSystem::processSpawnQueue() {
        if (m_spawnQueue.empty())
            return;

        auto entity = m_spawnQueue[0];
        spawnEntity(entity);

        m_spawnQueue.erase(m_spawnQueue.begin());
        m_activeEntities.push_back(entity);
    }

    void SpawnSystem::processDespawnQueue() {
        if (m_despawnQueue.empty())
            return;

        auto entity = m_despawnQueue[0];
        despawnEntity(entity);

        auto iter = std::find(m_activeEntities.begin(), m_activeEntities.end(), entity);
        m_activeEntities.erase(iter);

        m_despawnQueue.erase(m_despawnQueue.begin());
    }

    void SpawnSystem::spawnEntity(entt::handle entity) {
        auto& spawning = Core::EntityRegistry.get<Spawning>(entity);
        spawning.spawn(entity);
        spawning.active = true;
    }

    void SpawnSystem::despawnEntity(entt::handle entity) {
        auto& spawning = Core::EntityRegistry.get<Spawning>(entity);
        spawning.despawn(entity);
        spawning.active = false;
    }

}