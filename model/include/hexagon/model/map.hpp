//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MAP_H_
#define HEXAGON_MAP_H_

#include <cassert>
#include <limits>
#include <vector>
#include "tile.hpp"

namespace hexagon::model
{
    class unit;
}

namespace hexagon::model
{
    struct basic_map_index {
        std::size_t x =
            std::numeric_limits<std::size_t>::max();  // invalid index
        std::size_t y = 0;
        basic_map_index() noexcept = default;
        basic_map_index(std::size_t x_, std::size_t y_) noexcept : x{x_}, y{y_}
        {
        }
    };

    bool operator==(const basic_map_index& lhs,
                    const basic_map_index& rhs) noexcept;

    basic_map_index operator+(const basic_map_index& lhs,
                              const basic_map_index& rhs) noexcept;

    basic_map_index operator-(const basic_map_index& lhs,
                              const basic_map_index& rhs) noexcept;

    basic_map_index west(basic_map_index i) noexcept;
    basic_map_index north_west(basic_map_index i) noexcept;
    basic_map_index north_east(basic_map_index i) noexcept;
    basic_map_index east(basic_map_index i) noexcept;
    basic_map_index south_east(basic_map_index i) noexcept;
    basic_map_index south_west(basic_map_index i) noexcept;

    template <typename T>
    class basic_map
    {
       public:
        using tiles_container = std::vector<T>;
        using iterator = typename tiles_container::iterator;
        using const_iterator = typename tiles_container::const_iterator;
        using size_type = typename tiles_container::size_type;
        using difference_type = typename tiles_container::difference_type;

       private:
        tiles_container tiles_;
        std::uint32_t width_ = 0;

       public:
        basic_map(tiles_container tiles, std::uint32_t width)
            : tiles_{tiles}, width_(width)
        {
        }

        basic_map() = default;
        basic_map(const basic_map&) = default;
        basic_map(basic_map&&) noexcept = default;
        basic_map& operator=(const basic_map&) = default;
        basic_map& operator=(basic_map&&) noexcept = default;
        ~basic_map() = default;

       public:
        auto begin() const noexcept { return tiles_.begin(); }
        auto begin() noexcept { return tiles_.begin(); }
        auto end() const noexcept { return tiles_.end(); }
        auto end() noexcept { return tiles_.end(); }

       public:
        auto width() const noexcept { return width_; }
        auto height() const noexcept { return tiles_.size() / width_; }
        auto size() const noexcept { return tiles_.size(); }

       public:
        T& at(basic_map_index idx) noexcept
        {
            assert(idx.x < width());
            assert(idx.y < height());
            return tiles_.at(idx.y * width_ + idx.x);
        }

        const T& at(basic_map_index idx) const noexcept
        {
            assert(idx.x < width());
            assert(idx.y < height());
            return tiles_.at(idx.y * width_ + idx.x);
        }

        T& at(std::uint32_t column, std::uint32_t row) noexcept
        {
            return at(basic_map_index{column, row});
        }

        const T& at(std::uint32_t column, std::uint32_t row) const noexcept
        {
            return at(basic_map_index{column, row});
        }
    };

    template <typename T>
    basic_map_index to_index(const basic_map<T>& m,
                             typename basic_map<T>::const_iterator it) noexcept
    {
        const auto offset = it - m.begin();
        const auto width = m.width();
        return {offset % width, offset / width};
    }

    template <typename T, typename Visitor>
    void iterate(const basic_map<T>& m, Visitor v)
    {
        basic_map_index idx;
        for (idx.y = 0; idx.y < m.height(); ++idx.y)
            for (idx.x = 0; idx.x < m.width(); ++idx.x) {
                v(m.at(idx), idx);
            }
    }

    template <typename T>
    bool contains(const basic_map<T>& m, basic_map_index i) noexcept
    {
        return i.x >= 0 && i.x < m.width() && i.y >= 0 && i.y < m.height();
    }

    template <typename T>
    class basic_map_overlay
    {
       public:
        using overlay_type = basic_map<T>;

       private:
        overlay_type overlay_;
        basic_map_index center_;

       public:
        basic_map_overlay() = default;
        basic_map_overlay(overlay_type m, basic_map_index center) noexcept
            : overlay_{std::move(m)}, center_{center}
        {
        }

       public:
        basic_map_index center() const noexcept { return center_; }
        const overlay_type& overlay() const noexcept { return overlay_; }

       public:
        template <typename R, typename Visitor>
        void apply(basic_map<R>& m, basic_map_index target, Visitor v) const
        {
            iterate(overlay_, [&m, &v, &target, &center = center_](
                                  const auto& obj, const auto& idx) {
                const auto t = target + (idx - center);
                if (contains(m, t)) v(m.at(t), obj);
            });
        }
    };

    using map = basic_map<tile>;

    map::iterator spawn(map&, unit&);
    basic_map_index find_unit(const map&, const unit&) noexcept;
    void move_unit(map&, basic_map_index, basic_map_index);

    // template <std::size_t Radius, typename Visitor>
    // void tiles_around(const map& m, map::const_iterator c, Visitor v)
    //{
    //    const auto width = m.width();
    //    const auto b = m.begin();
    //    const auto e = m.end();
    //    const auto c_offset = c - b;
    //    const auto c_x_offset = c_offset / width - Radius;
    //    const auto c_y_offset = c_offset % width - Radius;

    //    for (int y = 0; y < Radius * 2 + 1; ++y)
    //        for (int x = 0; x < Radius * 2 + 1; ++x) {
    //            auto it = m.find(x + c_x_offset, y + c_y_offset);
    //            if (it != m.end()) v(it, x, y);
    //        }
    //}
}  // namespace hexagon::model

#endif
