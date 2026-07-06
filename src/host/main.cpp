#if GAME_HOT_RELOAD

#define CR_DEBUG
#define CR_LOG(...)
#define CR_HOST
#include <cr.h>

constexpr auto DLL_PATH = "build/" CR_PLUGIN("plugin");

auto main(int argc, char *argv[]) -> int {
    cr_plugin ctx {};
    cr_plugin_open(ctx, DLL_PATH);

    while (true) {
        if (cr_plugin_update(ctx) == 1) break;
	if (ctx.failure == CR_INITIAL_FAILURE) break;
    }

    cr_plugin_close(ctx);

    return 0;
}

#else

#include <game.hpp>

auto main(int argc, char *argv[]) -> int {
    auto game = game::Game {};
    game::Game::init(game);

    while (game.running()) {
        game.frame();
    }

    return 0;
}

#endif
