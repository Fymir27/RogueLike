#include "AbilityEffects.h"

//-------- Over time effects ----------//


//#- Dots -#//

BurnEffect::BurnEffect(int damage, size_t dur) :
        OverTimeEffect("On fire", "Take damage every turn", true, damage, dur)
{
    effect_ = std::make_shared<BigParticleEffect>(sf::Color::Red, 15);
}

PoisonEffect::PoisonEffect(int damage, size_t dur) :
        OverTimeEffect("Poisoned", "Take damage every turn", true, damage, dur)
{
    effect_ = std::make_shared<BigParticleEffect>(sf::Color(100, 255, 0), 15);
}


//#- Hots -#//

RegenerationEffect::RegenerationEffect(int healing, size_t dur) :
        OverTimeEffect("Refreshed", "Heal every turn", false, healing, dur)
{
    effect_ = std::make_shared<BigParticleEffect>(sf::Color::Green, 15);
}


//-------- Stat effects ----------//


//#- Buffs #-//

AllStatsDown::AllStatsDown(unsigned amount, size_t dur) :
        StatEffect("Weakened", "Stats are reduced",
                   Stats(-amount, -amount, -amount, -amount, -amount), dur)
{
    effect_ = std::make_shared<BigParticleEffect>(sf::Color::Black, 10);
}



//#- Debuffs #-//

AllStatsUp::AllStatsUp(unsigned amount, size_t dur) :
        StatEffect("Powered up", "Stats are increased",
                   Stats(amount, amount, amount, amount, amount), dur)
{
    effect_ = std::make_shared<BigParticleEffect>(sf::Color::Blue, 10);
}

