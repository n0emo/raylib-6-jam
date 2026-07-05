#include "./systems.hpp"

#include "../physics/components.hpp"
#include "./components.hpp"

namespace game::systems {

auto draw_shapes(entt::registry& registry) -> void {
    auto view = registry.view<const components::Ball, const components::Position, const components::ShapeColor>();
    for (auto [entity, ball, position, color] : view.each()) {
        DrawCircleV(position.vec, ball.radius, color.color);
    }
}

} // namespace game::systems
