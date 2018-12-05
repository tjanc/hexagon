//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_CLIENT_CANVAS_H_
#define HEXAGON_CLIENT_CANVAS_H_

#include "sdl/renderer.hpp"

#include "map_textures.hpp"
#include "unit_facet.hpp"

namespace hexagon::sdl
{
    class window;
}

namespace hexagon::client
{
    class canvas
    {
        sdl::renderer renderer_;

        // textures
        map_textures tile_textures_;
        unit_texture_cache unit_textures_;

       public:
        explicit canvas(sdl::window& w);

       public:
        sdl::renderer& operator*() { return renderer_; }
        sdl::renderer* operator->() noexcept { return &renderer_; }
        const sdl::renderer& operator*() const { return renderer_; }
        const sdl::renderer* operator->() const noexcept { return &renderer_; }

       public:
        map_textures& tiles() noexcept { return tile_textures_; }
        unit_texture_cache& units() noexcept { return unit_textures_; }
    };
}  // namespace hexagon::client

#endif
