#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <map>
#include <vector>

#include "abilities.hpp"
#include "attributes.hpp"
#include "codeword.hpp"
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
        Enchanter,
        Imragarn
    };

    std::map<Character::Class, const char *> ClassName = {
        {Character::Class::None, "UNKNOWN"},
        {Character::Class::Warrior, "WARRIOR"},
        {Character::Class::Trickster, "TRICKSTER"},
        {Character::Class::Sage, "SAGE"},
        {Character::Class::Enchanter, "ENCHANTER"},
        {Character::Class::Imragarn, "IMRAGARN"}};

    std::map<Character::Class, const char *> Description = {
        {Character::Class::None, "Records Expunged"},
        {Character::Class::Warrior, "You are the master of the fighting arts. You have better Fighting Prowess than any other adventuring type at the same rank, and when you strike a blow you inflict more damage. These advantages give you a real edge in any fight, but you do not get things all your own way. Also, because you are of noble birth and follow the honourable traditions of your ancestors, you must be careful to stay true to the code of chivalry."},
        {Character::Class::Trickster, "Some Adventurers are honest, chivalrous and honourable. Not you. You are basically a rogue -- a likeable rogue, perhaps, but a rogue nonetheless. You live by your wits. If you can win a fight by trickery or by shooting someone in the back, you will. Cunning is your main weapon. But when you have to face someone in a straight fight, you are no pushover."},
        {Character::Class::Sage, "Your upbringing has been in the spartan Monastery of Illumination on the barren island of Kaxos. There you studied the Mystic Way -- a series of demanding psionic disciplines and rigorous physical training."},
        {Character::Class::Enchanter, "Forget the mundane arts of swordplay. You can use a sword if you have to, but your true forte is in the manipulation of occult powers of Sorcery."},
        {Character::Class::Imragarn, "He's been frozen in ice for almost a decade, since he entered the Battlepits with several companions as the champions of Magus Laglor."}};

    std::map<Character::Class, std::vector<Abilities::Type>> Abilities = {
        {Character::Class::None, {}},
        {Character::Class::Warrior, {Abilities::Type::Ambidextrousness, Abilities::Type::UnarmedMartialArts}},
        {Character::Class::Trickster, {Abilities::Type::Archery, Abilities::Type::Dodging, Abilities::Type::QuickThinking}},
        {Character::Class::Sage, {Abilities::Type::Archery, Abilities::Type::Quarterstaff, Abilities::Type::Healing, Abilities::Type::Exorcism, Abilities::Type::ESP, Abilities::Type::ParanormalSight, Abilities::Type::Levitation}},
        {Character::Class::Enchanter, {Abilities::Type::CallToMind, Abilities::Type::CastSpell}},
        {Character::Class::Imragarn, {}}};

    class Base
    {
    public:
        std::vector<Abilities::Type> Abilities = {};

        std::vector<Attributes::Base> Attributes = {};

        std::vector<Equipment::Base> Equipment = {};

        // Spells called to mind (only used by Enchanter character class)
        std::vector<Spell::Base> Spells = {};

        std::vector<Code::Status> Codes = {};

        Character::Class Class = Character::Class::None;

        int Rank = 1;

        int ExperiencePoints = 0;

        // damage dice
        int Damage = 0;

        int DamageModifier = 0;

        // equipment Limit
        int Encumbrance = 10;

        // if defending during a combat round
        bool Defending = false;

        // if has escaped (flee from combat)
        bool Escaped = false;

        // has triggered quick thinking (get an action at the end of all others)
        bool QuickThinking = false;

        bool UsedQuickThinking = false;

        std::vector<Spell::Status> SpellStatus = {};

        Assets::Type Asset = Assets::Type::None;

        // acts first before normal combat sequence
        bool ActFirst = false;

        // shoot first before normal combat sequence
        bool ShootFirst = false;

        // paralyzed characters can no longer take part in combat
        bool Paralyzed = false;

        // engaged in combat
        bool Engaged = false;

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

        if (character.Rank <= 2)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Damage = 1;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Damage = 1;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 2;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 2;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 2;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 3;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 3;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 9)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 3;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 10)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 4;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 11)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 4;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 12)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 4;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 13)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 5;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 14)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 5;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 15)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 5;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 16)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 6;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 17)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 6;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 18)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 6;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 19)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 7;
            character.DamageModifier = 0;
        }
        else if (character.Rank >= 20)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 11));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 7;
            character.DamageModifier = 1;
        }

        character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, character.Rank * 6));
    }

    void Trickster(Character::Base &character)
    {
        character.Attributes.clear();

        if (character.Rank <= 2)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 1;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 1;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 1;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 2;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 2;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 2;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 3;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 9)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 3;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 10)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 3;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 11)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 4;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 12)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 4;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 13)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 4;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 14)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 10));
            character.Damage = 5;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 15)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 10));
            character.Damage = 5;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 16)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 10));
            character.Damage = 5;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 17)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 10));
            character.Damage = 6;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 18)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 10));
            character.Damage = 6;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 19)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 10));
            character.Damage = 6;
            character.DamageModifier = 2;
        }
        else if (character.Rank >= 20)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 11));
            character.Damage = 7;
            character.DamageModifier = 0;
        }

        character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, character.Rank * 6));
    }

    void Sage(Character::Base &character)
    {
        character.Attributes.clear();

        if (character.Rank <= 2)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Damage = 1;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Damage = 1;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 1;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 2;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 2;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 2;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 3;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 9)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 3;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 10)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 3;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 11)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 4;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 12)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 4;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 13)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 4;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 14)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 5;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 15)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 5;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 16)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 5;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 17)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 6;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 18)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 6;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 19)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 6;
            character.DamageModifier = 2;
        }
        else if (character.Rank >= 20)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 10));
            character.Damage = 7;
            character.DamageModifier = 0;
        }

        character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, character.Rank * 5));
    }

    void Enchanter(Character::Base &character)
    {
        character.Attributes.clear();

        if (character.Rank <= 2)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Damage = 1;
            character.DamageModifier = -1;
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 6));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Damage = 1;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 6));
            character.Damage = 1;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 1;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 1;
            character.DamageModifier = 3;
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 2;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 7));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 2;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 9)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 2;
            character.DamageModifier = 3;
        }
        else if (character.Rank == 10)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 3;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 11)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 7));
            character.Damage = 3;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 12)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 3;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 13)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 3;
            character.DamageModifier = 3;
        }
        else if (character.Rank == 14)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 8));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 4;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 15)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 4;
            character.DamageModifier = 1;
        }
        else if (character.Rank == 16)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 4;
            character.DamageModifier = 2;
        }
        else if (character.Rank == 17)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 8));
            character.Damage = 4;
            character.DamageModifier = 3;
        }
        else if (character.Rank == 18)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 5;
            character.DamageModifier = 0;
        }
        else if (character.Rank == 19)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 10));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 5;
            character.DamageModifier = 1;
        }
        else if (character.Rank >= 20)
        {
            character.Attributes.push_back(Attributes::Base(Attributes::Type::FightingProwess, 9));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::PsychicAbility, 11));
            character.Attributes.push_back(Attributes::Base(Attributes::Type::Awareness, 9));
            character.Damage = 5;
            character.DamageModifier = 2;
        }

        character.Attributes.push_back(Attributes::Base(Attributes::Type::Endurance, character.Rank * 5));
    }

    void Initialize(Character::Base &character, Book::Type book, int rank)
    {
        // set rank and enforce bounds
        character.Rank = rank;

        character.Rank = std::min(20, character.Rank);

        character.Rank = std::max(0, character.Rank);

        // set starting experience points
        character.ExperiencePoints = (character.Rank - 1) * 250;

        // set class-specific starting attributes and equipment and abilities
        if (character.Class == Character::Class::Warrior)
        {
            Character::Warrior(character);

            character.Equipment = {Equipment::Sword, Equipment::ChainMail};
            character.Equipment.push_back(Equipment::Base(Equipment::Class::MoneyPouch, "money-pouch", "money-pouch", character.Rank * 5, 100));
            character.Abilities = Character::Abilities[Character::Class::Warrior];
            character.Asset = Assets::Type::Warrior;
        }
        else if (character.Class == Character::Class::Trickster)
        {
            Character::Trickster(character);

            character.Equipment = {Equipment::Sword, Equipment::Bow, Equipment::StuddedLeather};
            character.Equipment.push_back(Equipment::Base(Equipment::Class::MoneyPouch, "money-pouch", "money-pouch", character.Rank * 5, 100));
            character.Equipment.push_back(Equipment::Base(Equipment::Class::Quiver, "quiver", "quiver", 6, 6));
            character.Abilities = Character::Abilities[Character::Class::Trickster];
            character.Asset = Assets::Type::Trickster;
        }
        else if (character.Class == Character::Class::Sage)
        {
            Character::Sage(character);

            character.Equipment = {Equipment::Quarterstaff, Equipment::Bow, Equipment::RingMail};
            character.Equipment.push_back(Equipment::Base(Equipment::Class::MoneyPouch, "money-pouch", "money-pouch", character.Rank * 5, 100));
            character.Equipment.push_back(Equipment::Base(Equipment::Class::Quiver, "quiver", "quiver", 6, 6));
            character.Abilities = Character::Abilities[Character::Class::Sage];
            character.Asset = Assets::Type::Sage;
        }
        else if (character.Class == Character::Class::Enchanter)
        {
            Character::Enchanter(character);

            character.Equipment = {Equipment::Sword, Equipment::Silver};
            character.Equipment.push_back(Equipment::Base(Equipment::Class::MoneyPouch, "money-pouch", "money-pouch", character.Rank * 5, 100));
            character.Abilities = Character::Abilities[Character::Class::Enchanter];
            character.Asset = Assets::Type::Enchanter;
        }
        else if (character.Class == Character::Class::Imragarn)
        {
            Character::Warrior(character);

            character.Equipment = {Equipment::Sword, Equipment::LeatherArmour};
            character.Abilities = Character::Abilities[Character::Class::Warrior];
            character.Asset = Assets::Type::Adventurer;
        }
    }

    Character::Base Create(Character::Class type, Book::Type book, int rank)
    {
        auto character = Character::Base(type);

        Character::Initialize(character, book, rank);

        return character;
    }
}
#endif
