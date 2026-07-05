#include "states.hpp"

namespace game {

auto StateStack::setup(entt::registry& registry) -> void {
    registry.ctx().insert_or_assign(StateStack());
}

auto StateStack::get(entt::registry& registry) -> StateStack& {
    return registry.ctx().get<StateStack>();
}

auto StateStack::push(State state) -> void {
    m_pending = PendingPush {.state = std::move(state)};
}

auto StateStack::pop() -> void {
    m_pending = PendingPop {};
}

auto StateStack::replace(State state) -> void {
    m_pending = PendingPush {.state = std::move(state)};
}

auto StateStack::update(entt::registry& registry) -> void {
    if (auto pending = std::get_if<PendingPush>(&m_pending)) {
        std::visit([&](auto& s) -> void { s.on_enter(registry); }, pending->state);
        m_stack.push(std::move(pending->state));
        m_pending = NoPendingTransition {};
    } else if (!m_stack.empty() && std::holds_alternative<PendingPop>(m_pending)) {
        std::visit([&](auto& s) -> void { s.on_exit(registry); }, m_stack.top());
        m_stack.pop();
        m_pending = NoPendingTransition {};
    } else if (auto pending = std::get_if<PendingReplace>(&m_pending)) {
        std::visit([&](auto& s) -> void { s.on_exit(registry); }, m_stack.top());
        m_stack.pop();
        std::visit([&](auto& s) -> void { s.on_enter(registry); }, pending->state);
        m_stack.push(std::move(pending->state));
        m_pending = NoPendingTransition {};
    }

    if (!m_stack.empty()) {
        std::visit([&](auto& s) -> void { s.update(registry); }, m_stack.top());
    }
}

auto StateStack::draw(entt::registry& registry) -> void {
    if (!m_stack.empty()) {
        std::visit([&](auto& s) -> void { s.draw(registry); }, m_stack.top());
    }
}

auto StateStack::clear(entt::registry& registry) -> void {
    while (!m_stack.empty()) {
        std::visit([&](auto& s) -> void { s.on_exit(registry); }, m_stack.top());
        m_stack.pop();
    }
}

} // namespace game
