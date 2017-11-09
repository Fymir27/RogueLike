#pragma once
#include "Common.h"
#include "Inventory.h"
#include "Types.h"
#include "Ressource.h"
//#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>

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
class AbilityEffect;
class Character
{
protected:
	string     name_;
	string     class_ = "None";
	Position   pos_;
	Inventory* inventory_;

	size_t level_ = 1;
	static map<size_t, size_t> exp_needed_;
	Ressource hp_;
	Ressource mana_;
	Ressource exp_;
	Stats     stats_;
	list<AbilityEffect*> effects_;

	sf::Texture texture_;
	sf::Sprite  sprite_;

	Character(const string name, const Position pos, const Stats stats, string filename);
	

public:
	virtual ~Character();
	static void init_exp_needed();
	void grantExp(size_t amount);
	void levelUp();
	size_t getLevel() { return level_; }
	size_t getExpNeeded() { return exp_needed_[level_]; }
	Ressource const& getHp() { return hp_; }
	Ressource const& getMana() { return mana_; }
	Ressource const& getExp() { return exp_; }

	//--- Getters ---//
	string   getName() const     { return name_; }
	string   getClassName() const { return class_; }
	Position getPosition() const { return pos_; }
	Stats    getStats() const    { return stats_; }

	virtual void heal(const int amount);
	virtual void damage(const int amount);
	void applyEffect(AbilityEffect* effect);
	void removeEffect(AbilityEffect *effect);
	bool dead() { return hp_.depleted(); }
	
	bool addItem(Item* item);

	virtual bool move(Position new_pos); //returns if the move was valid
	virtual void attack(Character* target) = 0;
  virtual void advanceEffects();

	bool castSpell(int nr, Character * target);

	void draw(sf::RenderWindow& window);


};
