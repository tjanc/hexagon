//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_SERVER_PRELOAD_ASSETS_H_
#define HEXAGON_SERVER_PRELOAD_ASSETS_H_

#include <boost/filesystem.hpp>
#include <vector>

#include <hexagon/model/map.hpp>

namespace hexagon::server
{
    class preload_assets
    {
        const std::vector<hexagon::model::map> maps_;

       public:
        explicit preload_assets(boost::filesystem::path assets_dir);

       public:
        const hexagon::model::map* get_map(std::size_t) const noexcept;
    };

}  // namespace hexagon::server

#endif
