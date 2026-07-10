#pragma once

#include <entt/entity/registry.hpp>
#include <entt_editor.hpp>
#include <raylib.h>

namespace cfu::comp {

struct Transform {
    Vector3 translation;
    Vector3 rotation;
    Vector3 scale;
};

template<>
void ComponentEditorWidget<Transform>(entt::registry& reg, entt::registry::entity_type e);

struct Position: Vector2 {};

template<>
void ComponentEditorWidget<Position>(entt::registry& reg, entt::registry::entity_type e);

struct Velocity: Vector2 {};

template<>
void ComponentEditorWidget<Velocity>(entt::registry& reg, entt::registry::entity_type e);

struct CircleCollider {
    float radius;
};

template<>
void ComponentEditorWidget<CircleCollider>(entt::registry& reg, entt::registry::entity_type e);

} // namespace cfu::comp
