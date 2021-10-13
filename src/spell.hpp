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
        Volcano,
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
    };
}
#endif