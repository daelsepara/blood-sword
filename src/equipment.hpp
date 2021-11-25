#ifndef __EQUIPMENT_HPP__
#define __EQUIPMENT_HPP__

#include <string>

namespace Equipment
{
    enum class Class
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

    // for non-weapon, non-quiver, non-money-pouches items, e.g. unique items
    enum class Item
    {
        Any = 0,
        RubyRing
    };

    enum class Mode
    {
        USE,
        DROP,
        TAKE
    };

    class Base
    {
    public:
        Equipment::Class Class = Equipment::Class::Normal;

        Equipment::Weapon Weapon = Equipment::Weapon::None;

        Equipment::Item Item = Equipment::Item::Any;

        // for money pouch
        int Gold = 0;

        int GoldLimit = 100;

        // for armours
        int Rating = 0;

        // for quivers
        int Arrows = 0;

        int ArrowLimit = 6;

        std::string Name = "";

        std::string Description = "";

        Attributes::Type Attribute = Attributes::Type::None;

        int Score = 0;

        int Damage = 0;

        Base(Equipment::Class type, const char *name, const char *description)
        {
            Class = type;

            Name = name;

            Description = description;
        }

        Base(Equipment::Class type, Equipment::Weapon weapon, const char *name, const char *description)
        {
            Class = type;

            Name = name;

            Description = description;

            Weapon = weapon;

            Attribute = Attributes::Type::FightingProwess;
        }

        Base(Equipment::Item item, const char *name, const char *description, Attributes::Type attribute, int score)
        {
            Item = item;

            Name = name;

            Description = description;

            Attribute = attribute;

            Score = score;
        }

        Base(Equipment::Item item, const char *name, const char *description)
        {
            Item = item;

            Name = name;

            Description = description;
        }

        Base(Equipment::Class type, Equipment::Weapon weapon, const char *name, const char *description, Attributes::Type attribute, int score, int damage)
        {
            Class = type;

            Name = name;

            Description = description;

            Weapon = weapon;

            Attribute = attribute;

            Score = score;

            Damage = damage;
        }

        Base(Equipment::Class type, const char *name, const char *description, int value)
        {
            Class = type;

            Name = name;

            Description = description;

            if (Class == Equipment::Class::Armour)
            {
                Rating = value;
            }
            else if (Class == Equipment::Class::MoneyPouch)
            {
                Gold = value;
            }
            else if (Class == Equipment::Class::Quiver)
            {
                Arrows = value;
            }
        }

        Base(Equipment::Class type, const char *name, const char *description, int value, int limit)
        {
            Class = type;

            Name = name;

            Description = description;

            if (Class == Equipment::Class::MoneyPouch)
            {
                Gold = value;

                GoldLimit = limit;
            }
            else if (Class == Equipment::Class::Quiver)
            {
                Arrows = value;

                ArrowLimit = limit;
            }
        }

        std::string String(bool strong = false)
        {
            std::string ItemString = (strong ? "<b>" : "") + Name + (strong ? "</b>" : "");

            std::string Modifiers = "";

            auto ModCount = 0;

            if (Class != Equipment::Class::Quiver && Class != Equipment::Class::MoneyPouch)
            {
                if (Attribute != Attributes::Type::None && Score != 0)
                {
                    Modifiers = (Score >= 0 ? "+" : "") + std::to_string(Score) + " " + std::string(Attributes::Abbreviation[Attribute]);
                    
                    ModCount++;
                }

                if (Class == Equipment::Class::Armour)
                {
                    if (ModCount > 0)
                    {
                        Modifiers += ", ";
                    }

                    Modifiers += "armour " + std::to_string(Rating);
                    
                    ModCount++;
                }

                if (Damage != 0)
                {
                    if (ModCount > 0)
                    {
                        Modifiers += ", ";
                    }

                    Modifiers += (Damage >= 0 ? "+" : "") + std::to_string(Damage) + " DMG";
                    
                    ModCount++;
                }
            }
            else if (Class == Equipment::Class::Quiver)
            {
                Modifiers += std::to_string(Arrows) + " arrow" + (Arrows != 1 ? "s" : "");

                ModCount++;
            }
            else if (Class == Equipment::Class::MoneyPouch)
            {
                Modifiers += std::to_string(Gold) + " gold piece" + (Gold != 1 ? "s" : "");

                ModCount++;
            }

            if (ModCount > 0)
            {
                ItemString += " [" + Modifiers + "]";
            }

            return ItemString;
        }
    };

    // weapons
    auto Bow = Equipment::Base(Equipment::Class::Weapon, Equipment::Weapon::Bow, "bow", "bow");
    auto Sword = Equipment::Base(Equipment::Class::Weapon, Equipment::Weapon::Sword, "sword", "sword");
    auto Quarterstaff = Equipment::Base(Equipment::Class::Weapon, Equipment::Weapon::Quarterstaff, "quarterstaff", "quarterstaff");

    // containers
    auto MoneyPouch = Equipment::Base(Equipment::Class::MoneyPouch, "money-pouch", "money-pouch", 0, 100);
    auto Quiver = Equipment::Base(Equipment::Class::Quiver, "quiver", "quiver", 0, 6);

    // armours
    auto ChainMail = Equipment::Base(Equipment::Class::Armour, "chainmail", "chainmail", 3);
    auto RingMail = Equipment::Base(Equipment::Class::Armour, "ringmail", "ringmail", 2);
    auto Silver = Equipment::Base(Equipment::Class::Armour, "silver armour", "silvers armour", 2);
    auto StuddedLeather = Equipment::Base(Equipment::Class::Armour, "studded leather", "studded leather", 2);

    // book 1 items
    auto RubyRing = Equipment::Base(Equipment::Item::RubyRing, "ruby ring", "ruby ring");
}

#endif