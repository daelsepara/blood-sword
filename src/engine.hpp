#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "enemy.hpp"
#include "party.hpp"
#include "random.hpp"

namespace Engine
{
    auto Random = Random::Base();

    void Randomize()
    {
        Engine::Random.UniformIntDistribution(1, 6);

        Engine::Random.UniformDistribution(0, 1.0);
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

    // list of equipment with the attribute and sorted in descending order
    std::vector<Equipment::Base> Equipment(Character::Base &character, Attributes::Type attribute, bool IsWeapon)
    {
        std::vector<Equipment::Base> equipment = {};

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Attribute == attribute && (character.Equipment[i].Type == Equipment::Type::Normal || (IsWeapon && character.Equipment[i].Type == Equipment::Type::Weapon && character.Equipment[i].Weapon != Equipment::Weapon::Bow)))
            {
                equipment.push_back(character.Equipment[i]);
            }
        }

        if (equipment.size() > 1)
        {
            std::sort(equipment.begin(), equipment.end(), [attribute](Equipment::Base &a, Equipment::Base &b) -> bool
                      { return (a.Attribute == attribute) && a.Score > b.Score; });
        }

        return equipment;
    }

    // list of weapons with the attributes and sorted in descending order
    std::vector<Equipment::Base> Weapons(Character::Base &character)
    {
        return Engine::Equipment(character, Attributes::Type::FightingProwess, true);
    }

    int FightingProwess(Character::Base &character)
    {
        return Engine::Score(character, Attributes::Type::FightingProwess);
    }

    int PsychicAbility(Character::Base &character)
    {
        return Engine::Score(character, Attributes::Type::PsychicAbility);
    }

    int Endurance(Character::Base &character)
    {
        return Engine::Score(character, Attributes::Type::Endurance);
    }

    int Awareness(Character::Base &character)
    {
        return Engine::Score(character, Attributes::Type::Awareness);
    }

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
            character.Attributes[index].Value = std::min(character.Attributes[index].Value, character.Attributes[index].Maximum);
            character.Attributes[index].Value = std::max(0, character.Attributes[index].Value);
        }
    }

    void Gain(Enemy::Base &enemy, int damage)
    {
        enemy.Endurance += damage;

        enemy.Endurance = std::max(0, enemy.Endurance);
    }

    int Armour(Character::Base &character)
    {
        auto armour = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Type == Equipment::Type::Armour && character.Equipment[i].Rating > armour)
            {
                armour = character.Equipment[i].Rating;
            }
        }

        return armour;
    }

    bool IsAlive(Character::Base &character)
    {
        return (Engine::Score(character, Attributes::Type::Endurance) > 0);
    }

    bool IsAlive(Party::Base &party)
    {
        auto result = false;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]))
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool Escaped(Party::Base &party)
    {
        auto result = true;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]) && !party.Members[i].Escaped)
            {
                result = false;

                break;
            }
        }

        return result;
    }

    bool IsAlive(Enemy::Base &enemy)
    {
        return (enemy.Endurance > 0);
    }

    bool IsAlive(std::vector<Enemy::Base> &enemys)
    {
        auto result = false;

        for (auto i = 0; i < enemys.size(); i++)
        {
            if (Engine::IsAlive(enemys[i]))
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool HasAbility(Character::Base &character, Abilities::Type ability)
    {
        auto result = false;

        for (auto i = 0; i < character.Abilities.size(); i++)
        {
            if (character.Abilities[i] == ability)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool HasWeapon(Character::Base &character)
    {
        auto result = false;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Type == Equipment::Type::Weapon && character.Equipment[i].Weapon != Equipment::Weapon::Bow)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool HasWeapon(Character::Base &character, Equipment::Weapon weapon)
    {
        auto result = false;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Type == Equipment::Type::Weapon && character.Equipment[i].Weapon == weapon)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool CanShoot(Character::Base &character)
    {
        return Engine::HasAbility(character, Abilities::Type::Archery);
    }

    bool HasBow(Character::Base &character)
    {
        auto result = false;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Type == Equipment::Type::Weapon && character.Equipment[i].Weapon == Equipment::Weapon::Bow)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    int CountArrows(Character::Base &character)
    {
        auto result = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Type == Equipment::Type::Quiver && character.Equipment[i].Arrows > 0)
            {
                result += character.Equipment[i].Arrows;
            }
        }

        return result;
    }

    bool HasArrows(Character::Base &character)
    {
        return Engine::CountArrows(character) > 0;
    }

    bool ShootArrow(Character::Base &character)
    {
        auto result = false;

        if (Engine::HasBow(character) && Engine::HasArrows(character))
        {
            for (auto i = 0; i < character.Equipment.size(); i++)
            {
                if (character.Equipment[i].Type == Equipment::Type::Quiver && character.Equipment[i].Arrows > 0)
                {
                    character.Equipment[i].Arrows--;

                    result = true;

                    break;
                }
            }
        }

        return result;
    }

    bool QuickThinking(Party::Base &party)
    {
        auto result = false;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]) && party.Members[i].QuickThinking)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool KnockedOff(std::vector<Enemy::Base> &enemys)
    {
        auto result = false;

        for (auto i = 0; i < enemys.size(); i++)
        {
            if (enemys[i].Endurance > 0 && enemys[i].KnockedOff)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    int CountMoney(Character::Base &character)
    {
        auto result = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Type == Equipment::Type::MoneyPouch && character.Equipment[i].Gold > 0)
            {
                result += character.Equipment[i].Gold;
            }
        }

        return result;
    }

    bool HaveMoney(Character::Base &character)
    {
        return Engine::CountMoney(character) > 0;
    }

    void ClearDefendingStatus(Party::Base &party)
    {
        // clear defensive stance
        for (auto i = 0; i < party.Members.size(); i++)
        {
            party.Members[i].IsDefending = false;
        }
    }

    int Find(Character::Base &character, Spell::Type spell)
    {
        auto result = -1;

        for (auto i = 0; i < character.Spells.size(); i++)
        {
            if (character.Spells[i].Type == spell)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool WasCalledToMind(Character::Base &character, Spell::Type spell)
    {
        auto result = Engine::Find(character, spell);

        return (result >= 0 && result < character.Spells.size());
    }

    void ResetSpellDifficulty(Character::Base &character)
    {
        if (Engine::HasAbility(character, Abilities::Type::CastSpell) && character.Spells.size() > 0)
        {
            for (auto i = 0; i < character.Spells.size(); i++)
            {
                character.Spells[i].Difficulty = character.Spells[i].Complexity;
            }
        }
    }

    void ResetSpellDifficulty(Party::Base &party)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            Engine::ResetSpellDifficulty(party.Members[i]);
        }
    }

    bool Enthraled(std::vector<Enemy::Base> &enemys)
    {
        auto result = true;

        for (auto i = 0; i < enemys.size(); i++)
        {
            result &= (enemys[i].Enthraled || !Engine::IsAlive(enemys[i]));
        }

        return result;
    }

    void UpdateSpellStatus(Character::Base &character, int CombatRound)
    {
        std::vector<Spell::Status> NewStatus = {};

        for (auto i = 0; i < character.SpellStatus.size(); i++)
        {
            auto StartRound = std::get<1>(character.SpellStatus[i]);

            if (CombatRound > StartRound)
            {
                auto Score = std::get<3>(character.SpellStatus[i]);

                auto Duration = std::get<2>(character.SpellStatus[i]);

                auto Type = std::get<0>(character.SpellStatus[i]);

                Duration--;

                if (Duration > 0)
                {
                    NewStatus.push_back({Type, StartRound, Duration, Score});
                }
            }
            else
            {
                NewStatus.push_back(character.SpellStatus[i]);
            }
        }

        character.SpellStatus = NewStatus;
    }

    void UpdateSpellStatus(Enemy::Base &enemy, int CombatRound)
    {
        std::vector<Spell::Status> NewStatus = {};

        for (auto i = 0; i < enemy.SpellStatus.size(); i++)
        {
            auto StartRound = std::get<1>(enemy.SpellStatus[i]);

            if (CombatRound > StartRound)
            {
                auto Score = std::get<3>(enemy.SpellStatus[i]);

                auto Duration = std::get<2>(enemy.SpellStatus[i]);

                auto Type = std::get<0>(enemy.SpellStatus[i]);

                Duration--;

                if (Duration > 0)
                {
                    NewStatus.push_back({Type, StartRound, Duration, Score});
                }
            }
            else
            {
                NewStatus.push_back(enemy.SpellStatus[i]);
            }
        }

        enemy.SpellStatus = NewStatus;
    }

    int GetStatus(Character::Base &character, Spell::Type status)
    {
        auto result = -1;

        for (auto i = 0; i < character.SpellStatus.size(); i++)
        {
            if (std::get<0>(character.SpellStatus[i]) == status)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool HasStatus(Character::Base &character, Spell::Type status)
    {
        auto result = Engine::GetStatus(character, status);

        return (result >= 0 && result < character.SpellStatus.size());
    }

    int GetStatus(Enemy::Base &enemy, Spell::Type status)
    {
        auto result = -1;

        for (auto i = 0; i < enemy.SpellStatus.size(); i++)
        {
            if (std::get<0>(enemy.SpellStatus[i]) == status)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool HasStatus(Enemy::Base &enemy, Spell::Type status)
    {
        auto result = Engine::GetStatus(enemy, status);

        return (result >= 0 && result < enemy.SpellStatus.size());
    }
}
#endif