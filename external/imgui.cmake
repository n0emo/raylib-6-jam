if(BUILD_SHARED_LIBS)
    add_library(imgui SHARED)
else()
    add_library(imgui STATIC)
endif()

target_include_directories(imgui SYSTEM PUBLIC ${imgui_SOURCE_DIR})
target_sources(imgui PRIVATE
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
)

if (MSVC)
    target_compile_options(imgui PRIVATE /W0)
else()
    target_compile_options(imgui PRIVATE -w)
endif()

set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)
