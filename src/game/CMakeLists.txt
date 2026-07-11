include(../cmake/deps.cmake)

if(GAME_DEV_MODE)
    add_library(${PROJECT_NAME}-game MODULE)
else()
    add_library(${PROJECT_NAME}-game STATIC)
endif()

target_sources(${PROJECT_NAME}-game PRIVATE
    ./audio/components.cpp
    ./audio/resource.cpp
    ./audio/systems.cpp
    ./background/components.cpp
    ./background/systems.cpp
    ./camera/systems.cpp
    ./combat/components.cpp
    ./data/components.cpp
    ./data/systems.cpp
    ./dev/commands.cpp
    ./dev/commands_level.cpp
    ./dev/commands_state.cpp
    ./dev/systems.cpp
    ./dev/systems_console.cpp
    ./dev/systems_data_editor.cpp
    ./dev/systems_dev_panel.cpp
    ./dev/systems_entity_editor.cpp
    ./dev/systems_level_editor.cpp
    ./dev/utils.cpp
    ./enemy/components.cpp
    ./enemy/systems.cpp
    ./fonts/resource.cpp
    ./fonts/systems.cpp
    ./game.cpp
    ./in_game/state.cpp
    ./level/components.cpp
    ./level/resource.cpp
    ./level/systems.cpp
    ./level/tile_index.cpp
    ./main_menu/state.cpp
    ./physics/components.cpp
    ./physics/systems.cpp
    ./player/components.cpp
    ./player/systems.cpp
    ./render_texture/systems.cpp
    ./shaders/components.cpp
    ./shaders/systems.cpp
    ./shapes/components.cpp
    ./shapes/systems.cpp
    ./solids/components.cpp
    ./solids/systems.cpp
    ./states.cpp
    ./textures/resource.cpp
    ./textures/systems.cpp
    ./vox/resource.cpp
    ./vox/systems.cpp
)

set_source_files_properties(./plugin.cpp PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

target_precompile_headers(${PROJECT_NAME}-game PRIVATE ./pch.hpp)

target_include_directories(${PROJECT_NAME}-game PUBLIC .)

set_target_properties(${PROJECT_NAME}-game PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_compile_definitions(${PROJECT_NAME}-game PRIVATE
    -DGAME_DEV_MODE=$<BOOL:${GAME_DEV_MODE}>
)

if(MSVC)
    target_compile_options(${PROJECT_NAME}-game PRIVATE /W3 /EHsc)
else()
    target_compile_options(${PROJECT_NAME}-game PRIVATE -Wall -Wextra -g)
endif()

set_dependencies(${PROJECT_NAME}-game)

if(GAME_DEV_MODE)
    set_target_properties(${PROJECT_NAME}-game PROPERTIES OUTPUT_NAME plugin)
    target_sources(${PROJECT_NAME}-game PRIVATE plugin.cpp)
endif()
