#pragma once

#include "Common.h"
#include "Types.h"
#include "GameObject.h"


class Character;
class Ability : public GameObject 
{
public:
    Ability(string name, string descr, unsigned damage, unsigned healing, size_t cd, size_t cost);
    virtual void cast(Character* target) = 0;
    size_t getCost() { return cost_; }

protected:
    unsigned damage_ = 0;
    unsigned healing_ = 0;
    size_t cooldown_ = 0;
    size_t cost_ = 0;  
};

class Fireball : public Ability
{
	public:
    Fireball();
    void cast(Character* target);
};

class Regeneration : public Ability
{
public:
	Regeneration();
	void cast(Character* target);
};
