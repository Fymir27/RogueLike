#pragma once
#include "Common.h"
#include "Utils.h"
#include "Singleton.h"

using std::string;

typedef enum ITEM_TYPE
{
	USEABLE,
	EQUIP,
	QUEST
} ItemType;

class Inventory;
class Room;
class Character;

class ItemOLD
{
	friend class Inventory;

protected:

	string name_;
	string description_;
	size_t count_;
	size_t max_count_;
	ItemType type_;

	string texture_file_;
	sf::Texture texture_;
	sf::Sprite sprite_;

public:
	ItemOLD(string name, string descr, size_t count, size_t max_count, string teture_file);
	virtual ItemOLD* clone() = 0;
	virtual ~ItemOLD() {};

	size_t getMaxCount()     const { return max_count_; };
	size_t getCount()        const { return count_; };
	string getName()         const { return name_; };
	string getDescription()  const { return description_; };
	sf::Sprite & getSprite() { return sprite_; };

	void increaseCount(const size_t amount) { count_ += amount; };
	void decreaseCount(const size_t amount) { count_ -= amount; };

	virtual void use(Character* who) {};
};