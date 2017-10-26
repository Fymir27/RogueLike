#pragma once
#include "Common.h"
#include "Character.h"

class Player : public Character
{

friend class Room;

private:
	bool invincible = false;
	unsigned int invincibility_frames = 0;

public:

	Player(const string name, const Position pos, const Stats stats);
	
	Inventory * getInventory();

	void setPosition(Position pos) { pos_ = pos; };
	void step();
	virtual bool move(Position new_pos);
	void rest();
	//void damage(const int amount);

};

extern Player* current_player;