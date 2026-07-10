#include "./components.hpp"

#include <entt/entity/registry.hpp>

namespace cfu::comp {

auto get_default_shader(entt::registry& reg) -> Shader {
    return reg.ctx().get<comp::DefaultShader>().shader;
}

template<typename T>
static auto get_shader_checked(entt::registry& reg) -> Shader {
    if (!reg.ctx().contains<T>()) return get_default_shader(reg);
    auto shader = reg.ctx().get<T>().shader;
    if (!IsShaderValid(shader)) return get_depth_shader(reg);
    return shader;
}

auto get_depth_shader(entt::registry& reg) -> Shader {
    return get_shader_checked<comp::DepthShader>(reg);
}

auto get_lighting_shader(entt::registry& reg) -> Shader {
    return get_shader_checked<comp::LightingShader>(reg);
}

auto create_light(entt::registry& reg, entt::entity e, LightType type, Vector3 position, Vector3 target, Color color)
    -> void {
    auto lights_count = reg.view<Light>()->size();
    if (lights_count >= MAX_LIGHTS) {
        TraceLog(LOG_ERROR, "Max light count exceeded");
        return;
    }

    reg.emplace<Light>(
        e,
        Light {
            .type = type,
            .enabled = true,
            .position = position,
            .target = target,
            .color = color,
            .index = int(lights_count),
        }
    );

    // TODO: light removal logic
}

auto set_lights_shader_locations(entt::registry& reg, Shader shader) -> void {
    auto view = reg.view<Light>();
    for (auto [entity, light] : view.each()) {
        const auto index = light.index;

        if (const auto loc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", index)); loc != -1) {
            light.enabledLoc = loc;
        } else {
            TraceLog(LOG_ERROR, "Lighting shader: could not get location for lighs %d: %s", index, "enabled");
        }

        if (const auto loc = GetShaderLocation(shader, TextFormat("lights[%i].type", index)); loc != -1) {
            light.typeLoc = loc;
        } else {
            TraceLog(LOG_ERROR, "Lighting shader: could not get location for lighs %d: %s", index, "type");
        }

        if (const auto loc = GetShaderLocation(shader, TextFormat("lights[%i].position", index)); loc != -1) {
            light.positionLoc = loc;
        } else {
            TraceLog(LOG_ERROR, "Lighting shader: could not get location for lighs %d: %s", index, "position");
        }

        if (const auto loc = GetShaderLocation(shader, TextFormat("lights[%i].target", index)); loc != -1) {
            light.targetLoc = loc;
        } else {
            TraceLog(LOG_ERROR, "Lighting shader: could not get location for lighs %d: %s", index, "target");
        }

        if (const auto loc = GetShaderLocation(shader, TextFormat("lights[%i].color", index)); loc != -1) {
            light.colorLoc = loc;
        } else {
            TraceLog(LOG_ERROR, "Lighting shader: could not get location for lighs %d: %s", index, "color");
        }

        TraceLog(LOG_INFO, "Shader locations for light %d updated successfully", index);
    }
}

template<>
void ComponentEditorWidget<Light>(entt::registry& reg, entt::registry::entity_type e) {
    auto& light = reg.get<Light>(e);
    auto enabled = bool(light.enabled);
    ImGui::Checkbox("Enabled", &enabled);
    light.enabled = enabled;

    auto type = int(light.type);
    ImGui::SeparatorText("Light type");
    ImGui::RadioButton("Directional", &type, int(LightType::Directional));
    ImGui::RadioButton("Point", &type, int(LightType::Point));
    light.type = LightType(type);

    ImGui::SeparatorText("Position");
    ImGui::DragFloat("X##position", &light.position.x);
    ImGui::DragFloat("Y##position", &light.position.y);
    ImGui::DragFloat("Z##position", &light.position.z);

    ImGui::SeparatorText("Target");
    ImGui::DragFloat("X##target", &light.target.x);
    ImGui::DragFloat("Y##target", &light.target.y);
    ImGui::DragFloat("Z##target", &light.target.z);

    ImGui::SeparatorText("Color");
    auto color = ColorNormalize(light.color);
    ImGui::ColorPicker4("Color", reinterpret_cast<float *>(&color)); // NOLINT
    light.color = ColorFromNormalized(color);
}

} // namespace cfu::comp
