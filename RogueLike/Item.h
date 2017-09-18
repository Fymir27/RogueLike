#pragma once
#include "Common.h"
#include "Types.h"
#include "Singleton.h"

using std::string;

class Inventory;
class Room;
class Character;

class Item : public Singleton<Item>
{
	friend class Inventory;

protected:
	Position pos_; //position relative to inventory
	string name_;
	string description_;
	int count_;
	Item* self_;
	Inventory* inventory_;
	bool usable_ = true;

	string texture_file_;
	sf::Texture texture_;
	sf::Sprite sprite_;
	sf::Text text_;

public:
	Item(string name, int count, const char* teture_file);
	Item();
	virtual ~Item() {};

	virtual const int getMaxCount() { return 0; };
	int getCount() const { return count_; };
	void increaseCount(const int amount) { count_ += amount; };
	void decreaseCount(const int amount);

	void associateWithInventory(Inventory* inventory) { inventory_ = inventory; };
	string getName() const { return name_; };
	friend std::ostream& operator<<(std::ostream& out, const Item* item);
	bool tryUse(Room* room, Character* character);
	sf::Sprite & getSprite();
	virtual void use(Room* room, Character* character) { cout << "Trying to use base class Item"; };
	virtual Item* duplicate() { return new Item(name_, count_, texture_file_.c_str()); };

	void setPosition(Position p) { pos_ = p; };
};