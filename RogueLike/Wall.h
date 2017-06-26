#pragma once

#include "Field.h"

class Wall : public Field
{
	private:
	static bool texture_loaded_;
	static sf::Texture texture_;

	public:
	sf::Texture& getTexture();
};