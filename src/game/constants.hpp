#pragma once

#include <raylib.h>

namespace cfu {

constexpr auto WINDOW_WIDTH = 720;
constexpr auto WINDOW_HEIGHT = 720;

constexpr auto B2_STEPS = 4;

constexpr auto CAMERA_LERP = 15.0f;
constexpr auto CAMERA_OFFSET = Vector3(100.0f, 100.0f, 100.0f);
constexpr auto CAMERA_UP = Vector3(0.0f, 1.0f, 0.0f);
constexpr auto CAMERA_FOVY = 120.0f;
constexpr auto CAMERA_PROJECTION = CAMERA_ORTHOGRAPHIC;

} // namespace cfu
