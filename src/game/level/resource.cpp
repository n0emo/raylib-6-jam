#include "./resource.hpp"

#include <algorithm>
#include <format>
#include <fstream>

#include <nlohmann/json.hpp>

namespace cfu::res {

auto tile_kind_to_string(TileKind kind) -> gsl::czstring {
    switch (kind) {
    #define CFU_X(name, str)                                                                                           \
    case TileKind::name: {                                                                                             \
        return str;                                                                                                    \
    }
        TILE_KIND_LIST
    #undef CFU_X
        default:
            throw std::range_error(std::format("Invalid TileKind: {}", int(kind)));
    }
}

auto tile_kind_from_string(std::string_view string) -> TileKind {
    #define CFU_X(name, str)                                                                                           \
    if ((str) == string) return TileKind::name;
    TILE_KIND_LIST
    #undef CFU_X
    throw std::range_error(std::format("Invalid TileKind: '{}'", string));
}

auto tile_rotation_to_string(TileRotation rot) -> gsl::czstring {
    switch (rot) {
    #define CFU_X(name, str)                                                                                           \
    case TileRotation::name: {                                                                                         \
        return str;                                                                                                    \
    }
        TILE_ROTATION_LIST
    #undef CFU_X
        default:
            throw std::range_error(std::format("Invalid TileRotation: {}", int(rot)));
    }
}

auto tile_rotation_from_string(std::string_view string) -> TileRotation {
    #define CFU_X(name, str)                                                                                           \
    if ((str) == string) return TileRotation::name;
    TILE_ROTATION_LIST
    #undef CFU_X
    throw std::range_error(std::format("Invalid TileRotation: '{}'", string));
}

void to_json(json& j, const Tile& t) {
    j = json {
        {"kind", tile_kind_to_string(t.kind)},
        {"rotation", tile_rotation_to_string(t.rotation)},
        {"model", model_id_to_string(t.model)},
    };
}

void from_json(const json& j, Tile& t) {
    t.model = model_id_from_string(j.value("model", "error.vox"));
    t.kind = tile_kind_from_string(j.value("kind", "wall"));
    t.rotation = tile_rotation_from_string(j.value("rotation", "none"));
}

void to_json(json& j, const LevelLight& l) {
    j = json {
        {"position", l.position},
        {"color", l.color},
    };
}

void from_json(const json& j, LevelLight& l) {
    l.position = j.value("position", Vector3 {});
    l.color = j.value("color", Color {});
}

void to_json(json& j, const LevelProp& p) {
    j = json {
        {"transform", p.transform},
        {"model", model_id_to_string(p.model)},
        {"tint", p.tint},
    };
}

void from_json(const json& j, LevelProp& p) {
    p.transform = j.value("transform", comp::Transform {});
    p.model = model_id_from_string(j.value("model", "error.vox"));
    p.tint = j.value("tint", WHITE);
}

void to_json(json& j, const LevelSun& s) {
    j = json {
        {"direction", s.direction},
        {"color", s.color},
    };
}

void from_json(const json& j, LevelSun& s) {
    s.direction = j.value("direction", Vector3One());
    s.color = j.value("color", GRAY);
}

void to_json(json& j, const Level& l) {
    auto tiles = std::vector<std::pair<TileIndex, Tile>>(l.map.begin(), l.map.end());
    std::ranges::sort(tiles, [](auto a, auto b) -> bool {
        return a.first.x < b.first.x || a.first.y < b.first.y || a.first.z < b.first.z;
    });
    auto tile_array = std::vector<json> {};
    std::ranges::transform(tiles, std::back_inserter(tile_array), [](auto v) -> json {
        return json {
            {"x", v.first.x},
            {"y", v.first.y},
            {"z", v.first.z},
            {"kind", tile_kind_to_string(v.second.kind)},
            {"rotation", tile_rotation_to_string(v.second.rotation)},
            {"model", model_id_to_string(v.second.model)},
        };
    });

    j = json {
        {"sun", l.sun},
        {"tiles", tile_array},
        {"props", l.props},
        {"lights", l.lights},
    };
}

void from_json(const json& j, Level& l) {
    l.sun = j.value("sun", LevelSun {});
    l.props = j.value("props", std::vector<LevelProp> {});
    l.lights = j.value("lights", std::vector<LevelLight> {});

    l.map.clear();
    for (const auto& t : j.value("tiles", std::vector<json> {})) {
        try {
            TileIndex idx {
                .x = t.value("x", gsl::index {}),
                .y = t.value("y", gsl::index {}),
                .z = t.value("z", gsl::index {}),
            };
            Tile tile {
                .kind = tile_kind_from_string(t.value("kind", std::string {})),
                .rotation = tile_rotation_from_string(t.value("rotation", std::string {})),
                .model = model_id_from_string(t.value("model", std::string {})),
            };
            l.map.emplace(idx, tile);
        } catch (const std::exception& e) {
            TraceLog(LOG_WARNING, "Invalid tile: %s", e.what());
        }
    }
}

LevelResource::LevelResource(std::string_view path) {
    auto file = std::ifstream {std::format("assets/data/{}", path)};
    auto j = json {};
    file >> j;
    this->level = j;
    TraceLog(LOG_INFO, "Loaded level `%.*s`", int(path.size()), path.data());
}

} // namespace cfu::res
