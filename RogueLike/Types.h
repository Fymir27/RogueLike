#pragma once

#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::endl;

const unsigned int TILE_SIZE = 64;
const unsigned int WINDOW_WIDTH  = 860;
const unsigned int WINDOW_HEIGHT = 640;


class Field;

typedef std::vector<Field*> Row;
typedef std::vector<Row> Map;

typedef struct
{
	int x_;
	int y_;
} Position;

std::ostream& operator<<(std::ostream& out, const Position& pos);

typedef struct
{
	int hp_;  //Hitpoints
	int mana_;//Mana
	int str_; //Strength
	int int_; //Intelligence
	int dex_; //Dexterity
} Stats;

std::ostream& operator<<(std::ostream& out, Stats stats);