#ifndef __ABILITIES_HPP__
#define __ABILITIES_HPP__

#include <map>

namespace Abilities
{
    enum class Type
    {
        None = 0,
        Dodging,
        Archery,
        QuickThinking,
        Quarterstaff,
        Healing,
        Exorcism,
        ESP,
        ParanormalSight,
        Levitation,
        CallToMind,
        CastSpell,
        Ambidextrousness,
        UnarmedMartialArts
    };

    std::map<Abilities::Type, const char *> Description = {
        {Abilities::Type::None, "None"},
        {Abilities::Type::Dodging, "Dodging"},
        {Abilities::Type::Archery, "Archery"},
        {Abilities::Type::QuickThinking, "Quick Thinking"},
        {Abilities::Type::Quarterstaff, "Quarterstaff"},
        {Abilities::Type::Healing, "Healing"},
        {Abilities::Type::Exorcism, "Exorcism"},
        {Abilities::Type::ESP, "ESP"},
        {Abilities::Type::ParanormalSight, "Paranormal Sight"},
        {Abilities::Type::Levitation, "Levitation"},
        {Abilities::Type::CallToMind, "Call to mind a spell"},
        {Abilities::Type::CastSpell, "Cast a spell"},
        {Abilities::Type::Ambidextrousness, "Ambidextrousness"},
        {Abilities::Type::UnarmedMartialArts, "Unarmed Martial Arts"}};
}
#endif