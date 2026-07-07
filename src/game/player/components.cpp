#include "./components.hpp"

#include "../physics/components.hpp"
#include "../solids/components.hpp"

namespace cfu::components {

auto create_player(entt::registry& registry, entt::entity entity) -> void {
    registry.emplace<Player>(entity);
    registry.emplace<cfu::components::Transform>(
        entity,
        cfu::components::Transform {
            .translation = Vector3(0.0f, 10.0f, 0.0f),
            .rotation = Vector3(),
            .scale = Vector3(1.0f, 1.0f, 1.0f),
        }
    );
    registry.emplace<Cube>(entity, 20.0f, 20.0f, 20.0f);
    registry.emplace<SolidMaterial>(entity, RED);
}

} // namespace cfu::components
