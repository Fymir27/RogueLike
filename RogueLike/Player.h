#pragma once
#include "Common.h"
#include "Character.h"

class Ability;
class Player : public Character
{

friend class Room;

private:
	vector<Ability*> ability_bar_;

public:

	Player(const string name, const Position pos, const Stats stats);
	
	Inventory * getInventory();

	void setPosition(Position pos) { pos_ = pos; };
	virtual bool move(Position new_pos);
	void rest();
	//void damage(const int amount);

    bool castSpell(int nr, Character *target);
};

extern Player* current_player;