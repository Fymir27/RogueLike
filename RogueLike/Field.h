#pragma once

#include "Common.h"
#include "Types.h"

enum FIELD_STATUS
{
	SOLID,
	FREE,
	TRIGGER,
	OCCUPIED,
	PICKUP
};

class Character;
class Room;
class Item;
class Field
{
	friend class Room;

	protected:
		int tile_nr_;
		Field(Position pos, int tile_nr, FIELD_STATUS);
		Position pos_;
		FIELD_STATUS status_;
		Character* character_ = NULL;   //Character standing on that field
		Item*      item_      = NULL;   //Item lying on that field
		size_t     count_     = 0;      //number of Items
		size_t distance_to_player_ = std::numeric_limits<size_t>::max();

	public:
		virtual ~Field();
		int 				    getTileNr() const { return tile_nr_; };
		FIELD_STATUS 		getFieldStatus() { return status_; };
		Character* 			getCharacter() { return character_; };
		size_t          getDistanceToPlayer() { return distance_to_player_; };

		virtual void free();
		void occupy(Character* character);
		void placeItem(Item* item);
		void pickUpItem(Character* character);

		virtual Position trigger(Character* who) { cout << "[Error] No trigger available!" << endl; return pos_; };

		virtual void draw(sf::RenderWindow& window);

};

class Wall : public Field
{
	public:
	Wall(Position pos) : Field(pos, 0, SOLID) {};
};

class Floor : public Field
{
public:
	Floor(Position pos) :Field(pos, 1, FREE) {};
};

class Tree : public Field
{
public:
	Tree(Position pos) : Field(pos, 2, SOLID) {};
};

class Water : public Field
{
public:
	Water(Position pos) : Field(pos, 3, FREE) {};
};

class Lava : public Field
{
public:
	Lava(Position pos) : Field(pos, 4, TRIGGER) {};
	Position trigger(Character * who);
	void free();
};

class Door : public Field
{
private:
	Direction dir_;
public:
	Door(Position pos, Direction dir);
	virtual Position trigger(Character* who);
	void free();
};