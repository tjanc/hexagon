//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_GAME_FACET_H_
#define HEXAGON_CLIENT_GAME_FACET_H_

#include <hexagon/protocol/message.hpp>
#include <variant>
#include "battle_facet.hpp"
#include "connecting_facet.hpp"
#include "world_facet.hpp"

namespace hexagon::state
{
    class local_state;
    class connecting_state;
    class world_state;
    class battling_state;
}  // namespace hexagon::state

namespace hexagon::client
{
    class game_facet
    {
       public:
        using type = std::variant<connecting_facet, world_facet, battle_facet>;

       private:
        type raw_;

       public:
        game_facet(int x, int y, int width, int height) noexcept;

       public:
        connecting_facet& get(const state::connecting_state&) noexcept;
        world_facet& get(const state::world_state&) noexcept;
        battle_facet& get(const state::battling_state&) noexcept;

        const connecting_facet& get(const state::connecting_state&) const
            noexcept;
        const world_facet& get(const state::world_state&) const noexcept;
        const battle_facet& get(const state::battling_state&) const noexcept;

        int width() const noexcept;
        int height() const noexcept;

        game_facet& operator=(const connecting_facet& facet)
        {
            raw_ = facet;
            return *this;
        }
        game_facet& operator=(connecting_facet&& facet)
        {
            raw_ = std::move(facet);
            return *this;
        }

        game_facet& operator=(const world_facet& facet)
        {
            raw_ = facet;
            return *this;
        }
        game_facet& operator=(world_facet&& facet)
        {
            raw_ = std::move(facet);
            return *this;
        }

        game_facet& operator=(const battle_facet& facet)
        {
            raw_ = facet;
            return *this;
        }
        game_facet& operator=(battle_facet&& facet)
        {
            raw_ = std::move(facet);
            return *this;
        }

       public:
        void resize(int w, int h) noexcept;
    };

    void draw(graphics& g, const game_facet& facet,
              const state::local_state& s);

}  // namespace hexagon::client

#endif
