#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "party.hpp"
#include "random.hpp"

namespace Engine
{
    auto Random = Random::Base();

    void Randomize()
    {
        Engine::Random.UniformDistribution(1, 6);
    }

    // For Die rolling
    int Roll(int count, int modifier)
    {
        auto result = 0;

        for (auto i = 0; i < count; i++)
        {
            result += Engine::Random.NextInt();
        }

        result += modifier;

        if (result < 0)
        {
            result = 0;
        }

        return result;
    }

    enum class CombatActionType
    {
        None = 0,
        Move,
        Fight,
        Defend,
        Shoot,
        Flee,
        Memorize,
        Cast
    };

    int Find(Character::Base &character, Attributes::Type type)
    {
        auto found = -1;

        for (auto i = 0; i < character.Attributes.size(); i++)
        {
            if (character.Attributes[i].Type == type)
            {
                found = i;
            }
        }

        return found;
    };

    int Score(Character::Base &character, Attributes::Type type)
    {
        auto index = Engine::Find(character, type);

        if (index >= 0 && index < character.Attributes.size())
        {
            return character.Attributes[index].Value;
        }
        else
        {
            return 0;
        }
    };

    void Score(Character::Base &character, Attributes::Type type, int value)
    {
        auto index = Engine::Find(character, type);

        if (index >= 0 && index < character.Attributes.size())
        {
            character.Attributes[index].Value = value;
        }
    };

    int Maximum(Character::Base &character, Attributes::Type type)
    {
        auto index = Engine::Find(character, type);

        if (index >= 0 && index < character.Attributes.size())
        {
            return character.Attributes[index].Maximum;
        }
        else
        {
            return 0;
        }
    };

    void Maximum(Character::Base &character, Attributes::Type type, int value)
    {
        auto index = Engine::Find(character, type);

        if (index >= 0 && index < character.Attributes.size())
        {
            character.Attributes[index].Maximum = value;
        }
    };

    void Gain(Character::Base &character, Attributes::Type type, int value)
    {
        auto index = Engine::Find(character, type);

        if (index >= 0 && index < character.Attributes.size())
        {
            character.Attributes[index].Value += value;

            if (character.Attributes[index].Value > character.Attributes[index].Maximum)
            {
                character.Attributes[index].Value = character.Attributes[index].Maximum;
            }

            if (character.Attributes[index].Value < 0)
            {
                character.Attributes[index].Value = 0;
            }
        }
    }
}
#endif