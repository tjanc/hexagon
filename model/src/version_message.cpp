//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <hexagon/protocol/version_message.hpp>

#include <iostream>

namespace hexagon::protocol
{
    std::istream& operator>>(std::istream& in, version_response& msg)
    {
        in >> msg.major_version;
        in >> msg.minor_version;
        in >> msg.patch_version;

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const version_response& msg)
    {
        out << msg.major_version << ' '  //
            << msg.minor_version << ' '  //
            << msg.patch_version << ' ';

        return out;
    }

}  // namespace hexagon::protocol
