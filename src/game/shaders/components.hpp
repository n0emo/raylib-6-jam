#pragma once

#include <entt/fwd.hpp>
#include <entt_editor.hpp>
#include <raylib.h>

namespace cfu::comp {

constexpr auto MAX_LIGHTS = 4;

struct DefaultShader {
    Shader shader;
};

auto get_default_shader(entt::registry& reg) -> Shader;

struct DepthShader {
    Shader shader;
};

auto get_depth_shader(entt::registry& reg) -> Shader;

struct LightingShader {
    Shader shader {};
    RenderTexture2D shadow_map {};

    int light_vp_loc = -1;
    int shadow_map_loc = -1;
    int shadow_map_resolution_loc = -1;
    int shadow_light_index_loc = -1;
};

auto get_lighting_shader(entt::registry& reg) -> Shader;

enum class LightType : int {
    Directional = 0,
    Point = 1,
};

struct Light {
    LightType type {};
    int enabled {};
    Vector3 position {};
    Vector3 target {};
    Color color {};

    int index = -1;
    int enabledLoc = -1;
    int typeLoc = -1;
    int positionLoc = -1;
    int targetLoc = -1;
    int colorLoc = -1;
};

auto create_light(entt::registry& reg, entt::entity e, LightType type, Vector3 position, Vector3 target, Color color)
    -> void;

auto set_lights_shader_locations(entt::registry& reg, Shader shader) -> void;

template<>
void ComponentEditorWidget<Light>(entt::registry& reg, entt::registry::entity_type e);

struct ShadowLightTag {};

} // namespace cfu::comp
