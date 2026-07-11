#pragma once

#include <entt/resource/resource.hpp>
#include <entt/entity/registry.hpp>
#include <entt/resource/cache.hpp>
#include <gsl/gsl>
#include <raylib.h>

namespace cfu::res {

namespace vox {

#define RES_VOX_ID_LIST                                                                                                \
    CFU_X(BloodPool, "blood-pool.vox")                                                                                 \
    CFU_X(BloodWay, "blood-way.vox")                                                                                   \
    CFU_X(Boiler, "boiler.vox")                                                                                        \
    CFU_X(Bookshelf, "bookshelf.vox")                                                                                  \
    CFU_X(Box2, "box-2.vox")                                                                                           \
    CFU_X(Box, "box.vox")                                                                                              \
    CFU_X(BrokenChain, "broken-chain.vox")                                                                             \
    CFU_X(ChimeraCage, "chimera-cage.vox")                                                                             \
    CFU_X(ConveyorEnd, "conveyor-end.vox")                                                                             \
    CFU_X(ConveyorMiddle, "conveyor-middle.vox")                                                                       \
    CFU_X(Door2, "door-2.vox")                                                                                         \
    CFU_X(Door, "door.vox")                                                                                            \
    CFU_X(Error, "error.vox")                                                                                          \
    CFU_X(Floor, "floor.vox")                                                                                          \
    CFU_X(LabTable, "lab-table.vox")                                                                                   \
    CFU_X(Lamp, "lamp.vox")                                                                                            \
    CFU_X(Machine, "machine.vox")                                                                                      \
    CFU_X(PipeEnd, "pipe-end.vox")                                                                                     \
    CFU_X(PipeStraight, "pipe-straight.vox")                                                                           \
    CFU_X(Pipe, "pipe.vox")                                                                                            \
    CFU_X(Player, "player.vox")                                                                                        \
    CFU_X(Stair, "stair.vox")                                                                                          \
    CFU_X(Stand, "stand.vox")                                                                                          \
    CFU_X(TortureTable, "torture-table.vox")                                                                           \
    CFU_X(WallShackles, "wall-shackles.vox")                                                                           \
    CFU_X(Wall, "wall.vox")                                                                                            \
    CFU_X(Washbasin, "washbasin.vox")

    enum VoxelModelId {

    #define CFU_X(name, ...) name,
        RES_VOX_ID_LIST
    #undef CFU_X

    };

} // namespace vox

auto model_id_to_string(vox::VoxelModelId id) -> gsl::czstring;
auto model_id_from_string(std::string_view string) -> vox::VoxelModelId;

struct VoxelModel {
    Model model {};
    BoundingBox bounding_box {};

    VoxelModel(std::string_view path);
    VoxelModel(VoxelModel&& other) noexcept;
    auto operator=(VoxelModel&& other) noexcept -> VoxelModel&;
    ~VoxelModel();

    VoxelModel(const VoxelModel&) = delete;
    auto operator=(const VoxelModel&) -> VoxelModel& = delete;

    operator Model();
};

using VoxelModelCache = entt::resource_cache<VoxelModel>;

} // namespace cfu::res
