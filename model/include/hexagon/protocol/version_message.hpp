//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_PROTOCOL_VERSION_MESSAGE_H_
#define HEXAGON_PROTOCOL_VERSION_MESSAGE_H_

#include <iomanip>
#include <iostream>

namespace hexagon::protocol
{
    static constexpr const char* VERSION_RESPONSE_ID = "VERSION";

    struct version_response {
        int major_version = 0;
        int minor_version = 0;
        int patch_version = 0;

        version_response() = default;
        version_response(std::uint16_t major_, std::uint16_t minor_,
                         std::uint16_t patch_)
            : major_version(major_),
              minor_version(minor_),
              patch_version(patch_)
        {
        }
    };

    std::istream& operator>>(std::istream& in, version_response& msg);
    std::ostream& operator<<(std::ostream& out, const version_response& msg);
}  // namespace hexagon::protocol

#endif
