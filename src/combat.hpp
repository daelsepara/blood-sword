#ifndef __COMBAT__HPP__
#define __COMBAT__HPP__

namespace Combat
{
    enum class Action
    {
        None = 0,
        Move,
        Fight,
        Defend,
        Shoot,
        Flee,
        Memorize,
        Cast
    };

    enum class Mode
    {
        Normal = 0,
        Move,   // Selecting destination
        Attack, // Selecting attack target
        Magic   // Selecting magic target
    };
}
#endif
