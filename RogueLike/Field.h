#pragma once

#include "Common.h"
#include "Types.h"

class Player;
class Room;
class Field
{
	friend class Room;

	protected:
		int tile_nr_;
		Field(Position pos, int tile_nr);
		Position pos_;
	public:
		int getTileNr() const { return tile_nr_; };
		virtual bool stepOn(Player* player) { return true; };
		virtual bool stepOff() { return true; };
		virtual void draw(sf::RenderWindow& window) {};
};

class Wall : public Field
{
	public:
	Wall(Position pos) :Field(pos, 0) {};
	virtual bool stepOn(Player* player) { return false; };
};

class Floor : public Field
{
	public:
	Floor(Position pos) :Field(pos, 1) {};
};

class Tree : public Field
{
public:
	Tree(Position pos) : Field(pos, 2) {};
	virtual bool stepOn(Player* player) { return false; };
};

class Water : public Field
{
public:
	Water(Position pos) : Field(pos, 3) {};
};

class Lava : public Field
{
public:
	Lava(Position pos) : Field(pos, 4) {};
	virtual bool stepOn(Player* player) { return false; };
};

class Exit : public Field
{
private:
	Direction dir_;
public:
	Exit(Position pos, Room* room);
	virtual bool stepOn(Player* player);
};

class Door : public Field
{
private:
	Direction dir_;
public:
	Door(Position pos, Direction dir);
	virtual bool stepOn(Player* player);
};