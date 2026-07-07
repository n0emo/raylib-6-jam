#pragma once

#if defined(__llvm__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#else
#pragma warning(push, 0)
#endif

#include <raylib.h> // IWYU pragma: export
#include <raymath.h> // IWYU pragma: export

#if defined(__llvm__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#else
#pragma warning(pop)
#endif
