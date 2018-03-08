#pragma once

#include "Common.h"

class Field;
typedef std::vector<Field*> Row;
typedef std::vector<Row> Map;

extern std::default_random_engine random_engine;

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

const char newl = '\n';
const string separator(50, '-');

struct Position
{
	int x_;
	int y_;
	Position(int x = 0, int y = 0);
};

const Position DELTA_POS[4] = { Position(0,-1), Position(1,0), Position(0,1), Position(-1,0) };

extern Position operator+(Position first, const Position& second);
extern Position operator-(Position first, const Position& second);
extern bool operator==(const Position& first, const Position& second);

std::ostream& operator<<(std::ostream& out, const Position& pos);

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

enum EComparison
{
	SMALLER, EQUAL, BIGGER
};

inline Direction opposite(Direction dir)
{
	return (Direction)((dir + 2) % 4);
}

inline float getVectorLength(sf::Vector2f v)
{
    //return std::sqrtf(v.x*v.x + v.y*v.y);
	return std::sqrt(v.x*v.x + v.y*v.y);
}

inline sf::Vector2f worldToScreen(Position pos)
{
	return sf::Vector2f(pos.x_ * TILE_SIZE, pos.y_ * TILE_SIZE);
}

template <typename T>
inline void clamp(T& val, T min, T max)
{
    if(val <  min)
        val = min;
    if(val > max)
        val = max;
}

template <typename T>
inline size_t absolute(T i)
{
    return i > 0 ? i : i*(-1);
};

inline bool roll(size_t count, size_t out_of)
{
    return ((random_engine() % out_of) < count);
}

template <typename T>
inline T getEnumFromNode(xml_node node)
{
	return static_cast<T>(std::stoi(node.child_value()));
}

template <typename T>
inline T getEnumFromAttribute(xml_attribute attr)
{
	return static_cast<T>(std::stoi(attr.value()));
}