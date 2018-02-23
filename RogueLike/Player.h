#pragma once
#include "Common.h"
#include "Character.h"

class Ability;
class Player : public Character
{

friend class Room;

public:

	Player(string name, Position pos, Stats stats, string filename = "../images/player.png");
	
	Inventory * getInventory();

	void setPosition(Position pos) { pos_ = pos; };
	virtual bool move(Position new_pos);
	void rest();
    Character * findTarget(Direction dir);
	void coolDownAbilities();
	//void damage(const int amount);
	vector<Ability*> getAbilities() { return ability_bar_; };

    void draw(sf::RenderWindow& window);
};

extern Player* current_player;