#ifndef __BATTLE_HPP__
#define __BATTLE_HPP__

namespace Battle
{
    class Base
    {
    public:
        bool SurprisedEnemy = false;

        bool SurprisedByEnemy = false;

        int SoloCombat = -1;

        int ShootingRounds = 0;

        Base()
        {
        }
    };
}
#endif