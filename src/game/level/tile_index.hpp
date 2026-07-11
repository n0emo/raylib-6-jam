#pragma once

#include <gsl/gsl>

namespace cfu {

struct TileIndex {
    gsl::index x {};
    gsl::index y {};
    gsl::index z {};
};

auto operator==(const cfu::TileIndex& a, const cfu::TileIndex& b) -> bool;
auto operator!=(const cfu::TileIndex& a, const cfu::TileIndex& b) -> bool;

} // namespace cfu

template<>
struct std::hash<cfu::TileIndex> {
    auto operator()(const cfu::TileIndex& k) const -> std::size_t;
};
