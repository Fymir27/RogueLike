#include "AbilityEffectTypes.h"
#include "UI.h"

void AbilityEffect::remove()
{
    if(effect != nullptr)
        Effect::removeEffect(effect);
}

//-----------------------------------------------------------------------------//

OverTimeEffect::OverTimeEffect(string name, string descr, bool harmful, int amount, size_t dur) :
AbilityEffect(name, descr, dur), harmful_(harmful), amount_(amount)
{

}

void OverTimeEffect::apply(Character* target)
{
	target_ = target;
    target_->applyEffect(this);
}

size_t OverTimeEffect::tick()
{
    dur_--;
    /*
    UI::displayText(target_->getName() + " gets " + (harmful_?"hurt":"healed") +
                    " by " + name_ + " (" + std::to_string(amount_) + ")");
    */

    if(harmful_)
        target_->damage(amount_);
    else
        target_->heal(amount_);
    return dur_;
}

//-----------------------------------------------------------------------------//

StatEffect::StatEffect(string name, string descr, Stats delta, size_t dur) :
AbilityEffect(name, descr, dur), delta_(delta)
{

}

void StatEffect::apply(Character* target)
{
    target_ = target;
    target_->setStats(target_->getStats() + delta_);
    target_->applyEffect(this);
}

void StatEffect::remove()
{
    AbilityEffect::remove();
    target_->setStats(target_->getStats() - delta_);
    //UI::displayText(name_ + " fades from " + target_->getName());
}

//-----------------------------------------------------------------------------//

ConditionEffect::ConditionEffect(string name, string descr, ConditionType type, size_t dur) :
AbilityEffect(name, descr, dur), type_(type)
{

}

void ConditionEffect::apply(Character* target)
{
    //target->applyEffect()
    //oder
    //target->setStatus()
}

size_t ConditionEffect::tick()
{

}

