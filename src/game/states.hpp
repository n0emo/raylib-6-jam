#include <stack>
#include <variant>

#include <entt/entt.hpp>
#include <gsl/gsl>

#include "main_menu/state.hpp"
#include "in_game/state.hpp"

namespace game {

using State = std::variant<MainMenuState, InGameState>;

class StateStack {
  public:
    static auto setup(entt::registry& registry) -> void;
    static auto get(entt::registry& registry) -> StateStack&;

    auto push(State state) -> void;
    auto pop() -> void;
    auto replace(State state) -> void;
    auto update(entt::registry& registry) -> void;
    auto draw(entt::registry& registry) -> void;
    auto clear(entt::registry& registry) -> void;

  private:
    struct NoPendingTransition {};

    struct PendingPush {
        State state;
    };

    struct PendingPop {};

    struct PendingReplace {
        State state;
    };

    std::stack<State> m_stack;
    std::variant<NoPendingTransition, PendingPush, PendingPop, PendingReplace> m_pending = NoPendingTransition {};
};
} // namespace game
