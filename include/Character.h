#pragma once
#include "Common.h"
#include "Inventory.h"
#include "Utils.h"
#include "Item.h"
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
	int wil_; //Willpower
	Stats(int str, int end, int dex, int intel, int will);
	Stats() {};
    Stats(xml_node const& stats_node);
	Stats& operator+= (const Stats& other);
	Stats& operator-= (const Stats& other);
    Stats operator+ (const Stats& right);
    Stats operator- (const Stats& right);
};

extern std::ostream& operator<<(std::ostream& out, Stats stats);

class Effect;
class AbilityEffect;
class Ability;
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

	list<AbilityEffect*> ability_effects_;

    vector<Ability*> ability_bar_;


    sf::Texture texture_;
	sf::Sprite  sprite_;
    list<shared_ptr<Effect>> effects_;

	Character(string const& name, Position pos, Stats stats, string const& filename);
    Character(Character* orig);

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

    //--- Setters ---//
    void setStats(const Stats& stats) { stats_ = stats; };

	virtual void heal(unsigned amount);
	virtual void damage(unsigned amount);
	virtual void restoreMana(unsigned amount);

	void applyAbilityEffect(AbilityEffect* effect);
	void addVisualEffect(shared_ptr<Effect> e);
    void removeVisualEffect(shared_ptr<Effect> e);
	bool dead() { return hp_.depleted(); }
	
	bool addItem(shared_ptr<Items::Item> item, size_t count);

	virtual bool move(Position new_pos); //returns if the move was valid
	void setPosition(Position pos) { pos_ = pos; }
	virtual void attack(Character* target) = 0;
	bool castSpell(size_t nr, Direction dir, bool self = false);
    virtual void advanceEffects();


    virtual void draw(sf::RenderWindow& window);


};
