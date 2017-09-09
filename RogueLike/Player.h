#pragma once
#include "Common.h"
#include "Character.h"

class Player : public Character
{

friend class Room;

private:

public:
	Player(const string name, const Position pos, const Stats stats);
	void processInput(const sf::Event& event);
	void click(const sf::Event& event);
	Inventory * getInventory();
	void setPosition(Position pos) { pos_ = pos; };
};

extern Player* current_player;