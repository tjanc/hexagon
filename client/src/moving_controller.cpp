//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "moving_controller.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>

#include <hexagon/model/map.hpp>

#include "battle_controller.hpp"
#include "mouse.hpp"

using namespace hexagon::client;
using namespace hexagon::model;

namespace
{
    using reach_map = std::vector<int>;

    auto left_of(int width, reach_map& reaches, reach_map::iterator it)
    {
        const auto offset = it - reaches.begin();
        if (0 == (offset % width)) {
            return reaches.end();
        } else {
            return --it;
        }
    }

    auto right_of(int width, reach_map& reaches, reach_map::iterator it)
    {
        if (((width - 1) == ((it - reaches.begin()) % width))) {
            return reaches.end();
        } else {
            return ++it;
        }
    }

    auto top_right_of(int width, reach_map& reaches, reach_map::iterator it)
    {
        const auto offset = it - reaches.begin();
        const bool is_wide = 0 == ((offset / width) % 2);

        if (((width - 1) == (offset % width)))
            if (!is_wide) return reaches.end();

        if (is_wide) return it - width;
        return it - width + 1;
    }

    auto top_left_of(int width, reach_map& reaches, reach_map::iterator it)
    {
        const auto offset = it - reaches.begin();
        const bool is_wide = 0 == ((offset / width) % 2);

        if ((0 == (offset % width)))
            if (is_wide) return reaches.end();

        if (!is_wide) return it - width;
        return it - width - 1;
    }

    auto bottom_right_of(int width, reach_map& reaches, reach_map::iterator it)
    {
        const auto offset = it - reaches.begin();
        const bool is_wide = 0 == ((offset / width) % 2);

        if (((width - 1) == (offset % width)))
            if (!is_wide) return reaches.end();

        if (is_wide) return it + width;
        return it + width + 1;
    }

    auto bottom_left_of(int width, reach_map& reaches, reach_map::iterator it)
    {
        const auto offset = it - reaches.begin();
        const bool is_wide = 0 == ((offset / width) % 2);

        if ((0 == (offset % width)))
            if (is_wide) return reaches.end();

        if (!is_wide) return it + width;
        return it + width - 1;
    }

    void mark_reachable(map& m, map::tile_container::iterator tile)
    {
        assert(tile->get_if_unit());
        auto& u = *tile->get_if_unit();

        // reset overlay flags
        for (auto& t : m) t.reset_reachable();

        reach_map reaches(m.size(), 0);

        std::deque<reach_map::iterator> queue;
        auto unit_reach_it = reaches.begin() + (tile - m.begin());
        *unit_reach_it = u.range_;
        queue.emplace_back(unit_reach_it);

        auto queue_reach = [&reaches, &m, &queue](auto current, auto next) {
            if (reaches.end() != next) {
                const auto map_current =
                    m.begin() + (current - reaches.begin());
                const auto map_tile = m.begin() + (next - reaches.begin());
                const auto d_elev =
                    map_tile->elevation() - map_current->elevation();

                float reach = 0;
                if (d_elev >= 0 && d_elev < 2)
                    reach = *current - 1 - d_elev;
                else if (d_elev < 0 && d_elev > -2)
                    reach = *current - 1;

                if (map_tile->empty() && *next < reach) {
                    *next = reach;
                    queue.emplace_front(next);
                }
            }
        };

        while (!queue.empty()) {
            auto current = queue.back();

            queue_reach(  //
                current,  //
                left_of(m.width(), reaches, current));

            queue_reach(  //
                current,  //
                right_of(m.width(), reaches, current));

            queue_reach(  //
                current,  //
                top_right_of(m.width(), reaches, current));

            queue_reach(  //
                current,  //
                top_left_of(m.width(), reaches, current));

            queue_reach(  //
                current,  //
                bottom_right_of(m.width(), reaches, current));

            queue_reach(  //
                current,  //
                bottom_left_of(m.width(), reaches, current));

            queue.pop_back();
        }

        auto b = m.begin();
        for (auto& reach : reaches) {
            if (reach > 0) b->set_reachable();
            ++b;
        }
        assert(b == m.end());
    }

    void move_unit(map::tile_container::iterator source,
                   map::tile_container::iterator target)
    {
        auto* u = source->detach_unit();
        assert(u);
        assert(target->empty());
        target->attach(*u);
    }
}  // namespace

moving_controller::moving_controller(battle b,
                                     battle::team_container::iterator t,
                                     team::unit_container::iterator u) noexcept
    : model_{std::move(b)},
      facet_{0, 0},
      team_ptr_{t},
      unit_ptr_{u},
      unit_tile_ptr_{model_.get_map().find_unit(*u)},
      commands_{}
{
    if (unit_tile_ptr_ != model_.get_map().end())
        mark_reachable(model_.get_map(), unit_tile_ptr_);
}

void moving_controller::update(battle_controller& c, const mouse& m) noexcept
{
    map& field = model_.get_map();

    if (m.moved()) {
        auto [x, y] = facet_.map().transpose(m.x, m.y);
        auto new_hover = field.find(x, y);
        if (new_hover != field.end()) facet_.map().hover(new_hover);
    }

    if (m.released()) {
        auto source = unit_tile_ptr_;
        auto target = facet_.map().hover();

        if (source != field.end()      //
            && target != field.end()   //
            && target->is_reachable()  //
            && target->empty()) {
            //
            move_unit(source, target);
            commands_.emplace_back(source, target);

            auto next = unit_ptr_ + 1;

            // if everyone in team is moved, commit and transfer to next state
            if (next == team_ptr_->units.end()) {
                std::cout << "TODO: commit movements\n";
                c.state(moving_controller{std::move(model_), team_ptr_,
                                          team_ptr_->units.begin()});
            } else {
                c.state(
                    moving_controller{std::move(model_), team_ptr_, next});
            }

        }
    }
}

void moving_controller::draw(canvas& c) const { facet_.draw(c, *this); }

const battle& moving_controller::model() const noexcept { return model_; }

battle& moving_controller::model() noexcept { return model_; }

map::tile_container::iterator moving_controller::unit_tile() noexcept
{
    return unit_tile_ptr_;
}

map::tile_container::const_iterator moving_controller::unit_tile() const
    noexcept
{
    return unit_tile_ptr_;
}
