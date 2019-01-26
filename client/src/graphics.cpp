//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "graphics.hpp"
#include <emscripten.h>
#include "sdl/sdl.hpp"
#include "sdl/window.hpp"
#include "version.hpp"

using namespace hexagon::client;

extern "C" {
EMSCRIPTEN_KEEPALIVE void window_resized(int w, int h)
{
    graphics::instance().resize(w, h);
}
}

// clang-format off
EM_JS(void, adapt_canvas, (), {
    window.onresize = function() {
      canvas.width = window.innerWidth;
      canvas.height = canvas.width * 2/3;
      _window_resized(canvas.width, canvas.height);
    };
    window.onresize();
});

EM_JS(int, window_width, (), {
    const width = window.innerWidth;
    return width;
});

EM_JS(int, window_height, (), {
    const height = window.innerHeight;
    return height;
});

EM_JS(int, canvas_width, (), {
    const width = canvas.offsetWidth;
    return width;
});

EM_JS(int, canvas_height, (), {
    const height = canvas.offsetHeight;
    return height;
});
// clang-format on

namespace
{
    constexpr std::size_t RESIZED_FLAG = 0;
}

graphics::graphics()
    : flags_{1 << RESIZED_FLAG},
      sdl_{},
      window_{sdl_, "Hexagon " HEXAGON_CLIENT_VERSION, window_width(),
              window_height()},
      renderer_{window_},
      tile_textures_{renderer_},
      unit_textures_{renderer_}
{
    adapt_canvas();
}

graphics& graphics::instance()
{
    static graphics instance_;
    return instance_;
}

int graphics::width() const noexcept { return canvas_width(); }
int graphics::height() const noexcept { return canvas_height(); }

void graphics::resized(bool v) noexcept { flags_.set(RESIZED_FLAG, v); }
bool graphics::resized() const noexcept { return flags_.test(RESIZED_FLAG); }
void graphics::resize(int w, int h) noexcept
{
    flags_.set(RESIZED_FLAG);
    window_.resize(w, h);
}
