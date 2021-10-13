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
        Armour
    };

    class Base
    {
    public:
        Equipment::Type Type = Equipment::Type::None;

        bool IsMoneyPouch = false;

        bool IsQuiver = false;

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
    };
}

#endif