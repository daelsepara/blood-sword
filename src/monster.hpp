#ifndef __MONSTER_HPP__
#define __MONSTER_HPP__

#include <string>

#include "spell.hpp"

namespace Monster
{
    enum class Type
    {
        None = 0,
        Barbarian
    };

    class Base
    {
    public:
        Monster::Type Type = Monster::Type::None;

        std::string Name = "";

        int FightingProwess = 0;

        int PsychicAbility = 0;

        int Awareness = 0;

        int Endurance = 0;

        int Damage = 0;

        int DamageModifier = 0;

        int Armour = 0;

        bool KnockedOff = false;

        bool Enthraled = false;

        std::vector<Spell::Status> SpellStatus = {};

        Base()
        {
        }

        Base(Monster::Type type, const char *name, int fightingProwess, int psychicAbility, int awareness, int endurance, int damage, int damageModifier, int armour)
        {
            Type = type;

            Name = name;

            FightingProwess = fightingProwess;

            PsychicAbility = psychicAbility;

            Awareness = awareness;

            Endurance = endurance;

            Damage = damage;

            DamageModifier = damageModifier;

            Armour = armour;
        }
    };
}
#endif