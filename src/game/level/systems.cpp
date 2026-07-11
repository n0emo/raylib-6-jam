#include "./systems.hpp"

#include <fstream>
#include <format>

#include <nlohmann/json.hpp>

#include "../level/components.hpp"
#include "../physics/components.hpp"
#include "./components.hpp"
#include "./resource.hpp"

namespace cfu::systems {

static auto reload_single_level(res::LevelCache& cache, res::level::LevelId id, std::string_view path) {
    auto [it, loaded] = cache.load(id, path);
    if (!loaded) *it->second.handle() = res::LevelResource(path);
}

static auto save_single_level(res::LevelCache& cache, res::level::LevelId id, std::string_view path) {
    const auto& level = cache[id]->level;
    auto file = std::ofstream {std::format("assets/data/{}", path)};
    const auto json = nlohmann::json(level);
    file << std::setw(2) << json;
}

auto setup_level_cache(entt::registry& registry) -> void {
    registry.ctx().insert_or_assign(res::LevelCache {});
    reload_levels(registry);
}

auto reload_levels(entt::registry& registry) -> void {
    auto& cache = registry.ctx().get<res::LevelCache>();
    #define CFU_X(name, path) reload_single_level(cache, res::level::name, path);
    RES_LEVEL_ID_LIST
    #undef CFU_X
}

auto save_levels(entt::registry& registry) -> void {
    auto& cache = registry.ctx().get<res::LevelCache>();
    #define CFU_X(name, path) save_single_level(cache, res::level::name, path);
    RES_LEVEL_ID_LIST
    #undef CFU_X
}

auto spawn_level(entt::registry& registry, res::level::LevelId id) -> void {
    comp::deserialize_level(registry, id);

    (void)registry;
    (void)id;
    // TODO
}

auto update_level(entt::registry& registry) -> void {
    auto map_entity = registry.view<const comp::HeightMap>().back();
    if (map_entity == entt::null) return;
    const auto& map = registry.get<const comp::HeightMap>(map_entity);

    auto view = registry.view<const comp::Grounded, comp::Transform>();
    for (auto [entity, grounded, transform] : view.each()) {
        const auto height = map.get_height(transform.translation.x, transform.translation.z);
        transform.translation.y = grounded.y + height;
    }
}

} // namespace cfu::systems
