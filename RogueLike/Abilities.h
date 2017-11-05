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

protected:
    unsigned damage_ = 0;
    unsigned healing_ = 0;
    size_t cooldown_ = 0;
    size_t cost_ = 0;  
};

class Fireball : public Ability
{
    Fireball();
    void cast(Character* target);

};
