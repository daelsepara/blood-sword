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
}
#endif
