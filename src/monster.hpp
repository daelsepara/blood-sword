#ifndef __MONSTER_HPP__
#define __MONSTER_HPP__

#include <string>
#include <vector>

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

        int FightingProwess = 0;

        int PsychicAbility = 0;

        int Awareness = 0;

        int Endurance = 0;

        int Damage = 0;

        int DamageModifier = 0;
    };
}
#endif