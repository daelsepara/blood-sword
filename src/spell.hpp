#ifndef __SPELL_HPP__
#define __SPELL_HPP__

#include <string>

namespace Spell
{
    enum class Class
    {
        None = 0,
        Blasting,
        Psychic
    };

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

    class Base
    {
    public:
        Spell::Class Class = Spell::Class::None;

        Spell::Type Type = Spell::Type::None;

        int Complexity = 1;

        std::string Name = "";

        std::string Description = "";

        int CurrentModifier = 0;

        Base(Spell::Class spellClass, Spell::Type type, int complexity, const char *name, const char *description)
        {
            Class = spellClass;

            Type = type;

            Complexity = complexity;

            Name = name;

            Description = description;

            CurrentModifier = 0;
        }
    };

    auto VolcanoSpray = Spell::Base(Spell::Class::Blasting, Spell::Type::VolcanoSpray, 1, "Volcano Spray", "Causes all enemies in the vicinity to lose one Die Endurance. This is a Blasting spell, so it cannot be resisted. The enemies' Armour Rating, if any, is deducted from the damage Die roll.");
    auto Nighthowl = Spell::Base(Spell::Class::Psychic, Spell::Type::Nighthowl, 1, "Nighthowl", "A Psychic spell that affects a single opponent. If the opponent fails to resist, he/she/it must make fight or shoot rolls using one Die more than usual (that is, on 3D rather than 2D) for the next four Rounds.");
    auto WhiteFire = Spell::Base(Spell::Class::Blasting, Spell::Type::WhiteFire, 1, "White Fire", "This Blasting spell strikes one opponent, causing the loss of 2D+2 Endurance (less Armour Rating).");
    auto Swordthrust = Spell::Base(Spell::Class::Blasting, Spell::Type::Swordthrust, 2, "Swordthrust", "A Blasting spell affecting one enemy, who loses 3D+3 Endurance; armour reduces the damage in the usual way.");
    auto EyeOfTheTiger = Spell::Base(Spell::Class::Psychic, Spell::Type::EyeOfTheTiger, 2, "Eye of the Tiger", "When this spell is cast, you can either add two to your Fighting Prowess and damage rolls or add one to the Fighting Prowess and damage rolls of everyone in the party including yourself. This lasts for four Rounds of combat.");
    auto ImmediateDeliverance = Spell::Base(Spell::Class::Psychic, Spell::Type::ImmediateDeliverance, 2, "Immediate Deliverance", "Used during a combat from which you wish to flee, this spell Teleports everyone in the party to the exit (if there is one). You are then ready to beat a retreat in the next Round.");
    auto MistsOfDeath = Spell::Base(Spell::Class::Psychic, Spell::Type::MistsOfDeath, 3, "Mists of Death", "All enemies in the vicinity lose 2D Endurance if they fail to resist this Psychic spell. Armour gives no protection.");
    auto TheVampireSpell = Spell::Base(Spell::Class::Psychic, Spell::Type::TheVampireSpell, 3, "The Vampire Spell", "This Psychic spell can be directed against a single foe, who loses four Dice Endurance if he fails to resist it. Some of the vital energy he loses is channelled into you: your own Endurance is increased by half the amount he loses (rounded down). Of course, your Endurance still cannot exceed its initial score.");
    auto SheetLightning = Spell::Base(Spell::Class::Blasting, Spell::Type::SheetLightning, 4, "Sheet Lightning", "A powerful Blasting spell that inflicts 2D+2 damage to all opponents in the vicinity. Armour protects from this as usual.");
    auto GhastlyTouch = Spell::Base(Spell::Class::Psychic, Spell::Type::GhastlyTouch, 4, "Ghastly Touch", "This is the only spell that requires you to be in an adjacent square to your intended victim. It is a Psychic spell that affects one opponent, who loses 7D Endurance if he fails to resist it - and 2D even if he does resist it. Armour gives no protection.");
    auto NemesisBolt = Spell::Base(Spell::Class::Blasting, Spell::Type::NemesisBolt, 5, "Nemesis Bolt", "This highly focused bolt of energy strikes one foe, who loses 7D+7 Endurance. It is a Blasting spell, so armour will reduce the damage.");
    auto ServileEnthralment = Spell::Base(Spell::Class::Psychic, Spell::Type::ServileEnthralment, 5, "Servile Enthralment", "This Psychic spell affects one enemy. If not resisted, it brings the enemy under your control. He/she/it simply stops moving and in non-combat situations may respond to your questions. If you order an Enthralled foe to fight for you (that is, against his own former companions), you must roll 1D: on a six he recovers his wits and attacks you. Enthralment lasts long enough for you to leave the vicinity, so you proceed as though you had slain the opponent in question.");

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