#ifndef __EQUIPMENT_HPP__
#define __EQUIPMENT_HPP__

#include <string>

namespace Item
{
    // for non-weapon, non-quiver, non-money-pouches items, e.g. unique items
    enum class Type
    {
        Any = 0,
        RubyRing,
        OctagonalPrism,
        BlueIceJewel,
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
        ChimeraSpittle,
        VellumScroll,
        RingOfWarding,
        EmeraldScarab,
        HeartOfSkrymir,
        OpalMedallion,
        BronzeKey,
        DaggerOfVislet,
        GoldenSnuffBox,
        DragonlordGem,
        GiantsSkull,
        FossilizedHeart,
        RibCage
    };
}

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

    std::map<Item::Type, const char *> ItemDescription = {
        {Item::Type::Any, "any item"},
        {Item::Type::RubyRing, "ruby ring"},
        {Item::Type::OctagonalPrism, "octagonal prism"},
        {Item::Type::BlueIceJewel, "blue ice jewel"},
        {Item::Type::SteelSceptre, "steel sceptre"},
        {Item::Type::VialOfBlackLiquid, "vial of black liquid"},
        {Item::Type::ScrollOfInvisibility, "vial of black liquid"},
        {Item::Type::ScorchedSkull, "scorched skull"},
        {Item::Type::ScrollOfHealing, "healing (scroll)"},
        {Item::Type::ScrollOfAdjustment, "adjust (scroll)"},
        {Item::Type::ScrollOfTimeBlink, "time blink (scroll)"},
        {Item::Type::ScrollOfPrecognition, "precognition (scroll)"},
        {Item::Type::BronzeMallet, "bronze mallet"},
        {Item::Type::MagnificentSword, "magnificent sword"},
        {Item::Type::GildedBridle, "gilded bridle"},
        {Item::Type::ChimeraSpittle, "chimera spittle"},
        {Item::Type::VellumScroll, "vellum scroll"},
        {Item::Type::RingOfWarding, "ring of warding"},
        {Item::Type::EmeraldScarab, "emerald scarab"},
        {Item::Type::HeartOfSkrymir, "heart of Skrymir"},
        {Item::Type::OpalMedallion, "opal medallion"},
        {Item::Type::BronzeKey, "bronze key"},
        {Item::Type::DaggerOfVislet, "dagger of Vislet"},
        {Item::Type::GoldenSnuffBox, "golden snuff-box"},
        {Item::Type::DragonlordGem, "dragonlord gem"},
        {Item::Type::GiantsSkull, "giant's skull"},
        {Item::Type::FossilizedHeart, "fossilized heart"},
        {Item::Type::RibCage, "rib-cage"}};

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

        Item::Type Item = Item::Type::Any;

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

        bool CanDrop = true;

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

        Base(Item::Type item, Attributes::Type attribute, int score)
        {
            Item = item;

            Name = Equipment::ItemDescription[item];

            Description = Equipment::ItemDescription[item];

            Attribute = attribute;

            Score = score;
        }

        Base(Item::Type item)
        {
            Item = item;

            Name = Equipment::ItemDescription[item];

            Description = Equipment::ItemDescription[item];
        }

        Base(Item::Type item, bool canDrop)
        {
            Item = item;

            Name = Equipment::ItemDescription[item];

            Description = Equipment::ItemDescription[item];

            CanDrop = canDrop;
        }

        Base(Item::Type item, int charge, int limit)
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
            else if (ChargeLimit > 0)
            {
                if (ModCount > 0)
                {
                    Modifiers += ", ";
                }

                Modifiers += std::to_string(Charge) + " charge" + (Charge != 1 ? "s" : "");

                ModCount++;
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
    auto LeatherArmour = Equipment::Base(Equipment::Class::Armour, "leather armour", "leather armour", 1);
}

namespace Item
{
    // book 1 items
    auto RubyRing = Equipment::Base(Item::Type::RubyRing);
    auto OctagonalPrism = Equipment::Base(Item::Type::OctagonalPrism);
    auto BlueIceJewel = Equipment::Base(Item::Type::BlueIceJewel);
    auto SteelSceptre = Equipment::Base(Item::Type::SteelSceptre, 4, 4);
    auto VialOfBlackLiquid = Equipment::Base(Item::Type::VialOfBlackLiquid);
    auto ScorchedSkull = Equipment::Base(Item::Type::ScorchedSkull, false);
    auto BronzeMallet = Equipment::Base(Item::Type::BronzeMallet);
    auto MagnificentSword = Equipment::Base(Item::Type::MagnificentSword);
    auto GildedBridle = Equipment::Base(Item::Type::GildedBridle);
    auto ChimeraSpittle = Equipment::Base(Item::Type::ChimeraSpittle);
    auto RingOfWarding = Equipment::Base(Item::Type::RingOfWarding, 4, 4);
    auto EmeraldScarab = Equipment::Base(Item::Type::EmeraldScarab);
    auto HeartOfSkrymir = Equipment::Base(Item::Type::HeartOfSkrymir, false);
    auto OpalMedallion = Equipment::Base(Item::Type::OpalMedallion);
    auto BronzeKey = Equipment::Base(Item::Type::BronzeKey);
    auto DaggerOfVislet = Equipment::Base(Item::Type::DaggerOfVislet);
    auto GoldenSnuffBox = Equipment::Base(Item::Type::GoldenSnuffBox);
    auto DragonlordGem = Equipment::Base(Item::Type::DragonlordGem);
    auto GiantsSkull = Equipment::Base(Item::Type::GiantsSkull, false);
    auto FossilizedHeart = Equipment::Base(Item::Type::FossilizedHeart, false);
    auto RibCage = Equipment::Base(Item::Type::RibCage, false);
}

namespace Scroll
{
    auto Invisibility = Equipment::Base(Item::Type::ScrollOfInvisibility);
    auto Healing = Equipment::Base(Item::Type::ScrollOfHealing);
    auto Adjustment = Equipment::Base(Item::Type::ScrollOfAdjustment);
    auto TimeBlink = Equipment::Base(Item::Type::ScrollOfTimeBlink);
    auto Precognition = Equipment::Base(Item::Type::ScrollOfPrecognition);
}
#endif
