//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/model/battle_projection.hpp>

#include <algorithm>
#include <cassert>
#include <deque>

#include <hexagon/model/map.hpp>

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

    struct mark_reachable_visitor {
        hexagon::map& map_;

        void operator()(const move_state& state) const
        {
            auto tile = map_.find_unit(*state.current_unit_);
            for (auto& t : map_) t.reset_reachable();

            if (tile == map_.end()) {
                assert(false);
                return;
            }

            reach_map reaches(map_.size(), 0);

            std::deque<reach_map::iterator> queue;
            auto unit_reach_it = reaches.begin() + (tile - map_.begin());
            *unit_reach_it = state.current_unit_->range_;
            queue.emplace_back(unit_reach_it);

            auto queue_reach = [&reaches, &map = map_, &queue](auto current,
                                                               auto next) {
                if (reaches.end() != next) {
                    const auto map_current =
                        map.begin() + (current - reaches.begin());
                    const auto map_tile =
                        map.begin() + (next - reaches.begin());
                    const auto d_elev =
                        map_tile->elevation() - map_current->elevation();

                    float reach = 0;
                    if (d_elev >= 0 && d_elev < 2)
                        reach = *current - 1 - d_elev;
                    else if (d_elev < 0 && d_elev > -2)
                        reach = *current - 1;

                    if (*next < reach) {
                        *next = reach;
                        queue.emplace_front(next);
                    }
                }
            };

            while (!queue.empty()) {
                auto current = queue.back();

                queue_reach(  //
                    current,  //
                    left_of(map_.width(), reaches, current));

                queue_reach(  //
                    current,  //
                    right_of(map_.width(), reaches, current));

                queue_reach(  //
                    current,  //
                    top_right_of(map_.width(), reaches, current));

                queue_reach(  //
                    current,  //
                    top_left_of(map_.width(), reaches, current));

                queue_reach(  //
                    current,  //
                    bottom_right_of(map_.width(), reaches, current));

                queue_reach(  //
                    current,  //
                    bottom_left_of(map_.width(), reaches, current));

                queue.pop_back();
            }

            auto b = map_.begin();
            for (auto& reach : reaches) {
                if (reach > 0) b->set_reachable();
                ++b;
            }
            assert(b == map_.end());
        }

        void operator()(const attack_state&) const
        {
            assert(!"attack reachability not implemented");
        }

        void operator()(const move_state_committed&) const
        {
            assert(!"reachability cannot be calculated during commit");
        }

        void operator()(const attack_state_committed&) const
        {
            assert(!"reachability cannot be calculated during commit");
        }
    };
}  // namespace

void battle_projection::mark_reachable()
{
    std::visit(mark_reachable_visitor{battle_->get_map()}, state_);
}

battle_projection::battle_projection(battle& b, team t)
    : battle_(&b), team_(b.join(std::move(t))), state_(move_state{*team_})
{
    mark_reachable();
}

void battle_projection::move(map::tile_container::iterator target)
{
    if (target == battle_->get_map().end()) assert(!"cannot move out of map");
    assert(target->is_reachable());

    if (move_state* state = std::get_if<move_state>(&state_)) {
        map& field = battle_->get_map();

        auto source = field.find_unit(*state->current_unit_);
        assert(source != field.end());

        state->commands_.emplace_back(source, target);
        ++state->current_unit_;

        // if everyone in team is moved, commit and transfer to next state
        if (state->current_unit_ == team_->units.end()) {
            battle_->commit_movements(std::begin(state->commands_),
                                      std::end(state->commands_));

            // TODO XXX loop on move_state for debug purposes
            // state_ = move_state_committed{};
            state_ = move_state{*team_};
            mark_reachable();
        }

        return;
    }

    assert(!"moving only possible when in move state");
}
