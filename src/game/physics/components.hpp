#pragma once

#include <entt/entity/registry.hpp>
#include <entt_editor.hpp>
#include <raylib.h>
#include <nlohmann/json_fwd.hpp>

void to_json(nlohmann::json& j, const Vector2& v);
void from_json(const nlohmann::json& j, Vector2& v);

void to_json(nlohmann::json& j, const Vector3& v);
void from_json(const nlohmann::json& j, Vector3& v);

void to_json(nlohmann::json& j, const Color& c);
void from_json(const nlohmann::json& j, Color& c);

namespace cfu::comp {

using json = nlohmann::json;

struct Transform {
    Vector3 translation;
    Vector3 rotation;
    Vector3 scale;
};

void to_json(json& j, const Transform& t);
void from_json(const json& j, Transform& t);

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
