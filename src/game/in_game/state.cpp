#include "./state.hpp"

#include <raylib.h>

#include "../background/components.hpp"
#include "../background/systems.hpp"
#include "../camera/systems.hpp"
#include "../physics/systems.hpp"
#include "../player/components.hpp"
#include "../player/systems.hpp"
#include "../shaders/systems.hpp"
#include "../shaders/components.hpp"
#include "../shapes/systems.hpp"
#include "../solids/systems.hpp"
#include "../level/components.hpp"
#include "../level/systems.hpp"
#include "../vox/systems.hpp"
#include "../raymath.hpp"
#include "./components.hpp"

namespace cfu {

auto InGameState::on_enter(entt::registry& registry) -> void {
    TraceLog(LOG_INFO, "InGameState::on_enter");

    auto bg = registry.create();
    registry.emplace<comp::BackgroundColor>(bg, SKYBLUE);
    registry.emplace<comp::InGameTag>(bg);

    auto player = registry.create();
    comp::create_player(registry, player);
    registry.emplace<comp::InGameTag>(player);

    auto camera = registry.create();
    systems::create_camera(registry, camera);
    registry.emplace<comp::InGameTag>(camera);

    systems::spawn_level(registry, res::level::Level1);

    // auto tilemap = registry.create();
    // comp::create_map(registry, tilemap, 128, 128);
    // registry.emplace<comp::InGameTag>(tilemap);

    // for (auto i = 0; i < 128; i++) {
    //     for (auto j = 0; j < 128; j++) {
    //         auto tile = registry.create();
    //         comp::create_tile(registry, tile, comp::TileKind::Floor, comp::TileRotation::None, i, 0, j);
    //         registry.emplace<comp::InGameTag>(tile);
    //     }
    // }
}

auto InGameState::on_exit(entt::registry& registry) -> void {
    TraceLog(LOG_INFO, "InGameState::on_exit");
    auto view = registry.view<comp::InGameTag>();
    registry.destroy(view.begin(), view.end());
}

auto InGameState::update(entt::registry& registry) -> void {
    systems::update_player(registry);
    // systems::update_tilemap(registry);
    systems::update_shaders(registry);
}

auto InGameState::draw(entt::registry& registry) -> void {
    systems::draw_background_color(registry);
    systems::update_camera(registry);
    systems::begin_camera_mode(registry);
    systems::draw_physics(registry);
    systems::draw_solids(registry);
    auto shader = comp::get_lighting_shader(registry);
    systems::draw_voxel_models(registry, shader);

    systems::draw_shapes(registry);
    systems::draw_shaders(registry);

    systems::end_camera_mode(registry);

    // auto& shadow_map = registry.ctx().get<res::ShadowMap>();
    // DrawTextureRec(
    //     shadow_map.target.depth,
    //     Rectangle {0, 0, (float)shadow_map.target.depth.width, -(float)shadow_map.target.depth.height},
    //     Vector2 {0, 0},
    //     WHITE
    // );
}

} // namespace cfu
