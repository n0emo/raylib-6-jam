#include "./state.hpp"

#include <raylib.h>

#include "components.hpp"
#include "../background/components.hpp"
#include "../background/systems.hpp"
#include "../states.hpp"

namespace game {

auto MainMenuState::on_enter(entt::registry& registry) -> void {
    auto bg = registry.create();
    registry.emplace<components::BackgroundColor>(bg, GetColor(0xA4A4DAFF));
    registry.emplace<components::MainMenuTag>(bg);
}

auto MainMenuState::on_exit(entt::registry& registry) -> void {
    auto view = registry.view<components::MainMenuTag>();
    registry.destroy(view.begin(), view.end());
}

auto MainMenuState::update(entt::registry& registry) -> void {
    if (IsKeyPressed(KEY_SPACE)) {
        StateStack::get(registry).push(InGameState {});
    }
}

auto MainMenuState::draw(entt::registry& registry) -> void {
    systems::draw_background_color(registry);
};

} // namespace game
