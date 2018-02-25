#include "AbilityEffectTypes.h"
#include "UI.h"

AbilityEffect::AbilityEffect(string const& name, string const& descr, size_t dur) : GameObject(name, descr), dur_(dur)
{

}

AbilityEffect::~AbilityEffect()
{
    target_->removeVisualEffect(effect_);
}

AbilityEffect::AbilityEffect(AbilityEffect* orig, Character* target) :
        GameObject(orig), dur_(orig->dur_), target_(target), effect_(orig->effect_->createInstance())
{
    cout << "AbilityEffect::cctor" << endl;
    cout << "Applying effect" << endl;
    target_->applyAbilityEffect(this);
    cout << "Adding visual effect" << endl;
    target_->addVisualEffect(effect_);
}


//-----------------------------------------------------------------------------//

OverTimeEffect::OverTimeEffect(string const& name, string const& descr, bool harmful, int amount, size_t dur) :
        AbilityEffect(name, descr, dur), harmful_(harmful), amount_(amount)
{

}

size_t OverTimeEffect::tick()
{
    dur_--;
    /*
    UI::displayText(target_->getName() + " gets " + (harmful_?"hurt":"healed") +
                    " by " + name_ + " (" + std::to_string(amount_) + ")");
    */

    if (harmful_)
        target_->damage(amount_);
    else
        target_->heal(amount_);
    return dur_;
}

OverTimeEffect::OverTimeEffect(OverTimeEffect* orig, Character* target) :
        AbilityEffect(orig, target), harmful_(orig->harmful_), amount_(orig->amount_)
{
    cout << "OTEffect::cctor" << endl;
}

AbilityEffect* OverTimeEffect::createInstance(Character* target)
{
    cout << "OTEffect::createInstance, " << name_ << endl;
    AbilityEffect* e = new OverTimeEffect(this, target);
    cout << "OTEffect::createInstance success!" << endl;
    return e; //new OverTimeEffect(this, target);
}


//-----------------------------------------------------------------------------//

StatEffect::StatEffect(string const& name, string const& descr, Stats delta, size_t dur) :
        AbilityEffect(name, descr, dur), delta_(delta)
{

}

StatEffect::StatEffect(StatEffect* orig, Character* target) :
        AbilityEffect(orig, target), delta_(orig->delta_)
{
    target_->setStats(target_->getStats() + delta_);
}

StatEffect::~StatEffect()
{
    target_->setStats(target_->getStats() - delta_);
}

AbilityEffect* StatEffect::createInstance(Character* target)
{
    return new StatEffect(this, target);
}
