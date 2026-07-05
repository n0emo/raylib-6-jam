#include "./state.hpp"

#include <raylib.h>

#include "../player/systems.hpp"
#include "../player/components.hpp"
#include "../shapes/systems.hpp"
#include "./components.hpp"

namespace game {

auto InGameState::on_enter(entt::registry& registry) -> void {
    auto e = components::create_player(registry);
    registry.emplace<components::InGameTag>(e);
}

auto InGameState::on_exit(entt::registry& registry) -> void {
    auto view = registry.view<components::InGameTag>();
    registry.destroy(view.begin(), view.end());
}

auto InGameState::update(entt::registry& registry) -> void {
    systems::update_player(registry);
}

auto InGameState::draw(entt::registry& registry) -> void {
    systems::draw_shapes(registry);
    // ClearBackground(RED);
}

} // namespace game
