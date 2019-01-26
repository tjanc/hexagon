//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <cassert>
#include <cmath>
#include <hexagon/model/unit.hpp>

using namespace hexagon::model;
namespace
{
    std::uint16_t health_regen_rate(const unit_statistics& stats) noexcept
    {
        return stats.presence / 10;
    }

    std::uint16_t stamina_regen_rate(const unit_statistics& stats) noexcept
    {
        return stats.endurance / 10;
    }

    std::uint16_t magica_regen_rate(const unit_statistics& stats) noexcept
    {
        return stats.intelligence / 10;
    }

    std::uint16_t max_stamina(const unit_statistics& stats) noexcept
    {
        return stats.strength + stats.agility + stats.endurance;
    }

    std::uint16_t max_magica(const unit_statistics& stats) noexcept
    {
        return stats.wisdom + stats.intelligence + stats.presence;
    }

    std::uint16_t max_health(const unit_statistics& stats) noexcept
    {
        return stats.strength + stats.endurance + stats.presence;
    }

    // float damage_factor(const power&, const unit_statistics&) noexcept
    // {
    //     return 1.0;
    // }
}  // namespace

namespace
{
    void add_level_bonus(std::uint16_t, unit_statistics& stats)
    {
        stats.agility += 10;
        stats.presence += 10;
        stats.strength += 10;
        stats.intelligence += 10;
        stats.wisdom += 10;
        stats.endurance += 10;
    }

    void add_job_bonus(std::uint16_t, unit_job jb, unit_statistics& stats)
    {
        switch (jb) {
            default:
                return;

            case unit_job::rogue:
                stats.agility += 5;
                return;
            case unit_job::acolyte:
                stats.presence += 5;
                return;
            case unit_job::warrior:
                stats.strength += 5;
                return;
            case unit_job::mage:
                stats.intelligence += 5;
                return;
            case unit_job::druid:
                stats.wisdom += 5;
                return;
            case unit_job::hunter:
                stats.endurance += 5;
                return;

            case unit_job::illusionist:
                stats.agility += 10;
                stats.presence += 10;
                return;
            case unit_job::paladin:
                stats.presence += 10;
                stats.strength += 10;
                return;
            case unit_job::warlock:
                stats.strength += 10;
                stats.intelligence += 10;
                return;
            case unit_job::animist:
                stats.intelligence += 10;
                stats.wisdom += 10;
                return;
            case unit_job::beastmaster:
                stats.wisdom += 10;
                stats.endurance += 10;
                return;
            case unit_job::assassin:
                stats.endurance += 10;
                stats.agility += 10;
                return;

            case unit_job::shadowwalker:
                stats.agility += 20;
                stats.presence += 10;
                return;
            case unit_job::prophet:
                stats.presence += 20;
                stats.intelligence += 10;
                return;
            case unit_job::berserker:
                stats.strength += 20;
                stats.endurance += 10;
                return;
            case unit_job::arcane:
                stats.intelligence += 20;
                stats.wisdom += 10;
                return;
            case unit_job::elementalist:
                stats.wisdom += 20;
                stats.strength += 10;
                return;
            case unit_job::sharpshooter:
                stats.endurance += 20;
                stats.agility += 10;
                return;
        }
    }

    // std::uint16_t needed_experience(std::uint16_t lvl)
    // {
    //     return std::pow(lvl, 1.6) + 100;
    // }

}  // namespace

namespace
{
    equipment make_fists() noexcept
    {
        return equipment{"Fists",
                         {power{"Hit", unit_job::none, power_class::none, 50,
                                circular_overlay(0, 10)}}};
    }
}  // namespace

namespace
{
    float damage_bonus_factor(std::uint16_t avg)
    {
        return 2.0 * ((avg / 500.0)) / ((avg / 500.0) + 2.0);
    }

    float damage_factor(const unit_statistics& stats, const power& p)
    {
        if (p.is_melee())
            return 1.0 +
                   damage_bonus_factor((stats.strength + stats.presence) / 2);
        if (p.is_fired())
            return 1.0 +
                   damage_bonus_factor((stats.endurance + stats.agility) / 2);
        if (p.is_magic())
            return 1.0 +
                   damage_bonus_factor((stats.intelligence + stats.wisdom) / 2);
        return 1.0;
    }
}  // namespace

unit_status::unit_status(const unit_statistics& stats) noexcept
    : health(max_health(stats)),
      stamina(max_stamina(stats)),
      magica(max_magica(stats))
{
}

void unit_status::regenerate(const unit_statistics& stats) noexcept
{
    const auto health_candidate = health + health_regen_rate(stats);
    const auto health_max = max_health(stats);
    health = health_candidate > health_max ? health_max : health_candidate;

    const auto stamina_candidate = stamina + stamina_regen_rate(stats);
    const auto stamina_max = max_stamina(stats);
    stamina = stamina_candidate > stamina_max ? stamina_max : stamina_candidate;

    const auto magica_candidate = magica + magica_regen_rate(stats);
    const auto magica_max = max_magica(stats);
    magica = magica_candidate > magica_max ? magica_max : magica_candidate;
}

unit::unit(std::size_t id)
    : id_{id},
      job_{},
      level_{},
      exp_{},
      statistics_{},
      status_{},
      weapon_{make_fists()},
      powers_{}
{
}

