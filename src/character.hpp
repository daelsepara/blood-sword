#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <vector>

#include "abilities.hpp"
#include "attributes.hpp"
#include "equipment.hpp"
#include "spell.hpp"

namespace Character
{
    enum class Class
    {
        None = 0,
        Warrior,
        Trickster,
        Sage,
        Enchanter
    };

    class Base
    {
    public:
        std::vector<Abilities::Type> Abilities = {};

        std::vector<Attributes::Base> Attributes = {};

        std::vector<Equipment::Base> Equipment = {};

        // Spells called to mind (Only used by Enchanter Character Class)
        std::vector<Spell::Base> Spells = {};

        Character::Class Class = Character::Class::None;

        int Rank = 1;

        int ExperiencePoints = 0;

        // Damage Dice
        int Damage = 0;

        int DamageModifier = 0;

        // Equipment Limit
        int Encumbrance = 10;
    };
}
#endif