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
        NinjaAssassin,
        DirgeMan,
        Skiapyr,
        MagusVyl,
        IconTheUngodly,
        ManInBlue,
        Corpse,
        Bowmen,
        Dirges,
        Guillarme,
        Echidna,
        SkrymirTheGiant
    };

    class Base
    {
    public:
        Assets::Type Asset = Assets::Type::None;

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

        bool TargetAll = false;

        // player last attacked
        int Attacked = -1;

        // acts first before normal combat sequence
        bool ActFirst = false;

        std::vector<Spell::Status> SpellStatus = {};

        // round available
        int StartRound = 0;

        // start locations: Default for (-1, -1)
        int StartX = -1;

        int StartY = -1;

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

        Base(Enemy::Type type, const char *name, int fightingProwess, int psychicAbility, int awareness, int endurance, int damage, int damageModifier, int armour, Assets::Type asset, bool move, bool shoot)
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

            CanMove = move;

            CanShoot = shoot;
        }

        Base(Enemy::Type type, const char *name, int fightingProwess, int psychicAbility, int awareness, int endurance, int damage, int damageModifier, int armour, Assets::Type asset, bool move, bool shoot, bool target)
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

            CanMove = move;

            CanShoot = shoot;

            TargetAll = target;
        }

        Base(Enemy::Type type, const char *name, int fightingProwess, int psychicAbility, int awareness, int endurance, int damage, int damageModifier, int armour, Assets::Type asset, bool move, bool shoot, bool target, int round, int startx, int starty)
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

            CanMove = move;

            CanShoot = shoot;

            TargetAll = target;

            StartRound = round;

            StartX = startx;

            StartY = starty;
        }
    };
}
#endif
