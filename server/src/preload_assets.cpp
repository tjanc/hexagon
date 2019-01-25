//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "preload_assets.hpp"

#include "map_loader.hpp"

using namespace hexagon::server;
using namespace hexagon::model;

preload_assets::preload_assets(boost::filesystem::path assets_dir)
    : maps_{load_map(assets_dir.append("map2.map").string())}
{
}

const map* preload_assets::get_map(std::size_t i) const noexcept
{
    if (i < maps_.size()) return &maps_.at(i);
    return nullptr;
}
