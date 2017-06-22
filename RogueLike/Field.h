#pragma once

#include <SFML\Graphics.hpp>

class Field
{
	private:
		sf::Color color_;
	public:
		Field(sf::Color color) : color_(color) {}
		sf::Color getColor() const { return color_; }
};