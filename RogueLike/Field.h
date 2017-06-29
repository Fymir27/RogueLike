#pragma once

#include "Common.h"

class Player;
class Field
{
	protected:
		int tile_nr_;
		Field(int tile_nr) : tile_nr_(tile_nr) {};
	public:
		int getTileNr() const { return tile_nr_; };
		virtual bool stepOn(Player* player) { return true; };
};

class Wall : public Field
{
	public:
	Wall() :Field(0) {};
	virtual bool stepOn(Player* player) { return false; };
};

class Floor : public Field
{
	public:
	Floor() :Field(1) {};
};

class Tree : public Field
{
public:
	Tree() : Field(2) {};
	virtual bool stepOn(Player* player) { return false; };
};

class Water : public Field
{
public:
	Water() : Field(3) {};
};

class Lava : public Field
{
public:
	Lava() : Field( 4) {};
	virtual bool stepOn(Player* player) { return false; };
};