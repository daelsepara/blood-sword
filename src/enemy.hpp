#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include <string>
#include <vector>

#include "character.hpp"

namespace Enemy
{
    enum class Type
    {
        None = 0,
        Barbarian,
        NinjaAssassin
    };

    class Base
    {
    public:
        Enemy::Type Type = Enemy::Type::None;

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

        bool CanMove = true;

        bool CanShoot = false;

        std::vector<Spell::Status> SpellStatus = {};

        Assets::Type Asset = Assets::Type::None;

        Base()
        {
        }

        Base(Enemy::Type type, const char *name, int fightingProwess, int psychicAbility, int awareness, int endurance, int damage, int damageModifier, int armour, Assets::Type asset)
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

            Asset = asset;
        }
    };
}
#endif