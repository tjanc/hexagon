//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include "map_loader.hpp"

#include <emscripten.h>

using namespace hexagon;

namespace
{
    bool is_space(char c) { return c == ' ' || c == '\t'; }

}  // namespace

map hexagon::load_map(const char* file_name)
{
    map::tile_container tiles;
    int width = -1;

    FILE* file = fopen(file_name, "r");
    if (!file) throw file_not_found{file_name};

    char kind_flag = 'g';
    int elevation = 0;

    int scanned;
    while ((scanned = fscanf(file, " %c%d", &kind_flag, &elevation)) != EOF) {
        if (scanned != 2) {
            fclose(file);
            throw map_syntax_error();
        }

        switch (kind_flag) {
            default:
                fclose(file);
                throw map_syntax_error();
            case 'G':
                tiles.emplace_back(tile_type::grass0, elevation, true);
                break;
            case 'g':
                tiles.emplace_back(tile_type::grass0, elevation, false);
                break;
            case 'H':
                tiles.emplace_back(tile_type::grass1, elevation, true);
                break;
            case 'h':
                tiles.emplace_back(tile_type::grass1, elevation, false);
                break;
            case 'I':
                tiles.emplace_back(tile_type::grass2, elevation, true);
                break;
            case 'i':
                tiles.emplace_back(tile_type::grass2, elevation, false);
                break;
            case 'J':
                tiles.emplace_back(tile_type::grass3, elevation, true);
                break;
            case 'j':
                tiles.emplace_back(tile_type::grass3, elevation, false);
                break;
            case 'K':
                tiles.emplace_back(tile_type::grass4, elevation, true);
                break;
            case 'k':
                tiles.emplace_back(tile_type::grass4, elevation, false);
                break;
            case 'D':
                tiles.emplace_back(tile_type::dirt0, elevation, true);
                break;
            case 'd':
                tiles.emplace_back(tile_type::dirt0, elevation, false);
                break;
            case 'E':
                tiles.emplace_back(tile_type::dirt1, elevation, true);
                break;
            case 'e':
                tiles.emplace_back(tile_type::dirt1, elevation, false);
                break;
        }

        int postfix = fgetc(file);
        if (';' == postfix) {
            if (width < 0) width = tiles.size();
        } else
            ungetc(postfix, file);
    }

    fclose(file);
    return {tiles, width};
}  // namespace
