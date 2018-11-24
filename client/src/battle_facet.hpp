#ifndef HEXAGON_BATTLE_FACET_H_
#define HEXAGON_BATTLE_FACET_H_

#include <hexagon/model/battle_projection.hpp>

#include "sdl/texture.hpp"

#include "map_facet.hpp"

namespace hexagon::sdl
{
    class renderer;
}

namespace hexagon
{
    class battle_facet
    {
        map_facet map_facet_;

        //sdl::texture active_unit_name_;

        int width_ = 0;
        int height_ = 0;

       public:
        explicit battle_facet(sdl::renderer&);

       public:
        void mouse_move(model::battle_projection& model);
        void mouse_down(model::battle_projection& model);
        void mouse_up(model::battle_projection& model);

        void draw(sdl::renderer& r, model::battle_projection& model);
    };
}  // namespace hexagon

#endif
