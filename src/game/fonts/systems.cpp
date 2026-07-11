#include "./systems.hpp"

#include "./resource.hpp"
#include <entt/entity/registry.hpp>
#include <gsl/zstring>

namespace cfu::systems {

// TODO: reload_fonts

auto setup_font_cache(entt::registry& registry) -> void {
    registry.ctx().insert_or_assign(res::FontCache{});
    reload_fonts(registry);
}

static auto reload_single_font(res::FontCache& cache, res::fonts::FontId id, gsl::czstring path) {
    auto [it, loaded] = cache.load(id, path);
    if (!loaded) *it->second.handle() = res::CfuFont(path);
}

auto reload_fonts(entt::registry& registry) -> void {
    auto& cache = registry.ctx().get<res::FontCache>();
    #define CFU_X(name, path) reload_single_font(cache, res::fonts::name, path);
    RES_FONT_ID_LIST
    #undef CFU_X
}
} // namespace cfu::systems
