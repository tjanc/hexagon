//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "world_adapter.hpp"

#include <hexagon/state/local_state.hpp>
#include "battle_facet.hpp"
#include "game_facet.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::protocol;
using namespace hexagon::state;

namespace
{
    template <typename Message>
    constexpr bool handles_message = false;

    template <>
    constexpr bool handles_message<battle_message> = true;

}  // namespace

namespace
{
    void update_specific(local_state& s, world_state& cstate, game_facet& facet,
                         battle_message m)
    {
        const auto& teams = m.battle.teams();
        std::cout << "Into battle! There are " << teams.size()
                  << " teams, you are number " << m.team_id << '\n';

        if (teams.end() == std::find_if(teams.begin(), teams.end(),
                                        [tid = m.team_id](const auto& t) {
                                            return t.id == tid;
                                        })) {
            std::cerr << "WARN: invalid team id `" << m.team_id << "`\n";
            return;
        }

        const auto team_id = m.team_id;
        s.to_battle(battling_state{std::move(m.battle), team_id});

        facet = battle_facet{0, 0, facet.width(), facet.height()};
    }
}  // namespace

void hexagon::client::update(local_state& s, world_state& cstate,
                             game_facet& facet, protocol::server_message&& msg)
{
    std::visit(
        [&s, &cstate, &facet](auto m) {
            using M = typename std::decay<decltype(m)>::type;
            if constexpr (handles_message<M>) {
                update_specific(s, cstate, facet, std::move(m));
            } else {
                std::cout << "WARN: ignoring unexpected message "
                          << id<M> << '\n';
            }
        },
        std::move(msg));
}

void hexagon::client::update(local_state&, world_state&, game_facet&,
                             const mouse&)
{
    //  TODO
}
