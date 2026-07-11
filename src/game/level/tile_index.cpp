#include "./tile_index.hpp"

namespace cfu {

auto operator==(const TileIndex& a, const TileIndex& b) -> bool {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

auto operator!=(const TileIndex& a, const TileIndex& b) -> bool {
    return !(a == b);
}

} // namespace cfu

auto std::hash<cfu::TileIndex>::operator()(const cfu::TileIndex& k) const -> std::size_t {
    using std::hash;
    using std::size_t;
    using std::string;

    std::size_t res = 17;
    res = res * 31 + hash<gsl::index>()(k.x);
    res = res * 31 + hash<gsl::index>()(k.y);
    res = res * 31 + hash<gsl::index>()(k.z);
    return res;
};
