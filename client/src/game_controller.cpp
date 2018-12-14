//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "game_controller.hpp"

#include <hexagon/model/map_loader.hpp>
#include <hexagon/protocol/io/unit_io.hpp>
#include <type_traits>
#include "connection.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::model;
using namespace hexagon::protocol;

namespace
{
    void update_specific(battle_controller& c, const mouse& m) { c.update(m); }

    void update_specific(connecting_controller&, const mouse&) {}

    void update_specific(world_controller&, const mouse&) {}

}  // namespace

namespace
{
    void draw_specific(const connecting_controller&, canvas&) {}

    void draw_specific(const battle_controller& s, canvas& c) { s.draw(c); }

    void draw_specific(const world_controller&, canvas&) {}
}  // namespace

game_controller::game_controller(connecting_facet facet)
    : state_{connecting_controller{std::move(facet)}}
{
}

namespace
{
    template <typename Controller, typename Message>
    constexpr bool handles_message = false;

    template <>
    constexpr bool handles_message<connecting_controller, version_response> =
        true;

    template <>
    constexpr bool handles_message<connecting_controller, login_response> =
        true;

    template <>
    constexpr bool handles_message<world_controller, battle_message> = true;

}  // namespace

namespace
{
    template <typename State, typename Controller, typename Message>
    void respond(Controller& c, Message m, State& s)
    {
        using C = typename std::decay<decltype(c)>::type;
        using M = typename std::decay<decltype(m)>::type;
        if constexpr (handles_message<C, M>) {
            c.update(s, std::move(m));
        } else {
            std::cout << "WARN: ignoring message " << id<Message> << '\n';
        }
    }
}  // namespace

void game_controller::update(server_message msg)
{
    std::visit(
        [self = this, &msg](auto& c) {
            std::visit(
                [self, &c](auto m) {  //
                    respond(c, std::move(m), *self);
                },
                std::move(msg));
        },
        state_);
    updated_ = true;
}

void game_controller::update(const mouse& m)
{
    std::visit(
        [&m](auto& s) {  //
            return update_specific(s, m);
        },
        state_);
    updated_ = true;
}

void game_controller::draw(canvas& c)
{
    std::visit(
        [&c](const auto& s) {  //
            return draw_specific(s, c);
        },
        state_);
    updated_ = false;
}

bool game_controller::updated() const noexcept { return updated_; }

void game_controller::to_world(world w)
{
    state_ = world_controller{std::move(w), world_facet{0, 0, 0, 0}};
}

void game_controller::to_battle(battle b, std::size_t tid)
{
    //
    state_ = battle_controller{battle_facet{0, 0, 0, 0}, std::move(b), tid};
}
