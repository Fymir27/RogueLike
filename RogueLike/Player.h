#pragma once
#include "Common.h"
#include "Character.h"

class Player : public Character
{
private:

public:
	Player(const string name, const Position pos, const Stats stats);
	void processInput(const sf::Event& event);
	void click(const sf::Event& event);
	Inventory * getInventory();
};

extern Player* current_player;