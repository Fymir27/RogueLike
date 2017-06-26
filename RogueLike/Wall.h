#pragma once

#include "Field.h"

class Wall : public Field
{
	public:
		Wall():Field(sf::Color(0x80, 0x80, 0x80), 0) {};
};