unit::unit(std::size_t id, unit_job job, std::uint16_t level, std::uint16_t exp,
           unit_statistics statistics, unit_status status, equipment weapon,
           powers_container powers)
    : id_{id},
      job_{job},
      level_{level},
      exp_{exp},
      statistics_{statistics},
      status_{status},
      weapon_{std::move(weapon)},
      powers_{std::move(powers)}
{
}

unit::unit(std::size_t id, unit_job j)
    : id_{id},
      job_{j},
      level_{},
      exp_{},
      statistics_{},
      status_{},
      weapon_{make_fists()},
      powers_{}
{
}

std::uint16_t unit::level() const noexcept { return level_; }

unit_job unit::job() const noexcept { return job_; }

bool unit::morph(unit_job) noexcept
{
    assert(false);
    return false;
}

void unit::level_up() noexcept
{
    add_level_bonus(level_, statistics_);
    add_job_bonus(level_, job_, statistics_);
    exp_ = 0;
    ++level_;
}

void unit::regenerate_status() noexcept { status_.regenerate(statistics_); }

void unit::reset_status() noexcept { status_ = unit_status{statistics_}; }

const unit::powers_container& unit::powers() const noexcept { return powers_; }

const equipment& unit::weapon() const noexcept { return weapon_; }

void unit::use_weapon(powers_container::const_iterator p, map& m,
                      basic_map_index t) noexcept
{
    // distribute_damage
    p->overlay().apply(m, t, [p, &s = statistics_](auto& tl, auto dmg) {  //
        unit* u = tl.get_if_unit();
        if (u) {
            auto& health = u->status_.health;
            health -= (dmg * damage_factor(s, *p)) / u->defense();
            if (health < 0) health = 0;
        }
    });

    // add xp on power

    // add xp on unit
}

void unit::use_power(equipment::powers_container::const_iterator, map&,
                     basic_map_index) noexcept
{
}

float unit::defense() const noexcept { return 1.0; }

namespace
{
    constexpr std::bitset<16> TYPE_MASK = 0x00FF;
    constexpr std::bitset<16> KLASS_MASK = 0xFF00;
}  // namespace

power::power(std::string name,      //
             unit_job jtype,        //
             power_class ptype,     //
             std::uint16_t jp_req,  //
             std::uint16_t jp,      //
             damage_overlay overlay) noexcept
    : name_{std::move(name)},
      flags_(static_cast<std::uint16_t>(jtype) |
             static_cast<std::uint16_t>(ptype)),
      jp_req_{jp_req},
      jp_{jp},
      damage_map_{std::move(overlay)}
{
}

power::power(std::string name,      //
             unit_job jtype,        //
             power_class ptype,     //
             std::uint16_t jp_req,  //
             damage_overlay overlay) noexcept
    : power{std::move(name), jtype, ptype, jp_req, 0, std::move(overlay)}
{
}

const std::string& power::name() const noexcept { return name_; }

std::uint16_t power::jp_req() const noexcept { return jp_req_; }

std::uint16_t power::jp() const noexcept { return jp_; }

const damage_overlay& power::overlay() const noexcept { return damage_map_; }

bool power::is_melee() const noexcept { return klass() == power_class::melee; }

bool power::is_fired() const noexcept { return klass() == power_class::fired; }

bool power::is_magic() const noexcept { return klass() == power_class::magic; }

power_class power::klass() const noexcept
{
    auto raw = (flags_ & KLASS_MASK).to_ulong();
    return static_cast<power_class>(raw);
}

unit_job power::type() const noexcept
{
    auto raw = (flags_ & TYPE_MASK).to_ulong();
    return static_cast<unit_job>(raw);
}

damage_overlay hexagon::model::circular_overlay(std::size_t n,
                                                std::uint16_t dmg) noexcept
{
    using tiles_type = damage_overlay::overlay_type::tiles_container;

    const auto width = n + 1 + n;
    const auto center = basic_map_index{n, n};

    tiles_type tiles(width * width, 0);
    damage_overlay::overlay_type m(std::move(tiles), width);
    m.at(n, n) = dmg;

    std::vector<basic_map_index> ids;
    ids.emplace_back(n, n);

    const auto enqueue = [&ids, &m, dmg](const auto idx, auto& new_ids,
                                         auto r) {
        if (contains(m, idx) && 0 == m.at(idx)) {
            m.at(idx) = dmg;
            if (r > 1) new_ids.emplace_back(idx);
        }
    };

    for (auto r = n; r != 0; --r) {
        std::vector<basic_map_index> new_ids;
        for (const auto& id : ids) {
            enqueue(west(id), new_ids, r);
            enqueue(north_west(id), new_ids, r);
            enqueue(north_east(id), new_ids, r);
            enqueue(east(id), new_ids, r);
            enqueue(south_east(id), new_ids, r);
            enqueue(south_west(id), new_ids, r);
        }
        ids = new_ids;
    }

    return damage_overlay{std::move(m), center};
}

std::uint16_t unit::hmove_penalty() const noexcept { return 70; }

std::uint16_t unit::vmove_penalty() const noexcept { return 50; }

std::uint16_t unit::vmove_max() const noexcept
{
    return 1 + statistics_.agility / 100;
}

std::uint16_t unit::range() const noexcept
{
    return statistics_.agility + statistics_.endurance;
}

std::uint16_t unit::experience() const noexcept { return exp_; }

const unit_statistics& unit::statistics() const noexcept { return statistics_; }

const unit_status& unit::status() const noexcept { return status_; }

std::size_t unit::id() const noexcept { return id_; }
