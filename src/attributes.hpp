#ifndef __ATTRIBUTES_HPP__
#define __ATTRIBUTES_HPP__

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

        Base(Attributes::Type type, int value, int maximum)
        {
            Type = type;

            Value = value;

            Maximum = maximum;
        }
    };
}
#endif