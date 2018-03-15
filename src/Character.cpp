#include "Character.h"
#include "Field.h"
#include "Room.h"
#include "Item.h"
#include "UI.h"
#include "AbilityEffectTypes.h"
#include "Abilities.h"
#include "Player.h"

std::ostream& operator<<(std::ostream& out, Stats stats)
{
    out << "-------" << " Stats " << "------" << endl;
    out << "Strength:     " << stats.str_ << endl;
    out << "Endurance:    " << stats.end_ << endl;
    out << "Dexterity:    " << stats.dex_ << endl;
    out << "Intelligence: " << stats.int_ << endl;
    out << "Willpower:    " << stats.wil_ << endl;
    out << "-------------------" << endl;
    return out;
}

Stats& Stats::operator+=(const Stats& other)
{
    this->str_ += other.str_;
    this->end_ += other.end_;
    this->dex_ += other.dex_;
    this->int_ += other.int_;
    this->wil_ += other.wil_;
    return *this;
}

Stats& Stats::operator-=(const Stats& other)
{
    this->str_ -= other.str_;
    this->end_ -= other.end_;
    this->dex_ -= other.dex_;
    this->int_ -= other.int_;
    this->wil_ -= other.wil_;
    return *this;
}

Stats Stats::operator+(const Stats& right)
{
    Stats res;
    res.str_ = this->str_ + right.str_;
    res.end_ = this->end_ + right.end_;
    res.dex_ = this->dex_ + right.dex_;
    res.int_ = this->int_ + right.int_;
    res.wil_ = this->wil_ + right.wil_;
    return res;
}

Stats Stats::operator-(const Stats& right)
{
    Stats res;
    res.str_ = this->str_ - right.str_;
    res.end_ = this->end_ - right.end_;
    res.dex_ = this->dex_ - right.dex_;
    res.int_ = this->int_ - right.int_;
    res.wil_ = this->wil_ - right.wil_;
    return res;
}


Stats::Stats(int str, int end, int dex, int intel, int will) :
        str_(str), end_(end), dex_(dex), int_(intel), wil_(will)
{

}

Stats::Stats(xml_node const& stats_node)
{
    str_ = std::stoi(stats_node.child("str").child_value());
    end_ = std::stoi(stats_node.child("end").child_value());
    dex_ = std::stoi(stats_node.child("dex").child_value());
    int_ = std::stoi(stats_node.child("int").child_value());
    wil_ = std::stoi(stats_node.child("wil").child_value());
}

bool Character::addItem(shared_ptr<Items::Item> item, size_t count)
{
    return inventory_->addItem(shared_ptr<Items::Item>(item), count);
}

map<size_t, size_t> Character::exp_needed_;

void Character::init_exp_needed()
{
    static size_t max_level = 25;
    for (size_t i = 1; i < max_level; ++i)
    {
        exp_needed_[i] = 10 + (i - 1) * 5;
    }
}

void Character::grantExp(size_t amount)
{
    cout << amount << " Exp granted" << endl;
    long overflow = exp_.add(amount);
    while (exp_.full())
    {
        levelUp();
        overflow = exp_.add(overflow);
    }
}

void Character::levelUp()
{
    level_++;
    UI::displayText("Reached level " + std::to_string(level_) + "!");
    stats_ += Stats(1, 1, 1, 1, 1);
    hp_ = Ressource(stats_.end_ * 10);
    mana_ = Ressource(stats_.int_ * 10);
    exp_ = Ressource(exp_needed_[level_], 0); //reinstantiate exp with new max
}

Character::Character(string const& name, Position pos, Stats stats, string const& filename) : name_(name), pos_(pos),
                                                                                stats_(stats),
                                                                                inventory_(new Inventory())
{
    //cout << "-~=# " << name_ << " #=~-" << endl;
    if (!texture_.loadFromFile(filename))
        cout << "Failed to load character texture!" << endl;

    hp_ = Ressource(stats_.end_ * 10);
    mana_ = Ressource(stats_.int_ * 10);
    exp_ = Ressource(exp_needed_[1], 0);

    sprite_.setTexture(texture_);

    if(current_room != nullptr)
        current_room->occupyField(pos_, this);
}

Character::Character(Character* orig) : name_(orig->name_), pos_(orig->pos_), stats_(orig->stats_), inventory_(new Inventory())
{
    texture_ = orig->texture_;

    hp_ = Ressource(stats_.end_ * 10);
    mana_ = Ressource(stats_.int_ * 10);
    exp_ = Ressource(exp_needed_[1], 0);

    sprite_.setTexture(texture_);
}

Character::~Character()
{
    for (auto const& effect : ability_effects_)
    {
           delete effect;
    }
    effects_.clear();
    current_room->freeField(pos_);
    delete inventory_;
}

void Character::draw(sf::RenderWindow& window)
{
    sprite_.setPosition(worldToScreen(pos_));
    window.draw(sprite_);
    for(auto& effect : effects_)
    {
        effect->setPosition(worldToScreen(pos_));
        effect->update();
        window.draw(*effect);
    }
}

void Character::heal(unsigned amount)
{
    hp_ += amount;
    cout << name_ << " healed for " << amount << endl;
}

void Character::damage(unsigned amount)
{
    hp_ -= amount;
    cout << name_ << " damaged for " << amount << endl;
}

void Character::restoreMana(unsigned amount)
{
    mana_ += amount;
    cout << name_ << " restored " << amount << " mana" << endl;
}

/*
void Character::attack(Character* target)
{
	size_t damage = stats_.str_ + stats_.dex_/2 + stats_.int_/3;
	UI::displayText(name_ + " attacks " + target->getName() + " for " + std::to_string(damage) + " damage.");
	target->damage(damage);
}
*/

bool Character::move(Position new_pos)
{
    //cout << name_ << " moving from " << pos_ << " to " << new_pos << endl;
    bool valid = current_room->stepOn(new_pos, this, pos_);
    //cout << "Result: " << pos_ << endl;
    return valid;
}

void Character::applyAbilityEffect(AbilityEffect* effect)
{
    ability_effects_.push_back(effect);
}

void Character::advanceEffects()
{
    auto effects_tmp = ability_effects_; //copy so deleting is possible
    for (auto effect : effects_tmp)
    {
        if (effect->tick() == 0) //check if effect has run out
        {
            ability_effects_.remove(effect);
            delete effect;
        }
    }
}

bool Character::castSpell(size_t nr, Direction dir, bool self)
{
    Ability* ab = nullptr;

    try
    {
        ab = ability_bar_.at(nr - 1); //Arrays start at 0 goddamnit! (and so do vectors)
    }
    catch(std::out_of_range const& e)
    {
        UI::displayText("No such Spell!");
        return false;
    }

    if(mana_ < ab->getCost())
    {
        UI::displayText("Not enough Ressource!");
        return false;
    }

    if(ab->getCooldownLeft() > 0)
    {
        UI::displayText("That Ability is still on cooldown!");
        return false;
    }

    UI::displayText(name_ + " casts " + ab->getName() + "!");

    if(ab->cast(this, dir, self))
    {
        mana_ -= ab->getCost();
        ab->putOnCooldown();
        return true;
    }

    return false;
}

void Character::addVisualEffect(shared_ptr<Effect> e)
{
    effects_.push_back(e);
}

void Character::removeVisualEffect(shared_ptr<Effect> e)
{
    effects_.remove(e);
}





