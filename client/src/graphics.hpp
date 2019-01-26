//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_GRAPHICS_H_
#define HEXAGON_CLIENT_GRAPHICS_H_

#include "sdl/renderer.hpp"
#include "sdl/sdl.hpp"
#include "sdl/window.hpp"

#include "map_textures.hpp"
#include "unit_facet.hpp"

namespace hexagon::sdl
{
    class sdl;
}

namespace hexagon::client
{
    class graphics
    {
        std::bitset<1> flags_;

        sdl::sdl sdl_;
        sdl::window window_;
        sdl::renderer renderer_;

        // textures
        map_textures tile_textures_;
        unit_texture_cache unit_textures_;

        // singleton for global callbacks
        graphics();

       public:
        static graphics& instance();

       public:
        graphics(const graphics&) = delete;
        graphics(graphics&&) = default;

        graphics& operator=(const graphics&) = delete;
        graphics& operator=(graphics&&) = default;

       public:
        sdl::renderer& operator*() { return renderer_; }
        sdl::renderer* operator->() noexcept { return &renderer_; }
        const sdl::renderer& operator*() const { return renderer_; }
        const sdl::renderer* operator->() const noexcept { return &renderer_; }

       public:
        map_textures& tiles() noexcept { return tile_textures_; }
        unit_texture_cache& units() noexcept { return unit_textures_; }

       public:
        int width() const noexcept;
        int height() const noexcept;

        sdl::window& window() noexcept;
        const sdl::window& window() const noexcept;

        sdl::window& renderer() noexcept;
        const sdl::window& renderer() const noexcept;

       public:
        void resized(bool) noexcept;
        bool resized() const noexcept;
        void resize(int w, int h) noexcept;

       public:
        template <typename Visitor>
        void handle_all(Visitor handle)
        {
            if (resized()) handle(0);
            resized(false);
        }
    };
}  // namespace hexagon::client

#endif
