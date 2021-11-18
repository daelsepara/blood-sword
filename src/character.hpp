#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <map>
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

    std::map<Character::Class, const char *> Description = {
        {Class::None, "UNKNOWN"},
        {Class::Warrior, "WARRIOR"},
        {Class::Trickster, "TRICKSTER"},
        {Class::Sage, "SAGE"},
        {Class::Enchanter, "ENCHANTER"}};

    class Base
    {
    public:
        std::vector<Abilities::Type> Abilities = {};

        std::vector<Attributes::Base> Attributes = {};

        std::vector<Equipment::Base> Equipment = {};

        // Spells called to mind (only used by Enchanter character class)
        std::vector<Spell::Base> Spells = {};

        Character::Class Class = Character::Class::None;

        int Rank = 1;

        int ExperiencePoints = 0;

        // damage dice
        int Damage = 0;

        int DamageModifier = 0;

        // equipment Limit
        int Encumbrance = 10;

        // if defending during a combat round
        bool IsDefending = false;

        // if has escaped (flee from combat)
        bool Escaped = false;

        // has triggered quick thinking (get an action at the end of all others)
        bool QuickThinking = false;

        Base()
        {
        }

        Base(Character::Class character)
        {
            Class = character;
        }
    };

    void Warrior(Character::Base &character)
    {
        character.Attributes.clear();

        if (character.Rank == 2)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 12));

            character.Damage = 1;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 18));

            character.Damage = 1;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 24));

            character.Damage = 2;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 30));

            character.Damage = 2;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 36));

            character.Damage = 2;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 42));

            character.Damage = 3;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 48));

            character.Damage = 3;
            character.DamageModifier = 1;
        }
    }

    void Trickster(Character::Base &character)
    {
        character.Attributes.clear();

        if (character.Rank == 2)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 12));

            character.Damage = 1;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 18));

            character.Damage = 1;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 24));

            character.Damage = 1;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 30));

            character.Damage = 2;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 36));

            character.Damage = 2;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 42));

            character.Damage = 2;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 48));

            character.Damage = 3;
            character.DamageModifier = 0;
        }
    }

    void Sage(Character::Base &character)
    {
        character.Attributes.clear();

        if (character.Rank == 2)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 10));

            character.Damage = 1;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 15));

            character.Damage = 1;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 20));

            character.Damage = 1;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 25));

            character.Damage = 2;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 30));

            character.Damage = 2;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 35));

            character.Damage = 2;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 40));

            character.Damage = 3;
            character.DamageModifier = 0;
        }
    }

    void Enchanter(Character::Base &character)
    {
        character.Attributes.clear();

        if (character.Rank == 2)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 10));

            character.Damage = 1;
            character.DamageModifier = -1;
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 15));

            character.Damage = 1;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 20));

            character.Damage = 1;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 25));

            character.Damage = 1;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 30));

            character.Damage = 1;
            character.DamageModifier = 3;
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 35));

            character.Damage = 2;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, 40));

            character.Damage = 2;
            character.DamageModifier = 2;
        }
    }

    void Initialize(Character::Base &character, int rank)
    {
        // Set rank and check bounds
        character.Rank = rank;

        if (character.Rank < 2)
        {
            character.Rank = 2;
        }

        if (character.Rank > 13)
        {
            character.Rank = 13;
        }

        // Set starting experience points
        character.ExperiencePoints = (character.Rank - 1) * 250;

        // Set class-specific starting attributes and equipment and abilities
        if (character.Class == Character::Class::Warrior)
        {
            Character::Warrior(character);

            character.Equipment = {Equipment::Sword, Equipment::ChainMail};
            character.Equipment.push_back(Equipment::Base(Equipment::Type::MoneyPouch, "money-pouch", "money-pouch", character.Rank * 5, 100));
        }
        else if (character.Class == Character::Class::Trickster)
        {
            Character::Trickster(character);

            character.Equipment = {Equipment::Sword, Equipment::Bow, Equipment::StuddedLeather};
            character.Equipment.push_back(Equipment::Base(Equipment::Type::MoneyPouch, "money-pouch", "money-pouch", character.Rank * 5, 100));
            character.Equipment.push_back(Equipment::Base(Equipment::Type::Quiver, "quiver", "quiver", 6, 6));

            character.Abilities = {Abilities::Type::Dodge, Abilities::Type::Archery, Abilities::Type::QuickThinking};
        }
        else if (character.Class == Character::Class::Sage)
        {
            Character::Sage(character);

            character.Equipment = {Equipment::Quarterstaff, Equipment::Bow, Equipment::RingMail};
            character.Equipment.push_back(Equipment::Base(Equipment::Type::MoneyPouch, "money-pouch", "money-pouch", character.Rank * 5, 100));
            character.Equipment.push_back(Equipment::Base(Equipment::Type::Quiver, "quiver", "quiver", 6, 6));

            character.Abilities = {Abilities::Type::Archery, Abilities::Type::Quarterstaff, Abilities::Type::Healing, Abilities::Type::ESP, Abilities::Type::ParanormalSight, Abilities::Type::Levitation};
        }
        else if (character.Class == Character::Class::Enchanter)
        {
            Character::Enchanter(character);

            character.Equipment = {Equipment::Sword, Equipment::Silver};
            character.Equipment.push_back(Equipment::Base(Equipment::Type::MoneyPouch, "money-pouch", "money-pouch", character.Rank * 5, 100));
        }
    }

    Character::Base Create(Character::Class type, int rank)
    {
        auto character = Character::Base(type);

        Character::Initialize(character, rank);

        return character;
    }
}
#endif