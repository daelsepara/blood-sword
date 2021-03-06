#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "combat.hpp"
#include "glyphs.hpp"
#include "random.hpp"
#include "astar.hpp"

namespace Engine
{
    auto Random = Random::Base();

    void Randomize()
    {
        Engine::Random.UniformIntDistribution(1, 6);
        Engine::Random.UniformDistribution(0, 1.0);
    }

    enum class RollStage
    {
        START = 0,
        ROLL,
        END
    };

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

                break;
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
            if (character.Equipment[i].Attribute == attribute && (character.Equipment[i].Class == Equipment::Class::Normal || (IsWeapon && character.Equipment[i].Class == Equipment::Class::Weapon && character.Equipment[i].Weapon != Equipment::Weapon::Bow)))
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

    void Endurance(Character::Base &character, int endurance)
    {
        Engine::Score(character, Attributes::Type::Endurance, endurance);
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

            if (character.Attributes[index].Maximum > 0)
            {
                character.Attributes[index].Value = std::min(character.Attributes[index].Value, character.Attributes[index].Maximum);
            }

            character.Attributes[index].Value = std::max(0, character.Attributes[index].Value);
        }
    }

    void Gain(Character::Base &character, int value)
    {
        Engine::Gain(character, Attributes::Type::Endurance, value);
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
            if (character.Equipment[i].Class == Equipment::Class::Armour && character.Equipment[i].Rating > armour)
            {
                armour = character.Equipment[i].Rating;
            }
        }

        return armour;
    }

    int Damage(Character::Base &character, int damage)
    {
        auto total_damage = std::max(0, damage - Engine::Armour(character));

        Engine::Gain(character, -total_damage);

        return total_damage;
    }

    bool IsAlive(Character::Base &character)
    {
        return (Engine::Score(character, Attributes::Type::Endurance) > 0);
    }

    bool Paralyzed(Character::Base &character)
    {
        return Engine::IsAlive(character) && character.Paralyzed;
    }

    int Count(Party::Base &party)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]))
            {
                result++;
            }
        }

        return result;
    }

    int Remaining(Party::Base &party)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]) && !party.Members[i].Escaped && !party.Members[i].Away && !Engine::Paralyzed(party.Members[i]))
            {
                result++;
            }
        }

        return result;
    }

    bool HasArmour(Party::Base &Party)
    {
        auto result = false;

        for (auto i = 0; i < Party.Members.size(); i++)
        {
            if (Engine::IsAlive(Party.Members[i]) && Engine::Armour(Party.Members[i]) > 0)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    int Find(Character::Base &character, Code::Status code)
    {
        auto result = -1;

        for (auto i = 0; i < character.Codes.size(); i++)
        {
            if (character.Codes[i] == code)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool HasCode(Character::Base &character, Code::Status code)
    {
        auto result = Engine::Find(character, code);

        return result >= 0 && result < character.Codes.size();
    }

    void GetCode(Character::Base &character, Code::Status code)
    {
        if (!Engine::HasCode(character, code))
        {
            character.Codes.push_back(code);
        }
    }

    void RemoveCode(Character::Base &character, Code::Status code)
    {
        if (Engine::HasCode(character, code))
        {
            auto found = Engine::Find(character, code);

            character.Codes.erase(character.Codes.begin() + found);
        }
    }

    int Count(Party::Base &party, Code::Status code)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            result += Engine::IsAlive(party.Members[i]) && Engine::HasCode(party.Members[i], code) ? 1 : 0;
        }

        return result;
    }

    int First(Party::Base &party)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]))
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool HasSpace(Character::Base &character)
    {
        return character.Equipment.size() < character.Encumbrance;
    }

    int FirstSpace(Party::Base &party)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]) && Engine::HasSpace(party.Members[i]))
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool HasSpace(Party::Base &party)
    {
        auto result = Engine::FirstSpace(party);

        return result >= 0 && result < party.Members.size();
    }

    int Find(Party::Base &party, int index)
    {
        auto result = -1;

        auto found = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]))
            {
                found++;

                if (found == index)
                {
                    result = i;

                    break;
                }
            }
        }

        return result;
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

    bool Paralyzed(Party::Base &party)
    {
        auto paralyzed = 0;

        auto dead = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]) && Engine::Paralyzed(party.Members[i]))
            {
                paralyzed++;
            }
            else if (!Engine::IsAlive(party.Members[i]))
            {
                dead++;
            }
        }

        return (paralyzed > 0 && (paralyzed + dead) >= party.Members.size());
    }

    bool Escaped(Party::Base &party)
    {
        auto escaped = 0;

        auto dead = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]) && party.Members[i].Escaped)
            {
                escaped++;
            }
            else if (!Engine::IsAlive(party.Members[i]))
            {
                dead++;
            }
        }

        return (escaped > 0 && (escaped + dead) >= party.Members.size());
    }

    bool Wounded(Party::Base &party)
    {
        auto result = false;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            Character::Base &character = party.Members[i];

            if (Engine::IsAlive(character) && Engine::Endurance(character) < Engine::Maximum(character, Attributes::Type::Endurance))
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool IsAlive(Enemy::Base &enemy)
    {
        return (enemy.Endurance > 0);
    }

    int Count(std::vector<Enemy::Base> &enemies)
    {
        auto result = 0;

        for (auto i = 0; i < enemies.size(); i++)
        {
            if (Engine::IsAlive(enemies[i]))
            {
                result++;
            }
        }

        return result;
    }

    int First(std::vector<Enemy::Base> &enemies)
    {
        auto result = -1;

        for (auto i = 0; i < enemies.size(); i++)
        {
            if (Engine::IsAlive(enemies[i]))
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool IsAlive(std::vector<Enemy::Base> &enemies)
    {
        return Engine::Count(enemies) > 0;
    }

    bool HasAbility(Character::Base &character, Abilities::Type ability)
    {
        auto result = false;

        for (auto i = 0; i < character.Abilities.size(); i++)
        {
            if (character.Abilities[i] == ability && Engine::IsAlive(character))
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool HasAbility(Party::Base &party, Abilities::Type ability)
    {
        auto result = false;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            result |= Engine::IsAlive(party.Members[i]) && Engine::HasAbility(party.Members[i], ability);
        }

        return result;
    }

    bool HasWeapon(Character::Base &character)
    {
        auto result = false;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Class == Equipment::Class::Weapon && character.Equipment[i].Weapon != Equipment::Weapon::Bow && Engine::IsAlive(character))
            {
                result = true;

                break;
            }
        }

        return result;
    }

    int Find(Character::Base &character, Equipment::Weapon weapon)
    {
        auto result = -1;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Class == Equipment::Class::Weapon && character.Equipment[i].Weapon == weapon)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool HasWeapon(Character::Base &character, Equipment::Weapon weapon)
    {
        auto result = Engine::Find(character, weapon);

        return (result >= 0 && result < character.Equipment.size() && Engine::IsAlive(character));
    }

    int Count(Party::Base &party, Equipment::Weapon weapon)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::HasWeapon(party.Members[i], weapon))
            {
                result++;
            }
        }

        return result;
    }

    bool HasWeapon(Party::Base &party, Equipment::Weapon weapon)
    {
        return Engine::Count(party, weapon) > 0;
    }

    int First(Party::Base &party, Equipment::Weapon weapon)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::HasWeapon(party.Members[i], weapon))
            {
                result = i;

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
            if (character.Equipment[i].Class == Equipment::Class::Weapon && character.Equipment[i].Weapon == Equipment::Weapon::Bow && Engine::IsAlive(character))
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
            if (character.Equipment[i].Class == Equipment::Class::Quiver && character.Equipment[i].Arrows > 0 && Engine::IsAlive(character))
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
                if (character.Equipment[i].Class == Equipment::Class::Quiver && character.Equipment[i].Arrows > 0)
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

    bool ActingFirst(Party::Base &party)
    {
        auto result = false;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]) && party.Members[i].ActFirst)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool ActingFirst(std::vector<Enemy::Base> &enemies)
    {
        auto result = false;

        for (auto i = 0; i < enemies.size(); i++)
        {
            if (Engine::IsAlive(enemies[i]) && enemies[i].ActFirst)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    void ActFirst(Party::Base &party)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]))
            {
                party.Members[i].ActFirst = true;
            }
        }
    }

    void ShootFirst(Party::Base &party)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            Character::Base &character = party.Members[i];

            character.ShootFirst = Engine::IsAlive(character) && Engine::HasBow(character) && Engine::CanShoot(character) && Engine::HasArrows(character);
        }
    }

    void ActFirst(std::vector<Enemy::Base> &enemies)
    {
        for (auto i = 0; i < enemies.size(); i++)
        {
            if (Engine::IsAlive(enemies[i]))
            {
                enemies[i].ActFirst = true;
            }
        }
    }

    bool KnockedOff(std::vector<Enemy::Base> &enemies)
    {
        auto result = false;

        for (auto i = 0; i < enemies.size(); i++)
        {
            if (enemies[i].Endurance > 0 && enemies[i].KnockedOff)
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
            if (character.Equipment[i].Class == Equipment::Class::MoneyPouch && character.Equipment[i].Gold > 0)
            {
                result += character.Equipment[i].Gold;
            }
        }

        return result;
    }

    void LoseMoney(Character::Base &character, int Gold)
    {
        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Class == Equipment::Class::MoneyPouch && character.Equipment[i].Gold > 0 && Gold > 0)
            {
                character.Equipment[i].Gold -= Gold;

                if (character.Equipment[i].Gold < 0)
                {
                    Gold = std::abs(character.Equipment[i].Gold);
                }
                else
                {
                    break;
                }
            }
        }
    }

    bool HaveMoney(Character::Base &character)
    {
        return Engine::CountMoney(character) > 0;
    }

    void ClearAwayStatus(Party::Base &party)
    {
        // clear defensive stance
        for (auto i = 0; i < party.Members.size(); i++)
        {
            party.Members[i].Away = false;
        }
    }

    void ClearDefendingStatus(Party::Base &party)
    {
        // clear defensive stance
        for (auto i = 0; i < party.Members.size(); i++)
        {
            party.Members[i].Defending = false;

            party.Members[i].Defended = false;
        }
    }

    void ClearParalyzed(Party::Base &party)
    {
        // clear paralyzed status
        for (auto i = 0; i < party.Members.size(); i++)
        {
            party.Members[i].Paralyzed = false;
        }
    }

    void ClearEscaped(Party::Base &party)
    {
        // clear escaped status
        for (auto i = 0; i < party.Members.size(); i++)
        {
            party.Members[i].Escaped = false;
        }
    }

    void ClearEngaged(Party::Base &party)
    {
        // clear escaped status
        for (auto i = 0; i < party.Members.size(); i++)
        {
            party.Members[i].Engaged = false;
        }
    }

    void NormalThinking(Party::Base &party)
    {
        // clear defensive stance
        for (auto i = 0; i < party.Members.size(); i++)
        {
            party.Members[i].QuickThinking = false;
        }
    }

    void NormalCombatOrder(Party::Base &party)
    {
        // clear act first flag
        for (auto i = 0; i < party.Members.size(); i++)
        {
            party.Members[i].ActFirst = false;
        }
    }

    void NormalCombatOrder(std::vector<Enemy::Base> &enemies)
    {
        // clear act first flag
        for (auto i = 0; i < enemies.size(); i++)
        {
            enemies[i].ActFirst = false;
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

    bool Enthraled(std::vector<Enemy::Base> &enemies)
    {
        auto dead = 0;

        auto enthraled = 0;

        for (auto i = 0; i < enemies.size(); i++)
        {
            if (Engine::IsAlive(enemies[i]))
            {
                if (enemies[i].Enthraled)
                {
                    enthraled++;
                }
            }
            else
            {
                dead++;
            }
        }

        return (enthraled > 0 && (enthraled + dead) >= enemies.size());
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

        return (result >= 0 && result < character.SpellStatus.size()) && Engine::IsAlive(character);
    }

    void ClearSpellStatus(Character::Base &character)
    {
        character.SpellStatus.clear();
    }

    void ClearSpellStatus(Party::Base &party)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            Engine::ClearSpellStatus(party.Members[i]);
        }
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

        return (result >= 0 && result < enemy.SpellStatus.size() && Engine::IsAlive(enemy));
    }

    int Find(Party::Base &party, Character::Class member)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::IsAlive(party.Members[i]) && party.Members[i].Class == member)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool IsPresent(Party::Base &party, Character::Class member)
    {
        auto result = Engine::Find(party, member);

        return result >= 0 && result < party.Members.size();
    }

    bool IsPresent(std::vector<Enemy::Base> enemies, Enemy::Type enemy)
    {
        auto result = false;

        for (auto i = 0; i < enemies.size(); i++)
        {
            if (Engine::IsAlive(enemies[i]) && enemies[i].Type == enemy)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    template <typename T>
    int Find(std::vector<T> &selection, T value)
    {
        int result = -1;

        for (auto i = 0; i < selection.size(); i++)
        {
            if (selection[i] == value)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    template <typename T>
    bool InList(std::vector<T> &selection, T value)
    {
        auto result = Engine::Find(selection, value);

        return result >= 0 && result < selection.size();
    }

    template <typename T>
    void Erase(std::vector<T> &selection, T value)
    {
        if (Engine::InList(selection, value))
        {
            selection.erase(selection.begin() + Engine::Find(selection, value));
        }
    }

    int Find(std::vector<Equipment::Base> &Equipment, Item::Type item)
    {
        auto result = -1;

        for (auto i = 0; i < Equipment.size(); i++)
        {
            if (Equipment[i].Item == item)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int Find(std::vector<Equipment::Base> &Equipment, Item::Type item, int charge)
    {
        auto result = -1;

        for (auto i = 0; i < Equipment.size(); i++)
        {
            if (Equipment[i].Item == item && Equipment[i].ChargeLimit > 0 && Equipment[i].Charge >= charge)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int Find(Character::Base &character, Item::Type item)
    {
        return Engine::Find(character.Equipment, item);
    }

    bool HasItem(Character::Base &character, Item::Type item)
    {
        auto found = Engine::Find(character.Equipment, item);

        return (found >= 0 && found < character.Equipment.size() && Engine::IsAlive(character));
    }

    bool HasItem(Character::Base &character, Item::Type item, int charge)
    {
        auto found = Engine::Find(character.Equipment, item, charge);

        return (found >= 0 && found < character.Equipment.size() && Engine::IsAlive(character));
    }

    int Count(Party::Base &party, Item::Type item)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::HasItem(party.Members[i], item))
            {
                result++;
            }
        }

        return result;
    }

    bool HasItem(Party::Base &party, Item::Type item)
    {
        return Engine::Count(party, item) > 0;
    }

    int First(Party::Base &party, Item::Type item)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::HasItem(party.Members[i], item))
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool HasCharge(Character::Base &character, Item::Type item, int charge)
    {
        return Engine::HasItem(character, item, charge);
    }

    bool Discharge(Equipment::Base &equipment, int charge)
    {
        auto Discharged = false;

        if (equipment.Charge >= charge)
        {
            equipment.Charge -= charge;

            Discharged = true;
        }

        return Discharged;
    }

    bool Discharge(Character::Base &character, Item::Type item, int charge)
    {
        auto Discharged = false;

        if (Engine::HasCharge(character, item, charge))
        {
            auto found = Engine::Find(character.Equipment, item, charge);

            Discharged = Engine::Discharge(character.Equipment[found], charge);
        }

        return Discharged;
    }

    void Drop(Character::Base &character, Equipment::Weapon weapon)
    {
        if (Engine::HasWeapon(character, weapon))
        {
            auto found = Engine::Find(character, weapon);

            character.Equipment.erase(character.Equipment.begin() + found);
        }
    }

    void Drop(Character::Base &character, Item::Type item)
    {
        if (Engine::HasItem(character, item))
        {
            auto found = Engine::Find(character, item);

            character.Equipment.erase(character.Equipment.begin() + found);
        }
    }

    int FirstPouch(Character::Base &character, int value)
    {
        auto result = -1;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            Equipment::Base &equipment = character.Equipment[i];

            if (equipment.Class == Equipment::Class::MoneyPouch && (equipment.Gold + value) < equipment.GoldLimit)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int FirstPouch(Character::Base &character)
    {
        auto result = -1;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            Equipment::Base &equipment = character.Equipment[i];

            if (equipment.Class == Equipment::Class::MoneyPouch && equipment.Gold < equipment.GoldLimit)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int GainGold(Character::Base &character, int gold)
    {
        while (gold > 0)
        {
            auto pouch = Engine::FirstPouch(character, gold);

            if (pouch >= 0 && pouch < character.Equipment.size())
            {
                character.Equipment[pouch].Gold += gold;

                gold = 0;

                break;
            }
            else
            {
                pouch = Engine::FirstPouch(character);

                if (pouch >= 0 && pouch < character.Equipment.size())
                {
                    gold -= (character.Equipment[pouch].GoldLimit - character.Equipment[pouch].Gold);

                    character.Equipment[pouch].Gold = character.Equipment[pouch].GoldLimit;
                }
                else
                {
                    break;
                }
            }
        }

        gold = std::max(0, gold);

        return gold;
    }

    int FirstQuiver(Character::Base &character, int value)
    {
        auto result = -1;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            Equipment::Base &equipment = character.Equipment[i];

            if (equipment.Class == Equipment::Class::Quiver && (equipment.Arrows + value) < equipment.ArrowLimit)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int FirstQuiver(Character::Base &character)
    {
        auto result = -1;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            Equipment::Base &equipment = character.Equipment[i];

            if (equipment.Class == Equipment::Class::Quiver && equipment.Arrows < equipment.ArrowLimit)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int GainArrows(Character::Base &character, int arrows)
    {
        while (arrows > 0)
        {
            auto quiver = Engine::FirstQuiver(character, arrows);

            if (quiver >= 0 && quiver < character.Equipment.size())
            {
                character.Equipment[quiver].Arrows += arrows;

                arrows = 0;

                break;
            }
            else
            {
                quiver = Engine::FirstQuiver(character);

                if (quiver >= 0 && quiver < character.Equipment.size())
                {
                    arrows -= (character.Equipment[quiver].ArrowLimit - character.Equipment[quiver].Arrows);

                    character.Equipment[quiver].Arrows = character.Equipment[quiver].ArrowLimit;
                }
                else
                {
                    break;
                }
            }
        }

        arrows = std::max(0, arrows);

        return arrows;
    }

    bool Visited(Party::Base &party, Character::Base &character, Book::Destination destination)
    {
        auto result = false;

        if (party.Visited.count(character.Class) > 0)
        {
            std::vector<Book::Destination> &Visited = party.Visited[character.Class];

            for (auto i = 0; i < Visited.size(); i++)
            {
                if (destination.first == Visited[i].first && destination.second == Visited[i].second)
                {
                    result = true;

                    break;
                }
            }
        }
        else
        {
            party.Visited[character.Class] = std::vector<Book::Destination>();
        }

        return result;
    }

    void Visit(Party::Base &party, Character::Base &character, Book::Destination destination)
    {
        if (Engine::IsAlive(character) && !Engine::Visited(party, character, destination))
        {
            party.Visited[character.Class].push_back(destination);
        }
    }

    void Visit(Party::Base &party, int character, Book::Destination destination)
    {
        if (character >= 0 && character < party.Members.size())
        {
            Engine::Visit(party, party.Members[character], destination);
        }
    }

    bool Visited(Party::Base &party, int character, Book::Destination destination)
    {
        auto result = false;

        if (character >= 0 && character < party.Members.size())
        {
            result = Engine::Visited(party, party.Members[character], destination);
        }

        return result;
    }

    void Gain(Party::Base &party, Character::Class character, std::vector<Equipment::Base> equipment)
    {
        if (Engine::IsPresent(party, character))
        {
            auto member = Engine::Find(party, character);

            party.Members[member].Equipment.insert(party.Members[member].Equipment.end(), equipment.begin(), equipment.end());
        }
    }
}
#endif
