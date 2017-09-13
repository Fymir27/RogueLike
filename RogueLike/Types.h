#pragma once

#include "Common.h"

class Field;

typedef std::vector<Field*> Row;
typedef std::vector<Row> Map;

struct Position
{
	unsigned int x_;
	unsigned int y_;
};

extern Position operator+(Position first, const Position& second);
extern Position operator-(Position first, const Position& second);
extern bool operator==(const Position& first, const Position& second);

std::ostream& operator<<(std::ostream& out, const Position& pos);

#define CUR 0
#define MAX 1

typedef struct
{
	int hp_[2];  //Hitpoints
	int mana_[2];//Mana
	int str_; //Strength
	int int_; //Intelligence
	int dex_; //Dexterity
} Stats;

std::ostream& operator<<(std::ostream& out, Stats stats);

//extern char* getStatString(Stats stats);

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};