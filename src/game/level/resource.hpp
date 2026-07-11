#pragma once

#include <vector>
#include <array>

#include <raylib.h>
#include <nlohmann/json_fwd.hpp>
#include <entt/fwd.hpp>

#include "../physics/components.hpp"
#include "../vox/resource.hpp"
#include "./tile_index.hpp"

namespace cfu::res {

using json = nlohmann::json;

namespace level {

#define RES_LEVEL_ID_LIST                                                                                              \
    CFU_X(Level1, "level-1.json")                                                                                      \
    CFU_X(Level2, "level-2.json")                                                                                      \
    CFU_X(Level3, "level-3.json")                                                                                      \
    CFU_X(Level4, "level-4.json")                                                                                      \
    CFU_X(Level5, "level-5.json")                                                                                      \
    CFU_X(Level6, "level-6.json")                                                                                      \
    CFU_X(Level7, "level-7.json")                                                                                      \
    CFU_X(Level8, "level-8.json")                                                                                      \
    CFU_X(Level9, "level-9.json")                                                                                      \
    CFU_X(Level10, "level-10.json")

    enum LevelId {

#define CFU_X(name, ...) name,
        RES_LEVEL_ID_LIST
#undef CFU_X

    };

} // namespace level

constexpr auto LEVELS = std::array {

#define CFU_X(name, ...) level::name,
    RES_LEVEL_ID_LIST
#undef CFU_X

};

#define TILE_KIND_LIST                                                                                                 \
    CFU_X(Floor, "floor")                                                                                              \
    CFU_X(Wall, "wall")                                                                                                \
    CFU_X(Stair, "stair")

enum class TileKind {
    #define CFU_X(name, ...) name,
    TILE_KIND_LIST
    #undef CFU_X
};

auto tile_kind_to_string(TileKind kind) -> gsl::czstring;
auto tile_kind_from_string(std::string_view string) -> TileKind;

#define TILE_ROTATION_LIST                                                                                             \
    CFU_X(None, "none")                                                                                                \
    CFU_X(Deg90, "90")                                                                                                 \
    CFU_X(Deg180, "180")                                                                                               \
    CFU_X(Deg270, "270")

enum class TileRotation {
    #define CFU_X(name, ...) name,
    TILE_ROTATION_LIST
    #undef CFU_X
};

auto tile_rotation_to_string(TileRotation rot) -> gsl::czstring;
auto tile_rotation_from_string(std::string_view str) -> TileRotation;

struct Tile {
    TileKind kind = {};
    TileRotation rotation = {};
    vox::VoxelModelId model = {vox::Error};
};

void to_json(json& j, const Tile& t);
void from_json(const json& j, Tile& t);

struct LevelLight {
    Vector3 position = {};
    Color color = {};
};

void to_json(json& j, const LevelLight& l);
void from_json(const json& j, LevelLight& l);

struct LevelProp {
    comp::Transform transform = {};
    vox::VoxelModelId model = {vox::Error};
    Color tint = WHITE;
};

void to_json(json& j, const LevelProp& p);
void from_json(const json& j, LevelProp& p);

struct LevelSun {
    Vector3 direction;
    Color color;
};

void to_json(json& j, const LevelSun& s);
void from_json(const json& j, LevelSun& s);

struct Level {
    LevelSun sun;
    std::unordered_map<TileIndex, Tile> map = {};
    std::vector<LevelProp> props = {};
    std::vector<LevelLight> lights = {};
};

void to_json(json& j, const Level& l);
void from_json(const json& j, Level& l);

struct LevelResource {
    Level level;

    LevelResource(std::string_view path);
};

using LevelCache = entt::resource_cache<LevelResource>;

} // namespace cfu::res
