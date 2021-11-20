#ifndef __ABILITIES_HPP__
#define __ABILITIES_HPP__

#include <map>

namespace Abilities
{
    enum class Type
    {
        None = 0,
        Dodge,
        Archery,
        QuickThinking,
        Quarterstaff,
        Healing,
        Excorcism,
        ESP,
        ParanormalSight,
        Levitation
    };

    std::map<Abilities::Type, const char *> Description = {
        {Abilities::Type::None, "None"},
        {Abilities::Type::Dodge, "Dodge"},
        {Abilities::Type::Archery, "Archery"},
        {Abilities::Type::QuickThinking, "Quick Thinking"},
        {Abilities::Type::Quarterstaff, "Quarterstaff"},
        {Abilities::Type::Healing, "Healing"},
        {Abilities::Type::Excorcism, "Excorcism"},
        {Abilities::Type::ESP, "ESP"},
        {Abilities::Type::ParanormalSight, "Paranormal Sight"},
        {Abilities::Type::Levitation, "Levitation"}};
}
#endif