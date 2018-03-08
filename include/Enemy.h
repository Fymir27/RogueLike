#pragma once

#include "Common.h"
#include "Utils.h"
#include "Character.h"
#include "Factory.h"
#include "Biome.h"

enum MoveType
{
    SLEEP, //do nothing
    STAY,  //stay stationary, but attack
    WAIT,  //wait until attacked/someone near then FOLLOW
    FOLLOW,//follow player
    RANDOM //move randomly
};

class Enemy : public Character
{
private:
    MoveType move_type_;

    bool checkSurroundings();

    void moveRandomly();

protected:
    size_t exp_reward_;
    string attack_verb_;
    float scaling_[5];
    list<Biomes::Condition*> spawn_conditions_;

public:

    //-ctors-//
    Enemy(string name, Position pos, Stats stats, MoveType move_type, string texture = "../images/enemy.png"); //deprecated
    Enemy(string name, string texture, Stats stats, size_t exp_reward,
          string attack_verb, float scaling[5], MoveType move_type);
    Enemy(Enemy* orig);

    ~Enemy();

    virtual bool step(); //returns false when the enemy is dead

    virtual void attack(Character* target);
    void damage(unsigned int amount);

    void setSpawnConditions(list<Biomes::Condition*> conditions);
    list<Biomes::Condition*> getSpawnConditions() const;
};

template<>
Factory<Enemy>::Factory();

/*
template<>
class Factory<Enemy> : public Singleton<Factory<Enemy>>
{
public:
    Factory()
    {
        cout << typeid(this).name() << " ctor" << endl;
    }
};
 */


