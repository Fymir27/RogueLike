#include "Player.h"
#include <iostream>

Player::Player(string name, Position pos, Stats stats) : Character(name, pos, stats)
{
	cout << this;
}
