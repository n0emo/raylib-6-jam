#include "./systems.hpp"

#include <entt_editor.hpp>
#include <imgui.h>

#include "../background/components.hpp"
#include "../combat/components.hpp"
#include "../physics/components.hpp"
#include "../player/components.hpp"
#include "../shaders/components.hpp"
#include "../shapes/components.hpp"
#include "../solids/components.hpp"
#include "../level/components.hpp"
#include "../states.hpp"
#include "./components.hpp"

namespace cfu::systems {

auto setup_entt_editor(entt::registry& registry) -> void {
    auto editor = comp::Editor {};
    editor.show_window = false;
    editor.registerComponent<comp::BackgroundColor>("background::Color");
    editor.registerComponent<comp::Ball>("shapes::Ball");
    editor.registerComponent<comp::CircleCollider>("physics::CircleCollider");
    editor.registerComponent<comp::Cube>("solids::Cube");
    editor.registerComponent<comp::CurrentLevel>("level::CurrentLevel");
    editor.registerComponent<comp::CursorCube>("dev::CursorCube");
    editor.registerComponent<comp::Hp>("combat::Hp");
    editor.registerComponent<comp::LevelProp>("level::Prop");
    editor.registerComponent<comp::LevelTag>("level::Tag");
    editor.registerComponent<comp::Light>("shaders::Light");
    editor.registerComponent<comp::MeleeAttack>("combat::MeleeAttack");
    editor.registerComponent<comp::MoveSpeed>("combat::MoveSpeed");
    editor.registerComponent<comp::Player>("player::Player");
    editor.registerComponent<comp::Position>("physics::Position");
    editor.registerComponent<comp::ShadowLightTag>("shaders::ShadowLightTag");
    editor.registerComponent<comp::ShapeColor>("shapes::Color");
    editor.registerComponent<comp::SolidMaterial>("solids::Material");
    editor.registerComponent<comp::Transform>("physics::Transform");
    editor.registerComponent<comp::Velocity>("physics::Velocity");
    registry.ctx().insert_or_assign(editor);

    auto entity = comp::CurrentEntity {.entity = entt::null};
    registry.ctx().insert_or_assign(entity);
}

auto draw_entt_editor(entt::registry& registry) -> void {
    auto& entity = registry.ctx().get<comp::CurrentEntity&>().entity;
    auto& editor = registry.ctx().get<comp::Editor>();
    if (IsKeyPressed(KEY_F5)) editor.show_window = !editor.show_window;
    editor.renderSimpleCombo(registry, entity);
}

} // namespace cfu::systems
