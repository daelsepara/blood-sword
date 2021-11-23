#ifndef __SPELL_HPP__
#define __SPELL_HPP__

#include <map>
#include <string>
#include <vector>

#include "assets.hpp"

namespace Spell
{
    enum class Class
    {
        None = 0,
        Blasting,
        Psychic
    };

    std::map<Spell::Class, const char *> ClassDescription = {
        {Spell::Class::None, "NONE"},
        {Spell::Class::Blasting, "BLASTING"},
        {Spell::Class::Psychic, "PSYCHIC"}};

    enum class Type
    {
        None = 0,
        VolcanoSpray,
        Nighthowl,
        WhiteFire,
        Swordthrust,
        EyeOfTheTiger,
        ImmediateDeliverance,
        MistsOfDeath,
        TheVampireSpell,
        SheetLightning,
        GhastlyTouch,
        NemesisBolt,
        ServileEnthralment,
        SummonFaltyn,
        Prediction,
        DetectSpells
    };

    enum class Stage
    {
        START,
        CAST,
        END // end of attack
    };

    enum class Result
    {
        NONE = 0,
        SUCCESS,
        FAILURE
    };

    // Type of spell, round when cast, duration, bonus
    typedef std::tuple<Spell::Type, int, int, int> Status;

    class Base
    {
    public:
        Spell::Class Class = Spell::Class::None;

        Spell::Type Type = Spell::Type::None;

        int Complexity = 1;

        std::string Name = "";

        std::string Description = "";

        int Difficulty = 0;

        bool RequiresTarget = false;

        Assets::Type Asset = Assets::Type::None;

        Base(Spell::Class spellClass, Spell::Type type, int complexity, const char *name, const char *description, Assets::Type asset, bool requiresTarget)
        {
            Class = spellClass;

            Type = type;

            Complexity = complexity;

            Name = name;

            Description = description;

            Difficulty = complexity;

            Asset = asset;

            RequiresTarget = requiresTarget;
        }
    };

    auto VolcanoSpray = Spell::Base(Spell::Class::Blasting, Spell::Type::VolcanoSpray, 1, "Volcano Spray", "Causes all enemies in the vicinity to lose 1D endurance. This is a Blasting spell, so it cannot be resisted. The enemies' Armour Rating, if any, is deducted from the damage roll.", Assets::Type::VolcanoSpray, false);
    auto Nighthowl = Spell::Base(Spell::Class::Psychic, Spell::Type::Nighthowl, 1, "Nighthowl", "A Psychic spell that affects a single opponent. If the opponent fails to resist, he/she/it must make fight or shoot rolls using 1D more than usual (that is, on 3D rather than 2D) for the next four rounds.", Assets::Type::Nighthowl, true);
    auto WhiteFire = Spell::Base(Spell::Class::Blasting, Spell::Type::WhiteFire, 1, "White Fire", "This Blasting spell strikes one opponent, causing the loss of 2D+2 Endurance (less armour rating).", Assets::Type::WhiteFire, true);
    auto Swordthrust = Spell::Base(Spell::Class::Blasting, Spell::Type::Swordthrust, 2, "Swordthrust", "A Blasting spell affecting one enemy, who loses 3D+3 Endurance; armour reduces the damage in the usual way.", Assets::Type::Swordthrust, true);
    auto EyeOfTheTiger = Spell::Base(Spell::Class::Psychic, Spell::Type::EyeOfTheTiger, 2, "Eye of the Tiger", "When this spell is cast, you can either add +2 to your Fighting Prowess and damage rolls or add +1 to the Fighting Prowess and damage rolls of everyone in the party including yourself. This lasts for four rounds of combat.", Assets::Type::EyeOfTheTiger, false);
    auto ImmediateDeliverance = Spell::Base(Spell::Class::Psychic, Spell::Type::ImmediateDeliverance, 2, "Immediate Deliverance", "Used during a combat from which you wish to flee, this spell teleports everyone in the party to the exit (if there is one). You are then ready to beat a retreat in the next round.", Assets::Type::ImmediateDeliverance, false);
    auto MistsOfDeath = Spell::Base(Spell::Class::Psychic, Spell::Type::MistsOfDeath, 3, "Mists of Death", "All enemies in the vicinity lose 2D Endurance if they fail to resist this Psychic spell. Armour gives no protection.", Assets::Type::MistsOfDeath, false);
    auto TheVampireSpell = Spell::Base(Spell::Class::Psychic, Spell::Type::TheVampireSpell, 3, "The Vampire Spell", "This Psychic spell can be directed against a single foe, who loses 4D endurance if he fails to resist it. Some of the vital energy he loses is channelled into you: your own Endurance is increased by half the amount he loses (rounded down). Of course, your Endurance still cannot exceed its initial score.", Assets::Type::TheVampireSpell, true);
    auto SheetLightning = Spell::Base(Spell::Class::Blasting, Spell::Type::SheetLightning, 4, "Sheet Lightning", "A powerful Blasting spell that inflicts 2D+2 damage to all opponents in the vicinity. Armour protects from this as usual.", Assets::Type::SheetLightning, false);
    auto GhastlyTouch = Spell::Base(Spell::Class::Psychic, Spell::Type::GhastlyTouch, 4, "Ghastly Touch", "This is the only spell that requires you to be in an adjacent square to your intended victim. It is a Psychic spell that affects one opponent, who loses 7D endurance if he fails to resist it - and 2D even if he does resist it. Armour gives no protection.", Assets::Type::GhastlyTouch, true);
    auto NemesisBolt = Spell::Base(Spell::Class::Blasting, Spell::Type::NemesisBolt, 5, "Nemesis Bolt", "This highly focused bolt of energy strikes one foe, who loses 7D+7 Endurance. It is a Blasting spell, so armour will reduce the damage.", Assets::Type::NemesisBolt, true);
    auto ServileEnthralment = Spell::Base(Spell::Class::Psychic, Spell::Type::ServileEnthralment, 5, "Servile Enthralment", "This Psychic spell affects one enemy. If not resisted, it brings the enemy under your control. He/she/it simply stops moving and in non-combat situations may respond to your questions. If you order an Enthralled foe to fight for you (that is, against his own former companions), you must roll 1D: on a six he recovers his wits and attacks you. Enthralment lasts long enough for you to leave the vicinity, so you proceed as though you had slain the opponent in question.", Assets::Type::ServileEnthralment, true);

    std::vector<Spell::Base> All = {
        Spell::VolcanoSpray,
        Spell::Nighthowl,
        Spell::WhiteFire,
        Spell::Swordthrust,
        Spell::EyeOfTheTiger,
        Spell::ImmediateDeliverance,
        Spell::MistsOfDeath,
        Spell::TheVampireSpell,
        Spell::SheetLightning,
        Spell::GhastlyTouch,
        Spell::NemesisBolt,
        Spell::ServileEnthralment};
}
#endif