#pragma once

#include <SFML/Graphics.hpp>

class Field
{
	protected:
		int tile_nr_;
	private:
		sf::Color color_;
	public:
		Field(sf::Color color, int tile_nr) : tile_nr_(tile_nr), color_(color) {}
		sf::Color getColor() const { return color_; }
		int getTileNr() const { return tile_nr_; };
};

class Tree : public Field
{
public:
	Tree() : Field(sf::Color::Black, 2) {};
};

class Water : public Field
{
public:
	Water() : Field(sf::Color::Black, 3) {};
};

class Lava : public Field
{
public:
	Lava() : Field(sf::Color::Black, 4) {};
};
