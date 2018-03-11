#pragma once

#include "AbilityEffectTypes.h"

//-------- Over time effects ----------//


//#- Dots -#//

class BurnEffect : public OverTimeEffect
{
public:
    BurnEffect(int damage, size_t dur);
};

class PoisonEffect : public OverTimeEffect
{
public:
    PoisonEffect(int damage, size_t dur);
};

//#- Hots -#//

class RegenerationEffect : public OverTimeEffect
{
public:
    RegenerationEffect(int healing, size_t dur);
};

//-------- Stat effects ----------//


//#- Buffs #-//

class AllStatsDown : public StatEffect
{
public:
    AllStatsDown(int amount, size_t dur);
};


//#- Debuffs #-//

class AllStatsUp : public StatEffect
{
public:
    AllStatsUp(unsigned amount, size_t dur);
};