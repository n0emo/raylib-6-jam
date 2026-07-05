#include "./components.hpp"

#include "../shapes/components.hpp"
#include "../physics/components.hpp"

namespace game::components {

auto create_player(entt::registry& registry) -> entt::entity {
    auto e = registry.create();
    registry.emplace<Player>(e);
    registry.emplace<Position>(e, 200.0f, 200.0f);
    registry.emplace<Velocity>(e);
    registry.emplace<Ball>(e, 10.0f);
    registry.emplace<ShapeColor>(e, RED);
    return e;
}

} // namespace game::components
