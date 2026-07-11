#include "./systems.hpp"

#include <entt_editor.hpp>
#include <imgui.h>
#include <raylib.h>

#include "../constants.hpp"
#include "../level/components.hpp"
#include "../level/resource.hpp"
#include "../raymath.hpp"
#include "../states.hpp"
#include "./components.hpp"
#include "./utils.hpp"

namespace cfu::systems {

static auto get_cube_under_mouse(entt::registry& registry) -> std::tuple<entt::entity, Vector3> {
    // auto view = registry.view<const comp::Tile, const comp::Transform, const comp::Cube>();
    // for (auto [entity, tile, transform, cube] : view.each()) {
    //     const auto ray = dev::mouse_ray(registry);
    //     const auto size = cube.size * transform.scale;
    //     const auto min = transform.translation - size * 0.5f;
    //     auto max = min + size;
    //     auto box = BoundingBox {.min = min, .max = max};
    //     auto collision = GetRayCollisionBox(ray, box);
    //     if (collision.hit) {
    //         return {entity, collision.point};
    //         // current_entity.entity = entity;
    //         // editor.mode = comp::TileMapEditorMode::Free;
    //         break;
    //     }
    // }
    return {entt::null, Vector3()};
}

auto setup_tilemap_editor(entt::registry& registry) -> void {
    auto editor = comp::TileMapEditor {};
    registry.ctx().insert_or_assign(editor);
}

auto draw_tilemap_editor(entt::registry& registry) -> void {
    auto& editor = registry.ctx().get<comp::TileMapEditor>();
    if (IsKeyPressed(KEY_F6)) editor.show_window = !editor.show_window;
    if (!editor.show_window) return;

    auto tilemap_entity = registry.view<comp::TileMap>().back();
    if (tilemap_entity == entt::null) {
        editor.enabled = false;
        return;
    } else {
        editor.enabled = true;
    }

    auto& current_entity = registry.ctx().get<comp::CurrentEntity>();

    ImGui::Begin("Tilemap Editor");

    ImGui::SeparatorText("Mode");
    auto selected_mode = int(editor.mode);
    ImGui::RadioButton("Free", &selected_mode, int(comp::TileMapEditorMode::Free));
    ImGui::RadioButton("Selecting", &selected_mode, int(comp::TileMapEditorMode::Selecting));
    ImGui::RadioButton("Placing", &selected_mode, int(comp::TileMapEditorMode::Placing));
    ImGui::RadioButton("Erasing", &selected_mode, int(comp::TileMapEditorMode::Erasing));
    editor.mode = comp::TileMapEditorMode(selected_mode);

    auto selected_tile = int(editor.current_tile);
    ImGui::SeparatorText("Tile");
    ImGui::RadioButton("Floor", &selected_tile, int(res::TileKind::Floor));
    ImGui::RadioButton("Wall", &selected_tile, int(res::TileKind::Wall));
    editor.current_tile = res::TileKind(selected_tile);

    ImGui::End();

    switch (editor.mode) {
        case comp::TileMapEditorMode::Free: {
            break;
        }
        case comp::TileMapEditorMode::Selecting: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (auto [entity, position] = get_cube_under_mouse(registry); entity != entt::null) {
                    current_entity.entity = entity;
                    editor.mode = comp::TileMapEditorMode::Free;
                }
            }
            break;
        }
        case comp::TileMapEditorMode::Placing: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (auto [entity, position] = get_cube_under_mouse(registry); entity != entt::null) {
                    // const auto x = gsl::index(position.x / TILE_SIZE);
                    // const auto y = gsl::index(position.y / TILE_SIZE);
                    // const auto z = gsl::index(position.z / TILE_SIZE);
                    // auto new_entity = registry.create();
                    // TODO:: rotation
                    // comp::create_tile(registry, new_entity, editor.current_tile, comp::TileRotation::None, x, y, z);
                    // registry.emplace<comp::InGameTag>(new_entity);
                } else {
                    const auto ray = dev::mouse_ray(registry);
                    const auto size = float(TILE_SIZE);

                    auto collision = GetRayCollisionBox(
                        ray,
                        BoundingBox {
                            .min = Vector3(0.0f, 0.0f, 0.0f),
                            // TODO: get tilemap size
                            .max = Vector3(128.0f * size, size, 128.0f * size),
                        }
                    );

                    if (collision.hit) {
                        // const auto x = gsl::index(collision.point.x / TILE_SIZE);
                        // const auto y = 0;
                        // const auto z = gsl::index(collision.point.z / TILE_SIZE);
                        // auto new_entity = registry.create();
                        // comp::create_tile(registry, new_entity, editor.current_tile, comp::TileRotation::None, x, y, z);
                        // registry.emplace<comp::InGameTag>(new_entity);
                    }
                }
            }
            break;
        }
        case comp::TileMapEditorMode::Erasing: {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (auto [entity, position] = get_cube_under_mouse(registry); entity != entt::null) {
                    // TODO: Update height map
                    registry.destroy(entity);
                }
            }
            break;
            break;
        }
    }
}

} // namespace cfu::systems
