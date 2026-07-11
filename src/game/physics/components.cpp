#include "./components.hpp"

#include <nlohmann/json.hpp>

#include "../raymath.hpp"

void to_json(nlohmann::json& j, const Vector2& v) {
    j = nlohmann::json {v.x, v.y};
}

void from_json(const nlohmann::json& j, Vector2& v) {
    v = {};
    if (!j.is_array()) return;
    if (j.size() != 2) return;
    if (!j[0].is_number()) return;
    if (!j[1].is_number()) return;
    v.x = j[0];
    v.y = j[1];
}

void to_json(nlohmann::json& j, const Vector3& v) {
    j = nlohmann::json {v.x, v.y, v.z};
}

void from_json(const nlohmann::json& j, Vector3& v) {
    v = {};
    if (!j.is_array()) return;
    if (j.size() != 3) return;
    if (!j[0].is_number()) return;
    if (!j[1].is_number()) return;
    if (!j[2].is_number()) return;
    v.x = j[0];
    v.y = j[1];
    v.z = j[2];
}

void to_json(nlohmann::json& j, const Color& c) {
    j = nlohmann::json {c.r, c.g, c.b, c.a};
}

void from_json(const nlohmann::json& j, Color& c) {
    c = {};
    if (!j.is_array()) return;
    if (j.size() != 4) return;
    if (!j[0].is_number()) return;
    if (!j[1].is_number()) return;
    if (!j[2].is_number()) return;
    if (!j[3].is_number()) return;
    c.r = j[0];
    c.g = j[1];
    c.b = j[2];
    c.a = j[3];
}

namespace cfu::comp {

void to_json(json& j, const Transform& t) {
    j = json {
        {"translation", t.translation},
        {"rotation", t.rotation},
        {"scale", t.scale},
    };
}

void from_json(const json& j, Transform& t) {
    t.translation = j.value("translation", Vector3 {});
    t.rotation = j.value("rotation", Vector3 {});
    t.scale = j.value("scale", Vector3(1.0f, 1.0f, 1.0f));
}

template<>
void ComponentEditorWidget<Transform>(entt::registry& reg, entt::registry::entity_type e) {
    auto& transform = reg.get<Transform>(e);
    ImGui::SeparatorText("Translation");
    ImGui::DragFloat("X##Translation", &transform.translation.x, 1.0f);
    ImGui::DragFloat("Y##Translation", &transform.translation.y, 1.0f);
    ImGui::DragFloat("Z##Translation", &transform.translation.z, 1.0f);

    ImGui::SeparatorText("Rotation");
    Vector3 rotation = transform.rotation * 180.0f / M_PI;
    constexpr auto min = -180.0f;
    constexpr auto max = 180.0f;
    constexpr auto speed = 1.0f;
    constexpr auto flags = ImGuiSliderFlags_WrapAround;
    constexpr auto fmt = "%.0f";
    ImGui::DragFloat("X##Rotation", &rotation.x, speed, min, max, fmt, flags);
    ImGui::DragFloat("Y##Rotation", &rotation.y, speed, min, max, fmt, flags);
    ImGui::DragFloat("Z##Rotation", &rotation.z, speed, min, max, fmt, flags);
    transform.rotation = rotation / 180.0f * M_PI;

    ImGui::SeparatorText("Scale");
    ImGui::DragFloat("X##Scale", &transform.scale.x, 0.01f);
    ImGui::DragFloat("Y##Scale", &transform.scale.y, 0.01f);
    ImGui::DragFloat("Z##Scale", &transform.scale.z, 0.01f);
}

template<>
void ComponentEditorWidget<Position>(entt::registry& reg, entt::registry::entity_type e) {
    auto& position = reg.get<Position>(e);
    ImGui::DragFloat("X", &position.x);
    ImGui::DragFloat("Y", &position.y);
}

template<>
void ComponentEditorWidget<Velocity>(entt::registry& reg, entt::registry::entity_type e) {
    auto& velocity = reg.get<Velocity>(e);
    ImGui::DragFloat("X", &velocity.x);
    ImGui::DragFloat("Y", &velocity.y);
}

template<>
void ComponentEditorWidget<CircleCollider>(entt::registry& reg, entt::registry::entity_type e) {
    auto& collider = reg.get<CircleCollider>(e);
    ImGui::DragFloat("Radius", &collider.radius, 1.0f, 0.0f, 999.0f, "%.0f");
}

} // namespace cfu::comp
