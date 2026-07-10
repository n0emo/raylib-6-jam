#include "./resource.hpp"

#include <format>

#include "../raymath.hpp"

namespace cfu::res {

VoxelModel::VoxelModel(std::string_view path) {
    auto final_path = std::format("assets/vox/{}", path);
    this->model = LoadModel(final_path.c_str());
    const auto bb = GetModelBoundingBox(this->model);
    this->bounding_box = bb;
    const auto x = bb.min.x + (((bb.max.x - bb.min.x) / 2.0f));
    const auto z = bb.min.z + (((bb.max.z - bb.min.z) / 2.0f));
    Matrix matTranslate = MatrixTranslate(-x, 0.0f, -z);
    this->model.transform = matTranslate;
}

VoxelModel::VoxelModel(VoxelModel&& other) noexcept : model(other.model) {
    other.model = {};
}

auto VoxelModel::operator=(VoxelModel&& other) noexcept -> VoxelModel& {
    this->model = other.model;
    other.model = {};
    return *this;
}

VoxelModel::~VoxelModel() {
    if (IsModelValid(this->model)) UnloadModel(this->model);
}

VoxelModel::operator Model() {
    return model;
}

} // namespace cfu::res
