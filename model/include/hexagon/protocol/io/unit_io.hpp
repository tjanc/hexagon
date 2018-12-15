//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_UNIT_IO_H_
#define HEXAGON_PROTOCOL_UNIT_IO_H_

#include <iostream>

#include <hexagon/model/unit.hpp>

namespace hexagon::protocol::io
{
    std::istream& operator>>(std::istream&, model::equipment&);
    std::ostream& operator<<(std::ostream&, const model::equipment&);

    std::istream& operator>>(std::istream&, model::power&);
    std::ostream& operator<<(std::ostream&, const model::power&);

    std::istream& operator>>(std::istream&, model::power_class&);
    std::ostream& operator<<(std::ostream&, const model::power_class&);

    std::istream& operator>>(std::istream&, model::unit_job&);
    std::ostream& operator<<(std::ostream&, const model::unit_job&);

    std::istream& operator>>(std::istream&, model::unit_status&);
    std::ostream& operator<<(std::ostream&, const model::unit_status&);

    std::istream& operator>>(std::istream&, model::unit_statistics&);
    std::ostream& operator<<(std::ostream&, const model::unit_statistics&);

    std::istream& operator>>(std::istream&, model::unit&);
    std::ostream& operator<<(std::ostream&, const model::unit&);
}  // namespace hexagon::protocol::io

#endif
