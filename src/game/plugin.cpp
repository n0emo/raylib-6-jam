#include <cassert>

#include <cr.h>
#include <gsl/gsl>
#include <raylib.h>

#include <game.hpp>

CR_EXPORT auto cr_main(struct cr_plugin *ctx, enum cr_op operation) -> int {
    assert(ctx);

    switch (operation) {
        case CR_STEP: {
            auto game = static_cast<game::Game *>(ctx->userdata);
            game->frame();
            return game->running() ? 0 : 1;
        }
        case CR_LOAD: {
            if (ctx->userdata == nullptr) {
                auto game = gsl::owner<game::Game *> {new game::Game {}};
                game::Game::init(*game);
                ctx->userdata = game;
            } else {
                TraceLog(LOG_INFO, "Hot-reload success");
            }
            return 0;
        }
        case CR_UNLOAD: {
            TraceLog(LOG_INFO, "Preparing to hot-reload");
            return 0;
        }
        case CR_CLOSE: {
            auto game = gsl::owner<game::Game *>(ctx->userdata);
            game::Game::deinit(*game);
            delete game;
            return 0;
        }
        default:
            return -1;
    }
}
