#include "./components.hpp"

#include "../constants.hpp"
#include "../physics/components.hpp"
#include "../shaders/components.hpp"
#include "../solids/components.hpp"
#include "../vox/components.hpp"
#include "../vox/resource.hpp"
#include "../raymath.hpp"

namespace cfu::comp {

auto HeightMap::create(std::size_t tiles_x, std::size_t tiles_y) -> HeightMap {
    const auto width = tiles_x * TILE_SIZE;
    const auto height = tiles_y * TILE_SIZE;
    return HeightMap {
        .width = width,
        .height = height,
        .map = std::vector<float>(width * height),
    };
}

auto HeightMap::set_height_for_tile(gsl::index tile_x, gsl::index tile_y, float height) -> void {
    const auto size = float(TILE_SIZE);
    const auto region = Rectangle {
        .x = float(tile_x) * size,
        .y = float(tile_y) * size,
        .width = size,
        .height = size,
    };
    set_height_for_region(region, height);
}

auto HeightMap::set_height_for_region(Rectangle region, float height) -> void {
    for (auto x = gsl::index(region.x); x < gsl::index(region.x + region.width); x++) {
        for (auto y = gsl::index(region.y); y < gsl::index(region.y + region.height); y++) {
            if (x < 0 || x >= gsl::index(this->width)) continue;
            if (y < 0 || y >= gsl::index(this->height)) continue;
            map[y * width + x] = height;
        }
    }
}

auto HeightMap::get_height(float x, float y) const -> float {
    auto x_index = gsl::index(x);
    auto y_index = gsl::index(y);
    if (x_index < 0 || x_index >= gsl::index(this->width) || y_index < 0 || y_index >= gsl::index(this->height)) {
        return 0.0f;
    }

    return map[y_index * width + x_index];
}

auto create_tile(
    entt::registry& reg,
    entt::entity e,
    TileIndex index,
    res::TileKind kind,
    res::TileRotation rotation,
    res::vox::VoxelModelId model_id
) -> void {
    reg.emplace<comp::LevelTag>(e);
    reg.emplace<comp::LevelTile>(
        e,
        comp::LevelTile {
            .index = index,
            .kind = kind,
            .rotation = rotation,
        }
    );
    reg.emplace<comp::SolidMaterial>(e, WHITE);
    reg.emplace<comp::Transform>(
        e,
        comp::Transform {
            .translation = {},
            .rotation = {},
            .scale = Vector3One() * 4.0f,
        }
    );
    reg.emplace<comp::VoxelModel>(e, comp::VoxelModel::get(reg, model_id));
    update_tile_transform(reg, e);
}

auto update_tile_transform(entt::registry& reg, entt::entity e) -> void {
    const auto& tile = reg.get<comp::LevelTile>(e);
    auto& transform = reg.get<comp::Transform>(e);
    const auto position = Vector3(
        float(tile.index.x * TILE_SIZE) + TILE_SIZE * 0.5f,
        float(tile.index.y * TILE_SIZE),
        float(tile.index.z * TILE_SIZE) + TILE_SIZE * 0.5f
    );
    transform.translation = position;
}

auto create_prop(entt::registry& reg, entt::entity e, Vector3 position, res::vox::VoxelModelId model_id) -> void {
    create_prop(
        reg,
        e,
        comp::Transform {
            .translation = position,
            .rotation = Vector3Zero(),
            .scale = Vector3One() * 4.0f,
        },
        WHITE,
        model_id
    );
}

auto create_prop(entt::registry& reg, entt::entity e, Transform transform, Color tint, res::vox::VoxelModelId model_id)
    -> void {
    reg.emplace<comp::LevelTag>(e);
    reg.emplace<comp::LevelProp>(e);
    reg.emplace<comp::Transform>(e, transform);
    reg.emplace<comp::SolidMaterial>(e, tint);
    reg.emplace<comp::VoxelModel>(e, VoxelModel::get(reg, model_id));
}

auto create_level_light(entt::registry& reg, entt::entity e, Vector3 position, Color color) -> void {
    reg.emplace<comp::LevelTag>(e);
    comp::create_light(reg, e, LightType::Point, position, Vector3 {}, color);
}

auto deserialize_level(entt::registry& reg, res::level::LevelId id) -> void {
    auto level = reg.ctx().get<res::LevelCache>()[id]->level;
    auto id_e = reg.create();
    reg.emplace<comp::LevelTag>(id_e);
    reg.emplace<comp::CurrentLevel>(id_e, id);
    deserialize_sun(reg, level.sun);
    deserialize_tiles(reg, level.map);
    deserialize_props(reg, level.props);
    deserialize_lights(reg, level.lights);

    comp::set_lights_shader_locations(reg, comp::get_lighting_shader(reg));
}

auto serialize_level(entt::registry& reg) -> res::Level {
    return res::Level {
        .sun = serialize_sun(reg),
        .map = serialize_tiles(reg),
        .props = serialize_props(reg),
        .lights = serialize_lights(reg),
    };
}

auto deserialize_sun(entt::registry& reg, res::LevelSun sun) -> void {
    auto e = reg.create();
    reg.emplace<comp::LevelTag>(e);
    reg.emplace<comp::ShadowLightTag>(e);
    comp::create_light(reg, e, LightType::Directional, sun.direction * -1.0f, Vector3Zero(), sun.color);
}

auto serialize_sun(entt::registry& reg) -> res::LevelSun {
    auto e = reg.view<comp::ShadowLightTag, comp::Light>().back();
    if (e == entt::null) return {};
    auto l = reg.get<comp::Light>(e);
    return res::LevelSun {
        .direction = l.target - l.position,
        .color = l.color,
    };
}

auto deserialize_tiles(entt::registry& reg, const std::unordered_map<TileIndex, res::Tile>& tiles) -> void {
    for (const auto [index, tile] : tiles) {
        auto e = reg.create();
        create_tile(reg, e, index, tile.kind, tile.rotation, tile.model);
    }
}

auto serialize_tiles(entt::registry& reg) -> std::unordered_map<TileIndex, res::Tile> {
    auto result = std::unordered_map<TileIndex, res::Tile> {};
    auto view = reg.view<comp::LevelTile, comp::VoxelModel>();
    for (const auto [e, tile, model] : view.each()) {
        result.insert({
            tile.index,
            res::Tile {
                .kind = tile.kind,
                .rotation = tile.rotation,
                .model = model.id,
            },
        });
    }
    return result;
}

auto deserialize_props(entt::registry& reg, std::span<res::LevelProp> props) -> void {
    for (const auto& p : props) {
        auto e = reg.create();
        create_prop(reg, e, p.transform, p.tint, p.model);
    }
}

auto serialize_props(entt::registry& reg) -> std::vector<res::LevelProp> {
    auto result = std::vector<res::LevelProp> {};
    auto view = reg.view<comp::LevelProp, comp::Transform, comp::SolidMaterial, comp::VoxelModel>();
    for (auto [e, transform, material, model] : view.each()) {
        result.emplace_back(
            res::LevelProp {
                .transform = transform,
                .model = model.id,
                .tint = material.color,
            }
        );
    }
    return result;
}

auto deserialize_lights(entt::registry& reg, std::span<res::LevelLight> lights) -> void {
    for (const auto& l : lights) {
        auto e = reg.create();
        create_level_light(reg, e, l.position, l.color);
    }
}

auto serialize_lights(entt::registry& reg) -> std::vector<res::LevelLight> {
    auto result = std::vector<res::LevelLight> {};
    auto view = reg.view<comp::LevelTag, comp::Light>();
    for (auto [e, light] : view.each()) {
        if (light.type != LightType::Point) continue;
        result.emplace_back(
            res::LevelLight {
                .position = light.position,
                .color = light.color,
            }
        );
    }
    return result;
}

// auto create_map(entt::registry& registry, entt::entity entity, std::size_t tiles_x, std::size_t tiles_y) -> void {
//     registry.emplace<TileMap>(entity, tiles_x, tiles_y);
//     registry.emplace<HeightMap>(entity, HeightMap::create(tiles_x, tiles_y));
// }

// auto create_tile(
//     entt::registry& registry,
//     entt::entity entity,
//     TileKind kind,
//     TileRotation rotation,
//     gsl::index x,
//     gsl::index y,
//     gsl::index z
// ) -> void {
//     registry.emplace<Tile>(entity, kind, rotation);
//
//     auto model_id = res::vox::VoxelModelId {};
//     auto height = float {};
//     switch (kind) {
//         case TileKind::Floor:
//             model_id = res::vox::Floor;
//             height = TILE_SIZE * 0.5f;
//             break;
//         case TileKind::Wall:
//             model_id = res::vox::Wall;
//             height = TILE_SIZE;
//             break;
//     };
//
//     registry.emplace<Transform>(
//         entity,
//         Transform {
//             .translation =
//                 Vector3(float(x) * TILE_SIZE, float(y) * TILE_SIZE + height - TILE_SIZE * 0.5f, float(z) * TILE_SIZE),
//             .rotation = Vector3Zero(),
//             .scale = Vector3One() * 4.0f,
//         }
//     );
//
//     const auto model = registry.ctx().get<res::VoxelModelCache>()[model_id];
//     registry.emplace<VoxelModel>(entity, model);
//
//     auto height_map_entity = registry.view<HeightMap>().back();
//     if (height_map_entity != entt::null) {
//         auto& height_map = registry.get<HeightMap>(height_map_entity);
//         height_map.set_height_for_tile(x, z, float(y) * TILE_SIZE + height);
//     }
// }

} // namespace cfu::comp
