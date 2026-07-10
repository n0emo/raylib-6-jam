#include "./systems.hpp"

#include <raylib.h>
#include <rlgl.h>
#include <gsl/gsl>

#include "../data/components.hpp"
#include "../raymath.hpp"
#include "./components.hpp"

namespace cfu::systems {

template<typename F>
static auto with_location(Shader shader, gsl::czstring name, gsl::czstring context, F f) -> bool {
    if (const auto loc = GetShaderLocation(shader, name); loc != -1) {
        f(loc);
        return true;
    } else {
        TraceLog(LOG_ERROR, "%s: could not get shader location `%s`", context, name);
        return false;
    }
}

static auto update_light(comp::Light& light, Shader shader) {
    // Send to shader light enabled state and type
    if (light.enabledLoc != -1) SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    if (light.typeLoc != -1) SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

    // Send to shader light position values
    if (light.positionLoc != -1) {
        const auto position = std::array {light.position.x, light.position.y, light.position.z};
        SetShaderValue(shader, light.positionLoc, position.data(), SHADER_UNIFORM_VEC3);
    }

    // Send to shader light target position values
    if (light.targetLoc != -1) {
        const auto target = std::array {light.target.x, light.target.y, light.target.z};
        SetShaderValue(shader, light.targetLoc, target.data(), SHADER_UNIFORM_VEC3);
    }

    // Send to shader light color values
    if (light.colorLoc != -1) {
        const auto color = std::array {
            (float)light.color.r / (float)255,
            (float)light.color.g / (float)255,
            (float)light.color.b / (float)255,
            (float)light.color.a / (float)255
        };
        SetShaderValue(shader, light.colorLoc, color.data(), SHADER_UNIFORM_VEC4);
    }
}

void disable_lights(entt::registry& registry, entt::entity entity) {
    auto shader = comp::get_lighting_shader(registry);
    auto& light = registry.get<comp::Light>(entity);
    light.enabled = 0;
    update_light(light, shader);
}

auto setup_shaders(entt::registry& registry) -> void {
    registry.on_destroy<comp::Light>().connect<&disable_lights>();
}

auto reload_shaders(entt::registry& registry) -> void {
    reload_default_shader(registry);
    reload_depth_shader(registry);
    reload_lighting_shader(registry);
}

auto reload_default_shader(entt::registry& registry) -> void {
    auto shader = Shader {
        .id = rlGetShaderIdDefault(),
        .locs = rlGetShaderLocsDefault(),
    };

    registry.ctx().insert_or_assign(comp::DefaultShader {.shader = shader});
}

auto reload_depth_shader(entt::registry& registry) -> void {
    auto shader = LoadShader("assets/shaders/depth.vs", "assets/shaders/depth.fs");
    if (!IsShaderValid(shader)) return;

    if (registry.ctx().contains<comp::DepthShader>()) {
        UnloadShader(registry.ctx().get<comp::DepthShader>().shader);
        registry.ctx().erase<comp::DepthShader>();
    }

    auto depth_shader = comp::DepthShader {
        .shader = shader,
    };

    TraceLog(LOG_INFO, "Depth shader loaded successfully");
    registry.ctx().insert_or_assign(depth_shader);
}

auto reload_lighting_shader(entt::registry& registry) -> void {
    auto shader = LoadShader("assets/shaders/lighting.vs", "assets/shaders/lighting.fs");
    if (!IsShaderValid(shader)) return;

    auto lighting_shader = comp::LightingShader {.shader = shader};

    if (const auto loc = GetShaderLocation(shader, "mvp"); loc != -1) {
        lighting_shader.shader.locs[SHADER_LOC_MATRIX_VIEW] = loc;
    } else {
        TraceLog(LOG_ERROR, "Lighting shader: could not get location of `mvp`");
        return;
    }

    if (const auto loc = GetShaderLocation(shader, "matModel"); loc != -1) {
        lighting_shader.shader.locs[SHADER_LOC_MATRIX_MODEL] = loc;
    } else {
        TraceLog(LOG_ERROR, "Lighting shader: could not get location of `matModel`");
        return;
    }

    if (const auto loc = GetShaderLocation(shader, "ambient"); loc != -1) {
        lighting_shader.shader.locs[SHADER_LOC_COLOR_AMBIENT] = loc;
    } else {
        TraceLog(LOG_ERROR, "Lighting shader: could not get location of `ambient`");
        return;
    }

    if (const auto loc = GetShaderLocation(shader, "viewPos"); loc != -1) {
        lighting_shader.shader.locs[SHADER_LOC_VECTOR_VIEW] = loc;
    } else {
        TraceLog(LOG_ERROR, "Lighting shader: could not get location of `viewPos`");
        return;
    }

    if (registry.ctx().contains<comp::LightingShader>()) {
        UnloadShader(registry.ctx().get<comp::LightingShader>().shader);
        registry.ctx().erase<comp::LightingShader>();
    }

    TraceLog(LOG_INFO, "Lighting shader loaded successfully");
    registry.ctx().insert_or_assign(lighting_shader);
}

auto update_shaders(entt::registry& registry) -> void {
    update_lighting_shader(registry);
}

auto update_lighting_shader(entt::registry& registry) -> void {
    auto& d = registry.ctx().get<comp::GameData>();

    auto& lighting_shader = registry.ctx().get<comp::LightingShader>();
    const auto ambient_color = std::array {
        (float)d.balance.camera.ambient.r / (float)255,
        (float)d.balance.camera.ambient.g / (float)255,
        (float)d.balance.camera.ambient.b / (float)255,
        (float)d.balance.camera.ambient.a / (float)255
    };
    SetShaderValue(
        lighting_shader.shader,
        lighting_shader.shader.locs[SHADER_LOC_COLOR_AMBIENT],
        ambient_color.data(),
        SHADER_UNIFORM_VEC4
    );

    auto& shader = lighting_shader.shader;
    auto view = registry.view<comp::Light>();
    for (auto [entity, light] : view.each()) {
        update_light(light, shader);
    }
}

auto update_shadow_map(entt::registry&) -> void {
}

auto draw_shaders(entt::registry& registry) -> void {
    auto view = registry.view<comp::Light>();
    for (auto [entity, light] : view.each()) {
        DrawSphere(light.position, 2.0f, light.color);
    }
}

} // namespace cfu::systems
