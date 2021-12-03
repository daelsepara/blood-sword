#ifndef __CHOICE_HPP__
#define __CHOICE_HPP__

#include <string>
#include <tuple>
#include <vector>

#include "engine.hpp"

namespace Choice
{
    enum class Type
    {
        Normal = 0,
        Ability,
        Attribute,
        AttributeSelectedCharacter,
        Character,
        Equipment,
        Item,
        Weapon,
        Codeword,
        Shoot,
        Discharge,
        Pay,
        DropWeapon,
        DropItem,
        SelectAdventurer,
        SelectDice,
        CharacterItem,
        TakeScrolls,
        EnemyCastSpell,
        TakeEquipment
    };

    class Base
    {
    public:
        Book::Destination Destination = {Book::Type::None, 0};

        Book::Destination DestinationFail = {Book::Type::None, 0};

        Character::Class Character = Character::Class::None;

        Attributes::Type Attribute = Attributes::Type::None;

        Abilities::Type Ability = Abilities::Type::None;

        Spell::Type Spell = Spell::Type::None;

        Code::Word Codeword = Code::Word::NONE;

        Choice::Type Type = Choice::Type::Normal;

        std::vector<Equipment::Base> Equipment = {};

        Equipment::Item Item = Equipment::Item::Any;

        Equipment::Weapon Weapon = Equipment::Weapon::None;

        std::string SelectMessage = "";

        std::string Text = "";

        std::string Bye = "";

        int Charge = 0;

        int Gold = 0;

        Base(const char *choice, Book::Destination destination)
        {
            Text = choice;

            Destination = destination;

            Type = Choice::Type::Normal;
        }

        Base(const char *choice, Book::Destination destination, Character::Class character)
        {
            Text = choice;

            Destination = destination;

            Character = character;

            Type = Choice::Type::Character;
        }

        Base(const char *choice, Book::Destination destination, Book::Destination destinationFail, Character::Class character, Attributes::Type attribute)
        {
            Text = choice;

            Destination = destination;

            DestinationFail = destinationFail;

            Character = character;

            Attribute = attribute;

            Type = Choice::Type::Attribute;
        }

        Base(const char *choice, Book::Destination destination, Book::Destination destinationFail, Choice::Type type, Attributes::Type attribute)
        {
            Text = choice;

            Destination = destination;

            DestinationFail = destinationFail;

            Type = type;

            Attribute = attribute;
        }

        Base(const char *choice, Book::Destination destination, Character::Class character, Equipment::Item item)
        {
            Text = choice;

            Destination = destination;

            Character = character;

            Item = item;

            Type = Choice::Type::CharacterItem;
        }

        Base(const char *choice, Book::Destination destination, Abilities::Type ability)
        {
            Text = choice;

            Destination = destination;

            Ability = ability;

            Type = Choice::Type::Ability;
        }

        Base(const char *choice, Book::Destination destination, Code::Word codeword)
        {
            Text = choice;

            Destination = destination;

            Codeword = codeword;

            Type = Choice::Type::Codeword;
        }

        Base(const char *choice, Book::Destination destination, std::vector<Equipment::Base> equipment)
        {
            Text = choice;

            Destination = destination;

            Equipment = equipment;

            Type = Choice::Type::Equipment;
        }

        Base(const char *choice, Book::Destination destination, Choice::Type type, std::vector<Equipment::Base> equipment)
        {
            Text = choice;

            Destination = destination;

            Equipment = equipment;

            Type = type;
        }

        Base(const char *choice, Book::Destination destination, Choice::Type type)
        {
            Text = choice;

            Destination = destination;

            Type = type;
        }

        Base(const char *choice, Book::Destination destination, Equipment::Item item)
        {
            Text = choice;

            Destination = destination;

            Item = item;

            Type = Choice::Type::Item;
        }

        Base(const char *choice, Book::Destination destination, Choice::Type type, Equipment::Item item)
        {
            Text = choice;

            Destination = destination;

            Type = type;

            Item = item;
        }

        Base(const char *choice, Book::Destination destination, Choice::Type type, Equipment::Item item, int charge)
        {
            Text = choice;

            Destination = destination;

            Type = type;

            Item = item;

            Charge = charge;
        }

        Base(const char *choice, Book::Destination destination, Equipment::Weapon weapon)
        {
            Text = choice;

            Destination = destination;

            Weapon = weapon;

            Type = Choice::Type::Weapon;
        }

        Base(const char *choice, Book::Destination destination, Choice::Type type, Equipment::Weapon weapon)
        {
            Text = choice;

            Destination = destination;

            Type = type;

            Weapon = weapon;
        }

        Base(const char *choice, Book::Destination destination, Choice::Type type, std::string select)
        {
            Text = choice;

            Destination = destination;

            Type = type;

            SelectMessage = select;
        }

        Base(const char *choice, Book::Destination destination, Choice::Type type, Spell::Type spell)
        {
            Text = choice;

            Destination = destination;

            Type = type;

            Spell = spell;
        }
    };
}
#endif
