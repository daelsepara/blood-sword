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
        Quiver,
        Gold,
        Arrow
    };

    enum class Weapon
    {
        None = 0,
        Sword,
        Bow,
        Quarterstaff,
        Axe,
        Dagger
    };

    std::map<Equipment::Weapon, const char *> WeaponDescription = {
        {Equipment::Weapon::None, "none"},
        {Equipment::Weapon::Sword, "sword"},
        {Equipment::Weapon::Bow, "bow"},
        {Equipment::Weapon::Quarterstaff, "quarterstaff"},
        {Equipment::Weapon::Axe, "axe"},
        {Equipment::Weapon::Dagger, "dagger"}};

    // for non-weapon, non-quiver, non-money-pouches items, e.g. unique items
    enum class Item
    {
        Any = 0,
        RubyRing,
        SteelSceptre,
        VialOfBlackLiquid,
        ScorchedSkull,
        ScrollOfInvisibility,
        ScrollOfHealing,
        ScrollOfAdjustment,
        ScrollOfTimeBlink,
        ScrollOfPrecognition,
        BronzeMallet,
        MagnificentSword,
        GildedBridle,
        ChimeraSpittle
    };

    std::map<Equipment::Item, const char *> ItemDescription = {
        {Equipment::Item::Any, "any item"},
        {Equipment::Item::RubyRing, "ruby ring"},
        {Equipment::Item::SteelSceptre, "steel sceptre"},
        {Equipment::Item::VialOfBlackLiquid, "vial of black liquid"},
        {Equipment::Item::ScrollOfInvisibility, "vial of black liquid"},
        {Equipment::Item::ScorchedSkull, "scorched skull"},
        {Equipment::Item::ScrollOfHealing, "healing (scroll)"},
        {Equipment::Item::ScrollOfAdjustment, "adjust (scroll)"},
        {Equipment::Item::ScrollOfTimeBlink, "time blink (scroll)"},
        {Equipment::Item::ScrollOfPrecognition, "precognition (scroll)"},
        {Equipment::Item::BronzeMallet, "bronze mallet"},
        {Equipment::Item::MagnificentSword, "magnificent sword"},
        {Equipment::Item::GildedBridle, "gilded bridle"},
        {Equipment::Item::ChimeraSpittle, "chimera spittle"}};

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

        // items with charge
        int Charge = -1;

        int ChargeLimit = -1;

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

        Base(Equipment::Class type, Equipment::Weapon weapon)
        {
            Class = type;

            Name = Equipment::WeaponDescription[weapon];

            Description = Equipment::WeaponDescription[weapon];

            Weapon = weapon;

            Attribute = Attributes::Type::FightingProwess;
        }

        Base(Equipment::Item item, Attributes::Type attribute, int score)
        {
            Item = item;

            Name = Equipment::ItemDescription[item];

            Description = Equipment::ItemDescription[item];

            Attribute = attribute;

            Score = score;
        }

        Base(Equipment::Item item)
        {
            Item = item;

            Name = Equipment::ItemDescription[item];

            Description = Equipment::ItemDescription[item];
        }

        Base(Equipment::Item item, int charge, int limit)
        {
            Item = item;

            Name = Equipment::ItemDescription[item];

            Description = Equipment::ItemDescription[item];

            Charge = charge;

            ChargeLimit = limit;
        }

        Base(Equipment::Class type, Equipment::Weapon weapon, Attributes::Type attribute, int score, int damage)
        {
            Class = type;

            Name = Equipment::WeaponDescription[weapon];

            Description = Equipment::WeaponDescription[weapon];

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
            else if (Class == Equipment::Class::Gold)
            {
                Gold = value;

                GoldLimit = value;
            }
            else if (Class == Equipment::Class::Arrow)
            {
                Arrows = value;

                ArrowLimit = value;
            }
        }

        Base(Equipment::Class type, const char *name, const char *description, int value, int limit)
        {
            Class = type;

            Name = name;

            Description = description;

            if (Class == Equipment::Class::MoneyPouch || Class == Equipment::Class::Gold)
            {
                Gold = value;

                GoldLimit = limit;
            }
            else if (Class == Equipment::Class::Quiver || Class == Equipment::Class::Arrow)
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

            if (Class != Equipment::Class::Quiver && Class != Equipment::Class::MoneyPouch && Class != Equipment::Class::Gold && Class != Equipment::Class::Arrow)
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
            else if (Class == Equipment::Class::Gold)
            {
                ItemString = (strong ? "<b>" : "") + std::to_string(Gold) + " " + Name + (strong ? "</b>" : "");
            }
            else if (Class == Equipment::Class::Arrow)
            {
                ItemString = (strong ? "<b>" : "") + std::to_string(Arrows) + " " + Name + (strong ? "</b>" : "");
            }

            if (ModCount > 0)
            {
                ItemString += " [" + Modifiers + "]";
            }

            return ItemString;
        }
    };

    Equipment::Base Gold(int value)
    {
        auto gold = value != 1 ? "gold pieces" : "gold piece";

        return Base(Equipment::Class::Gold, gold, gold, value);
    }

    Equipment::Base Arrows(int value)
    {
        auto arrow = value != 1 ? "arrows" : "arrow";

        return Base(Equipment::Class::Arrow, arrow, arrow, value);
    }

    // weapons
    auto Bow = Equipment::Base(Equipment::Class::Weapon, Equipment::Weapon::Bow);
    auto Sword = Equipment::Base(Equipment::Class::Weapon, Equipment::Weapon::Sword);
    auto Quarterstaff = Equipment::Base(Equipment::Class::Weapon, Equipment::Weapon::Quarterstaff);
    auto Axe = Equipment::Base(Equipment::Class::Weapon, Equipment::Weapon::Axe);
    auto Dagger = Equipment::Base(Equipment::Class::Weapon, Equipment::Weapon::Dagger);

    // containers
    auto MoneyPouch = Equipment::Base(Equipment::Class::MoneyPouch, "money-pouch", "money-pouch", 0, 100);
    auto Quiver = Equipment::Base(Equipment::Class::Quiver, "quiver", "quiver", 0, 6);

    // armours
    auto ChainMail = Equipment::Base(Equipment::Class::Armour, "chainmail", "chainmail", 3);
    auto RingMail = Equipment::Base(Equipment::Class::Armour, "ringmail", "ringmail", 2);
    auto Silver = Equipment::Base(Equipment::Class::Armour, "silver armour", "silvers armour", 2);
    auto StuddedLeather = Equipment::Base(Equipment::Class::Armour, "studded leather", "studded leather", 2);
    auto BreastPlate = Equipment::Base(Equipment::Class::Armour, "breastplate", "breastplate", 1);
    auto LeatherJerkin = Equipment::Base(Equipment::Class::Armour, "leather jerkin", "leather jerkin", 1);

    // book 1 items
    auto RubyRing = Equipment::Base(Equipment::Item::RubyRing);
    auto SteelSceptre = Equipment::Base(Equipment::Item::SteelSceptre, 4, 4);
    auto VialOfBlackLiquid = Equipment::Base(Equipment::Item::VialOfBlackLiquid);
    auto ScorchedSkull = Equipment::Base(Equipment::Item::ScorchedSkull);
    auto BronzeMallet = Equipment::Base(Equipment::Item::BronzeMallet);
    auto MagnificentSword = Equipment::Base(Equipment::Item::MagnificentSword);
    auto GildedBridle = Equipment::Base(Equipment::Item::GildedBridle);
    auto ChimeraSpittle = Equipment::Base(Equipment::Item::ChimeraSpittle);

}

namespace Scroll
{
    auto Invisibility = Equipment::Base(Equipment::Item::ScrollOfInvisibility);
    auto Healing = Equipment::Base(Equipment::Item::ScrollOfHealing);
    auto Adjustment = Equipment::Base(Equipment::Item::ScrollOfAdjustment);
    auto TimeBlink = Equipment::Base(Equipment::Item::ScrollOfTimeBlink);
    auto Precognition = Equipment::Base(Equipment::Item::ScrollOfPrecognition);
}
#endif
