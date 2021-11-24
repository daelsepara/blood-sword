#ifndef __ATTRIBUTES_HPP__
#define __ATTRIBUTES_HPP__

#include <map>

namespace Attributes
{
    enum class Type
    {
        None = 0,
        FightingProwess,
        PsychicAbility,
        Awareness,
        Endurance
    };

    std::map<Attributes::Type, const char*> Description = {
        {Attributes::Type::None, "NONE"},
        {Attributes::Type::FightingProwess, "FIGHTING PROWESS"},
        {Attributes::Type::PsychicAbility, "PSYCHIC ABILITY"},
        {Attributes::Type::Awareness, "AWARENESS"},
        {Attributes::Type::Endurance, "ENDURANCE"},
    };

    std::map<Attributes::Type, const char*> Abbreviation = {
        {Attributes::Type::None, "NONE"},
        {Attributes::Type::FightingProwess, "FPR"},
        {Attributes::Type::PsychicAbility, "PSY"},
        {Attributes::Type::Awareness, "AWR"},
        {Attributes::Type::Endurance, "END"},
    };

    class Base
    {
    public:
        Attributes::Type Type = Attributes::Type::None;

        int Value = 0;

        int Maximum = 0;

        Base()
        {
            Type = Attributes::Type::None;

            Value = 0;

            Maximum = 0;
        }

        Base(Attributes::Type type, int value)
        {
            Type = type;

            Value = value;

            Maximum = value;
        }

        Base(Attributes::Type type, int value, int maximum)
        {
            Type = type;

            Value = value;

            Maximum = maximum;
        }
    };

    enum class Stage
    {
        START = 0,
        TEST,
        END
    };

    enum class Result
    {
        NONE = 0,
        SUCCESS,
        FAILURE
    };
}
#endif