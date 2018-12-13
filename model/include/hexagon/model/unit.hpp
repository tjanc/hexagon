//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#ifndef HEXAGON_UNIT_H_
#define HEXAGON_UNIT_H_

#include <bitset>
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <hexagon/model/map.hpp>

namespace hexagon::model
{
    enum class unit_job : std::uint8_t {
        none = 0,  //

        warrior,  //
        mage,     //
        druid,    //
        acolyte,  //
        hunter,   //
        rogue,    //

        illusionist,  //
        paladin,      //
        warlock,      //
        animist,      //
        beastmaster,  //
        assassin,     //

        shadowwalker,  //
        prophet,       //
        berserker,     //
        arcane,        //
        elementalist,  //
        sharpshooter   //
    };

    struct unit_statistics {
        std::uint16_t agility = 100;
        std::uint16_t presence = 100;
        std::uint16_t strength = 100;
        std::uint16_t intelligence = 100;
        std::uint16_t wisdom = 100;
        std::uint16_t endurance = 100;
        unit_statistics() = default;
    };

    using damage_overlay = basic_map_overlay<std::uint16_t>;
    damage_overlay circular_overlay(std::size_t n, std::uint16_t dmg) noexcept;

    enum class power_class : std::uint16_t {
        none = 0x000,
        melee = 0x100,
        fired = 0x200,
        magic = 0x400
    };

    class power
    {
       private:
        std::string name_ = "None";

        std::bitset<16> flags_ = 0;
        std::uint16_t jp_req_ = 0xFFFF;
        std::uint16_t jp_ = 0;
        std::uint8_t range_ = 0;

        damage_overlay damage_map_ = {};

       public:
        power() = default;
        power(std::string, unit_job, power_class, std::uint16_t,
              damage_overlay) noexcept;

        power(std::string, unit_job, power_class, std::uint16_t, std::uint16_t,
              damage_overlay) noexcept;

        const std::string& name() const noexcept;
        power_class klass() const noexcept;
        unit_job type() const noexcept;
        std::uint16_t jp_req() const noexcept;
        std::uint16_t jp() const noexcept;
        const damage_overlay& overlay() const noexcept;

        bool is_melee() const noexcept;
        bool is_fired() const noexcept;
        bool is_magic() const noexcept;
    };

    struct equipment {
        using powers_container = std::vector<power>;
        std::string name;
        powers_container powers;
    };

    struct unit_status {
        std::uint16_t health = 0;
        std::uint16_t stamina = 0;
        std::uint16_t magica = 0;

        unit_status() noexcept = default;
        explicit unit_status(const unit_statistics&) noexcept;
        void regenerate(const unit_statistics&) noexcept;
    };

    class unit
    {
       public:
        using powers_container = std::vector<power>;

       private:
        std::size_t id_;
        unit_job job_;

        std::uint16_t level_;
        std::uint16_t exp_;

        unit_statistics statistics_;
        unit_status status_;

        equipment weapon_;
        powers_container powers_;

       public:
        unit() = default;
        unit(std::size_t,      //
             unit_job,         //
             std::uint16_t,    //
             std::uint16_t,    //
             unit_statistics,  //
             unit_status,      //
             equipment,        //
             powers_container  //
        );

        unit(std::size_t);

       public:
        std::uint16_t level() const noexcept;
        unit_job job() const noexcept;
        std::size_t id() const noexcept;

       public:  // move
        std::uint16_t hmove_penalty() const noexcept;
        std::uint16_t vmove_penalty() const noexcept;
        std::uint16_t vmove_max() const noexcept;
        std::uint16_t range() const noexcept;

       public:  // attack
        std::uint16_t experience() const noexcept;
        const unit_statistics& statistics() const noexcept;
        const unit_status& status() const noexcept;
        const powers_container& powers() const noexcept;
        const equipment& weapon() const noexcept;
        float defense() const noexcept;

        void use_weapon(powers_container::const_iterator, map&,
                        basic_map_index) noexcept;
        void use_power(equipment::powers_container::const_iterator, map&,
                       basic_map_index) noexcept;

       public:
        bool morph(unit_job) noexcept;
        void level_up() noexcept;

        void regenerate_status() noexcept;
        void reset_status() noexcept;
    };
}  // namespace hexagon::model

#endif
