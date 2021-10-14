#ifndef __MONSTER_HPP__
#define __MONSTER_HPP__

#include <string>

namespace Monster
{
    enum class Type
    {
        None = 0
    };

    class Base
    {
    public:
        Monster::Type Type = Monster::Type::None;
        
        std::string Name = "";

        std::string Image = "";

        int FightingProwess = 0;

        int PsychicAbility = 0;

        int Awareness = 0;

        int Endurance = 0;

        int Damage = 0;

        int DamageModifier = 0;

        Base()
        {

        }

        Base(Monster::Type type, const char *name, const char *image, int fightingProwess, int psychicAbility, int awareness, int endurance, int damage, int damageModifier)
        {
            Type = type;

            Name = name;

            Image = image;

            FightingProwess = fightingProwess;

            PsychicAbility = psychicAbility;

            Awareness = awareness;

            Endurance = endurance;

            Damage = damage;

            DamageModifier = damageModifier;
        }
    };
}
#endif