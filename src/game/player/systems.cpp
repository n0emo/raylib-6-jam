#include "./systems.hpp"

#include "../physics/components.hpp"
#include "./components.hpp"

namespace game::systems {

auto update_player(entt::registry& registry) -> void {
    constexpr auto player_speed = 100.0f;
    const auto dt = GetFrameTime();

    auto view = registry.view<components::Position, components::Player>();
    for (auto [entity, position] : view.each()) {
        if (IsKeyDown(KEY_A)) position.vec.x -= player_speed * dt;
        if (IsKeyDown(KEY_D)) position.vec.x += player_speed * dt;
        if (IsKeyDown(KEY_W)) position.vec.y -= player_speed * dt;
        if (IsKeyDown(KEY_S)) position.vec.y += player_speed * dt;
    }
}

} // namespace game::systems
