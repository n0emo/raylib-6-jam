#include "./systems.hpp"

#include "../raymath.hpp"
#include "../physics/components.hpp"
#include "../combat/components.hpp"
#include "../camera/components.hpp"
#include "../level/components.hpp"
#include "./components.hpp"

namespace cfu::systems {

auto update_player(entt::registry& registry) -> void {
    auto camera_entity = registry.view<Camera3D, comp::CameraOffset>().back();
    if (camera_entity == entt::null) return;
    const auto camera_offset = registry.get<comp::CameraOffset>(camera_entity).offset;

    auto view = registry.view<
        comp::Transform,
        comp::Position,
        comp::Velocity,
        comp::Grounded,
        const comp::MoveSpeed,
        const comp::Player>();

    for (auto [entity, transform, position, velocity, grounded, move_speed] : view.each()) {
        auto input2d = Vector2();
        if (IsKeyDown(KEY_A)) input2d.x -= 1.0f;
        if (IsKeyDown(KEY_D)) input2d.x += 1.0f;
        if (IsKeyDown(KEY_W)) input2d.y -= 1.0f;
        if (IsKeyDown(KEY_S)) input2d.y += 1.0f;

        transform.translation = Vector3(position.x, 0.0f, position.y);
        grounded.y = 0.0f;

        if (Vector2LengthSqr(input2d) == 0) {
            velocity = {};
            continue;
        };

        const auto offset_normalized = Vector2Normalize(Vector2(camera_offset.x, camera_offset.y));
        const auto angle = Vector2Angle(Vector2(0.0f, 1.0f), offset_normalized);
        input2d = Vector2Normalize(Vector2Rotate(input2d, angle));

        transform.rotation.y = Vector2Angle(Vector2(1.0f, 0.0f), Vector2(input2d.x, input2d.y * -1.0f));

        velocity = comp::Velocity(input2d * move_speed.value);
    }
}

} // namespace cfu::systems
