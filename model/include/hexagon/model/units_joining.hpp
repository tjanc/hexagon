//  Copyright 2019 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_MODEL_UNITS_JOINING_H_
#define HEXAGON_MODEL_UNITS_JOINING_H_

namespace hexagon::model
{
    struct team;
}

namespace hexagon::model
{
    class units_joining
    {
        /// team in control
        team* team_;

       public:
        explicit units_joining(team& team) noexcept;

        units_joining(const units_joining&) noexcept = default;
        units_joining(units_joining&&) noexcept = default;
        units_joining& operator=(const units_joining&) noexcept = default;
        units_joining& operator=(units_joining&&) noexcept = default;

        const team& my_team() const noexcept;
        team& my_team() noexcept;
    };

}  // namespace hexagon::model

#endif
