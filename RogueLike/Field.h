#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

using std::cout;
using std::endl;

class Field
{
	public:
	virtual sf::Texture& getTexture() = 0;
};
