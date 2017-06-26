#pragma once

#include "Field.h"

class Floor : public Field
{
	public:
		Floor():Field(sf::Color::White, 1) {};
};