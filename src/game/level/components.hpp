#pragma once

#include <vector>

#include <raylib.h>
#include <entt/entity/registry.hpp>
#include <gsl/gsl>
#include <nlohmann/json.hpp>

#include "../physics/components.hpp"
#include "../vox/resource.hpp"
#include "./resource.hpp"

namespace cfu::comp {

struct TileMap {};

struct HeightMap {
    std::size_t width;
    std::size_t height;
    std::vector<float> map;

    static auto create(std::size_t width, std::size_t height) -> HeightMap;

    auto set_height_for_tile(gsl::index tile_x, gsl::index tile_y, float height) -> void;
    auto set_height_for_region(Rectangle region, float height) -> void;
    [[nodiscard]] auto get_height(float x, float y) const -> float;
};

struct Grounded {
    float y;
};

struct LevelTag {};

struct CurrentLevel {
    res::level::LevelId id;
};

struct LevelTile {
    TileIndex index {};
    res::TileKind kind {};
    res::TileRotation rotation {};
};

struct LevelProp {};

auto create_tile(
    entt::registry& reg,
    entt::entity e,
    TileIndex index,
    res::TileKind kind,
    res::TileRotation rotation,
    res::vox::VoxelModelId model_id
) -> void;
auto update_tile_transform(entt::registry& reg, entt::entity e) -> void;

auto create_prop(entt::registry& reg, entt::entity e, Vector3 position, res::vox::VoxelModelId model_id) -> void;
auto create_prop(entt::registry& reg, entt::entity e, Transform transform, Color tint, res::vox::VoxelModelId model_id)
    -> void;
auto create_level_light(entt::registry& reg, entt::entity e, Vector3 position, Color color) -> void;

auto deserialize_level(entt::registry& reg, res::level::LevelId id) -> void;
auto serialize_level(entt::registry& reg) -> res::Level;
auto deserialize_sun(entt::registry& reg, res::LevelSun sun) -> void;
auto serialize_sun(entt::registry& reg) -> res::LevelSun;
auto deserialize_tiles(entt::registry& reg, const std::unordered_map<TileIndex, res::Tile>& tiles) -> void;
auto serialize_tiles(entt::registry& reg) -> std::unordered_map<TileIndex, res::Tile>;
auto deserialize_props(entt::registry& reg, std::span<res::LevelProp> props) -> void;
auto serialize_props(entt::registry& reg) -> std::vector<res::LevelProp>;
auto deserialize_lights(entt::registry& reg, std::span<res::LevelLight> lights) -> void;
auto serialize_lights(entt::registry& reg) -> std::vector<res::LevelLight>;

// auto create_map(entt::registry& registry, entt::entity entity, std::size_t tiles_x, std::size_t tiles_y) -> void;
// auto create_tile(
//     entt::registry& registry,
//     entt::entity entity,
//     TileKind kind,
//     TileRotation rotation,
//     gsl::index x,
//     gsl::index y,
//     gsl::index z
// ) -> void;

} // namespace cfu::comp
