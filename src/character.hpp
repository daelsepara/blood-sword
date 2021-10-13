#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <vector>

#include "attributes.hpp"

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
        int Rank = 0;

        int ExperiencePoints = 0;

        std::vector<Attributes::Base> Attributes = std::vector<Attributes::Base>();

        int Damage = 0;

        int DamageModifier = 0;

        // Equipment Limit
        int Encumbrance = 10;
    };
}
#endif