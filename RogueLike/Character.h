#pragma once
#include "Common.h"
#include "Inventory.h"
#include "Types.h"
#include "Ressource.h"

//typedef array<int, 2> Ressource;
const int CUR = 0;
const int MAX = 1;

struct Stats
{
	int str_; //Strength
	int end_; //Endurance
	int dex_; //Dexterity
	int int_; //Intelligence
	int will_; //Willpower
	Stats(int str, int end, int dex, int intel, int will);
	Stats() {};
	Stats& operator+= (const Stats& other);
};

class Item;
class Character
{
protected:
	string     name_;
	Position   pos_;
	Inventory* inventory_;

	size_t level_ = 1;
	size_t exp_ = 0;
	static map<size_t, size_t> exp_needed_;
	Ressource<int> hp_;
	Ressource<int> mana_;
	Stats     stats_;

	sf::Texture texture_;
	sf::Sprite  sprite_;

	Character(const string name, const Position pos, const Stats stats, string filename);
	

public:
	virtual ~Character();
	static void init_exp_needed();
	void grantExp(size_t amount);
	void levelUp();
	size_t getLevel() { return level_; }
	size_t getExp() { return exp_; }
	size_t getExpNeeded() { return exp_needed_[level_]; }

	//--- Getters ---//
	string   getName() const     { return name_; };
	Position getPosition() const { return pos_; };
	Stats    getStats() const    { return stats_; };

	virtual void heal(const int amount);
	virtual void damage(const int amount);
	bool dead() { return hp_.depleted(); }
	
	bool addItem(Item* item);

	virtual bool move(Position new_pos); //returns if the move was valid
	virtual void attack(Character* target) = 0;

	void draw(sf::RenderWindow& window);
	friend std::ostream& operator<<(std::ostream& out, Character* character);
};
