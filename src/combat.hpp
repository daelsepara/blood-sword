#ifndef __COMBAT__HPP__
#define __COMBAT__HPP__

namespace Combat
{
    enum class Action
    {
        NONE = 0,
        ACTION,
        FIGHT,
        DEFEND,
        SHOOT,
        FLEE,
        MEMORIZE,
        CAST
    };

    enum class Mode
    {
        NORMAL = 0,
        MOVE,      // Selecting destination
        ATTACK,    // Selecting attack target
        SHOOT,     // Selecting a target for a ranged attack
        MAGIC,     // Selecting magic target
        HEAL       // Selecting a to heal
    };

    enum class Result
    {
        NONE = 0,
        FIGHT,
        VICTORY,
        DEFEAT,
        ESCAPED,
        UNSUCCESSFUL
    };

    enum class Stage
    {
        START,
        FIGHT,
        DAMAGE,
        END // end of attack
    };
}
#endif
