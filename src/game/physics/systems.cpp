#include "./systems.hpp"

#include "../tilemap/components.hpp"
#include "../raymath.hpp"
#include "./components.hpp"

namespace cfu::systems {

auto setup_physics(entt::registry& registry) -> void {
    (void)registry;
}

auto update_physics(entt::registry& registry) -> void {
    update_physics_velocity(registry);
}

auto update_physics_velocity(entt::registry& registry) -> void {
    auto view = registry.view<comp::Position, const comp::Velocity>();
    const auto dt = GetFrameTime();
    for (auto [entity, position, velocity] : view.each()) {
        position += velocity * dt;
    }
}

auto draw_physics(entt::registry& registry) -> void {
    draw_physics_velocity(registry);
    draw_physics_circle_colliders(registry);
}

auto draw_physics_velocity(entt::registry& registry) -> void {
    const auto map_entity = registry.view<const comp::HeightMap>().back();
    if (map_entity == entt::null) return;
    const auto& map = registry.get<const comp::HeightMap>(map_entity);

    auto view = registry.view<const comp::Velocity, const comp::Position>();
    for (auto [entity, velocity, position] : view.each()) {
        const auto x = position.x;
        const auto z = position.y;
        const auto y = map.get_height(x, z) + 1.0f;
        const auto begin = Vector3(x, y, z);
        const auto end = Vector3(x + velocity.x * 0.1f, y, z + velocity.y * 0.1f);
        DrawLine3D(begin, end, GREEN);
    }
}

auto draw_physics_circle_colliders(entt::registry& registry) -> void {
    const auto map_entity = registry.view<const comp::HeightMap>().back();
    if (map_entity == entt::null) return;
    const auto& map = registry.get<const comp::HeightMap>(map_entity);

    auto view = registry.view<const comp::CircleCollider, const comp::Position>();
    for (auto [entity, collider, position] : view.each()) {
        const auto x = position.x;
        const auto z = position.y;
        const auto y = map.get_height(x, z) + 1.0f;
        const auto pos = Vector3(x, y, z);
        const auto rotation_axis = Vector3(1.0f, 0.0f, 0.0f);
        const auto rotation_angle = 90.0f;
        const auto color = GREEN;
        DrawCircle3D(pos, 0.5f, rotation_axis, rotation_angle, color);
        DrawCircle3D(pos, collider.radius, rotation_axis, rotation_angle, color);
    }
}

} // namespace cfu::systems
