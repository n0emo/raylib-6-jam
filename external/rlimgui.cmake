if(BUILD_SHARED_LIBS)
    add_library(rlimgui SHARED)
else()
    add_library(rlimgui STATIC)
endif()

target_include_directories(rlimgui SYSTEM PUBLIC ${rlimgui_SOURCE_DIR})

target_sources(rlimgui PRIVATE
    ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)

if (MSVC)
    target_compile_options(rlimgui PRIVATE /W0)
else()
    target_compile_options(rlimgui PRIVATE -w)
endif()

set_target_properties(rlimgui PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(rlimgui PUBLIC raylib imgui)
