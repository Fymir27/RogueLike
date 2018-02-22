#pragma once

#include "Common.h"
#include "Utils.h"
#include "Player.h"
#include "Room.h"

//TODO: namespace?

enum PlayerClass
{
    WARR,
    MAGE,
    THIEF
};


map<string, PlayerClass> getPlayerClasses();
Player*                  getPlayer(PlayerClass player_class, const string& player_name);


class Warrior : public Player
{
private:
    static Stats base_stats_;
public:
    Warrior(string name, Position pos);
    void attack(Character* target);
};

class Mage : public Player
{
private:
    static Stats base_stats_;
public:
    Mage(string name, Position pos);
    void attack(Character* target);
};

class Thief : public Player
{
private:
    static Stats base_stats_;
public:
    Thief(string name, Position pos);
    void attack(Character* target);
};






