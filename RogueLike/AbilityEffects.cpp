#include "AbilityEffects.h"

//-------- Over time effects ----------//


//#- Dots -#//

BurnEffect::BurnEffect(int damage, size_t dur) :
        OverTimeEffect("On fire", "Take damage every turn", true, damage, dur)
{
    effect_ = shared_ptr<Effect>(new ParticleEffect(sf::Color::Red, 80));
}



//#- Hots -#//

RegenerationEffect::RegenerationEffect(int healing, size_t dur) :
        OverTimeEffect("Refreshed", "Heal every turn", false, healing, dur)
{
    effect_ = shared_ptr<Effect>(new ParticleEffect(sf::Color::Green, 60));
}


//-------- Stat effects ----------//


//#- Buffs #-//

AllStatsDown::AllStatsDown(unsigned amount, size_t dur) :
        StatEffect("Weakened", "Stats are reduced",
                   Stats(-amount, -amount, -amount, -amount, -amount), dur)
{
    effect_ = shared_ptr<Effect>(new ParticleEffect(sf::Color::Black, 40));
}



//#- Debuffs #-//

AllStatsUp::AllStatsUp(unsigned amount, size_t dur) :
        StatEffect("Powered up", "Stats are increased",
                   Stats(amount, amount, amount, amount, amount), dur)
{
    effect_ = shared_ptr<Effect>(new ParticleEffect(sf::Color::Blue, 50));
}

