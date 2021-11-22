#ifndef __EQUIPMENT_HPP__
#define __EQUIPMENT_HPP__

#include <string>

namespace Equipment
{
    enum class Type
    {
        None = 0,
        Normal,
        Weapon,
        Armour,
        MoneyPouch,
        Quiver
    };

    enum class Weapon
    {
        None = 0,
        Sword,
        Bow,
        Quarterstaff
    };

    class Base
    {
    public:
        Equipment::Type Type = Equipment::Type::None;

        Equipment::Weapon WeaponType = Equipment::Weapon::None;

        // For Money Pouch
        int Gold = 0;

        int GoldLimit = 100;

        // For Armours
        int Rating = 0;

        // For Quivers
        int Arrows = 0;

        int ArrowLimit = 6;

        std::string Name = "";

        std::string Description = "";

        Attributes::Type Attribute = Attributes::Type::None;

        int AttributeScore = 0;

        Base(Equipment::Type type, const char *name, const char *description)
        {
            Type = type;

            Name = name;

            Description = description;
        }

        Base(Equipment::Type type, Equipment::Weapon weapon, const char *name, const char *description)
        {
            Type = type;

            Name = name;

            Description = description;

            WeaponType = weapon;
        }

        Base(Equipment::Type type, const char *name, const char *description, int value)
        {
            Type = type;

            Name = name;

            Description = description;

            if (Type == Equipment::Type::Armour)
            {
                Rating = value;
            }
            else if (Type == Equipment::Type::MoneyPouch)
            {
                Gold = value;
            }
            else if (Type == Equipment::Type::Quiver)
            {
                Arrows = value;
            }
        }

        Base(Equipment::Type type, const char *name, const char *description, int value, int limit)
        {
            Type = type;

            Name = name;

            Description = description;

            if (Type == Equipment::Type::MoneyPouch)
            {
                Gold = value;

                GoldLimit = limit;
            }
            else if (Type == Equipment::Type::Quiver)
            {
                Arrows = value;

                ArrowLimit = limit;
            }
        }
    };

    // Weapons
    auto Bow = Equipment::Base(Equipment::Type::Weapon, Equipment::Weapon::Bow, "bow", "bow");
    auto Sword = Equipment::Base(Equipment::Type::Weapon, Equipment::Weapon::Sword, "sword", "sword");
    auto Quarterstaff = Equipment::Base(Equipment::Type::Weapon, Equipment::Weapon::Quarterstaff, "quarterstaff", "quarterstaff");

    // Containers
    auto MoneyPouch = Equipment::Base(Equipment::Type::MoneyPouch, "money-pouch", "money-pouch", 0, 100);
    auto Quiver = Equipment::Base(Equipment::Type::Quiver, "quiver", "quiver", 0, 6);

    // Armours
    auto ChainMail = Equipment::Base(Equipment::Type::Armour, "chainmail", "chainmail", 3);
    auto RingMail = Equipment::Base(Equipment::Type::Armour, "ringmail", "ringmail", 2);
    auto Silver = Equipment::Base(Equipment::Type::Armour, "silver", "silver", 2);
    auto StuddedLeather = Equipment::Base(Equipment::Type::Armour, "studded leather", "studded leather", 2);
}

#endif