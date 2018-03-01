#pragma once

#include "Common.h"
#include "Utils.h"
#include "Character.h"

enum MoveType
{
    SLEEP,
    STAY,
    WAIT,
    FOLLOW,
    RANDOM
};

#define ENEMY_TYPE_COUNT 3
enum EnemyType
{
    RANDOM_ENEMY,
    SPIDER,
    GHOST,
    GOLEM
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

public:

    //-ctors-//
    Enemy(string name, Position pos, Stats stats, MoveType move_type, string texture = "../images/enemy.png");
    Enemy(string name, string texture, Stats stats, size_t exp_reward, string attack_verb = "attacks");
    Enemy(Enemy* orig);

    ~Enemy();

    virtual bool step(); //returns false when the enemy is dead

    virtual void attack(Character* target);
    void damage(unsigned int amount);
};
