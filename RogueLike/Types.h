#pragma once

#include "Common.h"

class Field;

typedef std::vector<Field*> Row;
typedef std::vector<Row> Map;

template<class T>
void print(const std::vector<std::vector<T>>& v)
{
	for (const std::vector<T>& row : v)
	{
		for (const T& element : row)
		{
			std::cout << element << " ";
		}
		std::cout << std::endl;
	}
}

struct Position
{
	int x_;
	int y_;
	Position(int x = 0, int y = 0);
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