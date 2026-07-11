#pragma once

#include <span>
#include <string>
#include <expected>
#include <format>
#include <cmath>

#include "../solids/components.hpp"
#include "../constants.hpp"
#include "./components.hpp"

namespace cfu::dev {

using Args = std::span<std::string_view>;

template<typename T, typename E = std::string>
using Result = std::expected<T, E>;

using XYZ = std::tuple<int, int, int>;

using XYZRegion = std::pair<XYZ, XYZ>;

enum class CommandMode {
    Commit,
    Preview,
};

auto handle_command(entt::registry& registry, CommandMode mode, std::string_view command) -> std::string;
auto cmd_level(entt::registry& registry, CommandMode mode, Args args) -> std::string;
auto cmd_state(entt::registry& registry, CommandMode mode, Args args) -> std::string;

namespace detail {

    using namespace std;

    static inline auto xyz_to_vec(XYZ xyz) -> Vector3 {
        const auto [x, y, z] = xyz;
        return Vector3(float(x), float(y), float(z));
    }

    static inline auto parse_xyz(Args args) -> Result<XYZ> {
        if (args.size() != 3) {
            return unexpected(format("ERROR: Incorrect number of args\nNeed 3, got {}\n", args.size()));
        }

        auto x = int {}, y = int {}, z = int {};

        if (auto [ptr, ec] = from_chars(args[0].begin(), args[0].end(), x); ec != errc() || ptr != args[0].end())
            return unexpected(format("ERROR: Could not parse `{}` as int`\n", args[0]));
        if (auto [ptr, ec] = from_chars(args[1].begin(), args[1].end(), y); ec != errc() || ptr != args[1].end())
            return unexpected(format("ERROR: Could not parse `{}` as int`\n", args[1]));
        if (auto [ptr, ec] = from_chars(args[2].begin(), args[2].end(), z); ec != errc() || ptr != args[2].end())
            return unexpected(format("ERROR: Could not parse `{}` as int`\n", args[2]));

        return XYZ {x, y, z};
    }

    static inline auto parse_xyz_region(span<string_view> args) -> expected<XYZRegion, string> {
        if (args.size() != 6) {
            return unexpected(format("ERROR: Incorrect number of args\nNeed 3, got {}\n", args.size()));
        }

        auto x1 = int {}, y1 = int {}, z1 = int {};
        auto x2 = int {}, y2 = int {}, z2 = int {};

        if (auto [ptr, ec] = from_chars(args[0].begin(), args[0].end(), x1); ec != errc() || ptr != args[0].end())
            return unexpected(format("ERROR: Could not parse `{}` as int`\n", args[0]));
        if (auto [ptr, ec] = from_chars(args[1].begin(), args[1].end(), y1); ec != errc() || ptr != args[1].end())
            return unexpected(format("ERROR: Could not parse `{}` as int`\n", args[1]));
        if (auto [ptr, ec] = from_chars(args[2].begin(), args[2].end(), z1); ec != errc() || ptr != args[2].end())
            return unexpected(format("ERROR: Could not parse `{}` as int`\n", args[2]));
        if (auto [ptr, ec] = from_chars(args[3].begin(), args[3].end(), x2); ec != errc() || ptr != args[3].end())
            return unexpected(format("ERROR: Could not parse `{}` as int`\n", args[3]));
        if (auto [ptr, ec] = from_chars(args[4].begin(), args[4].end(), y2); ec != errc() || ptr != args[4].end())
            return unexpected(format("ERROR: Could not parse `{}` as int`\n", args[4]));
        if (auto [ptr, ec] = from_chars(args[5].begin(), args[5].end(), z2); ec != errc() || ptr != args[5].end())
            return unexpected(format("ERROR: Could not parse `{}` as int`\n", args[5]));

        return XYZRegion {XYZ {x1, y1, z1}, XYZ {x2, y2, z2}};
    }

    static inline auto set_cursor(entt::registry& registry, Vector3 position, Vector3 size, Color color) {
        auto cursor = registry.view<comp::CursorCube>().back();
        auto& transform = registry.get<comp::Transform>(cursor);
        transform.translation = position * TILE_SIZE;
        auto& cube = registry.get<comp::Cube>(cursor);
        cube.size = size * TILE_SIZE;
        auto& mat = registry.get<comp::SolidMaterial>(cursor);
        const auto alpha = (sinf(float(GetTime() * M_PI)) + 1.0f) / 2.0f * 0.4f + 0.2f;
        mat.color = ColorAlpha(color, alpha);
    }

    static inline auto tile_action(entt::registry& reg, CommandMode mode, Args args, auto f) -> std::string {
        if (args.size() != 3) {
            return std::format("ERROR: Incorrect number of args\nNeed 3, got {}\n", args.size());
        }

        auto xyz = parse_xyz(args);
        if (!xyz) return xyz.error();
        const auto [x, y, z] = *xyz;
        if (mode == CommandMode::Commit) {
            TraceLog(LOG_INFO, "Commiting command");
            return f(x, y, z);
        } else {
            set_cursor(reg, xyz_to_vec(*xyz), Vector3One(), RED);
            return "";
        }
    }

    static inline auto tile_or_region_action(entt::registry& reg, CommandMode mode, span<string_view> args, auto tile_f)
        -> string {
        if (args.size() == 3) {
            auto xyz = parse_xyz(args);
            if (!xyz) return xyz.error();
            const auto [x, y, z] = *xyz;
            if (mode == CommandMode::Commit) {
                return tile_f(x, y, z);
            } else {
                set_cursor(reg, xyz_to_vec(*xyz), Vector3One(), RED);
                return "";
            }
        } else if (args.size() == 6) {
            auto xyz_region = parse_xyz_region(args);
            if (!xyz_region) return xyz_region.error();
            const auto [min, max] = *xyz_region;
            auto [x1, y1, z1] = min;
            auto [x2, y2, z2] = max;
            if (x1 > x2) std::swap(x1, x2);
            if (y1 > y2) std::swap(y1, y2);
            if (z1 > z2) std::swap(z1, z2);

            string result {};

            if (mode == CommandMode::Commit) {
                for (auto x = x1; x <= x2; x++) {
                    for (auto y = y1; y <= y2; y++) {
                        for (auto z = z1; z <= z2; z++) {
                            result += tile_f(x, y, z);
                        }
                    }
                }
            } else {
                const auto minv = xyz_to_vec(min);
                const auto maxv = xyz_to_vec(max);
                const auto position = minv + (maxv - minv) * 0.5f;
                const auto size = (maxv - minv) + Vector3One();
                set_cursor(reg, position, size, RED);
            }

            return result;
        }

        else {
            return format("ERROR: Incorrect number of args\nNeed 3 or 6, got {}\n", args.size());
        }
    }

} // namespace detail

} // namespace cfu::dev
