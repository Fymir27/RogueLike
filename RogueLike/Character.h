#pragma once
#include "Common.h"
#include "Inventory.h"
#include "Types.h"

class Item;
class Character
{
protected:
	string     name_;
	Position   pos_;
	Stats      stats_;
	Inventory* inventory_;

	sf::Texture texture_;
	sf::Sprite  sprite_;

	Character(const string name, const Position pos, const Stats stats, string filename);

public:
	~Character();

	//--- Getters ---//
	string   getName() const     { return name_; };
	Position getPosition() const { return pos_; };
	Stats    getStats() const    { return stats_; };

	virtual void heal(const int amount);
	virtual void damage(const int amount);
	
	bool addItem(Item* item, unsigned int count);

	bool move(Position new_pos);
	bool isSolid(Position pos);

	void draw(sf::RenderWindow& window);
	friend std::ostream& operator<<(std::ostream& out, Character* character);
};
