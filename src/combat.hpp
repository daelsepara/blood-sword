#ifndef __COMBAT_HPP__
#define __COMBAT_HPP__

namespace Combat
{
    enum class Mode
    {
        NORMAL = 0,
        MOVE,   // Selecting destination
        ATTACK, // Selecting attack target
        SHOOT,  // Selecting a target for a ranged attack
        CAST,   // Selecting magic target
    };

    enum class Result
    {
        NONE = 0,
        FIGHT,
        VICTORY,
        DEFEAT,
        ESCAPED,
        UNSUCCESSFUL,
        KNOCKED_OFF,
        ENTHRALED
    };

    enum class Stage
    {
        START,
        FIGHT,
        DAMAGE,
        END // end of attack
    };

    enum class FightMode
    {
        NONE = 0,
        FIGHT,
        SHOOT
    };
}
#endif
