#pragma once

#include "Field.h"

class Wall : public Field
{
	private:
	static bool texture_loaded_;
	static sf::Texture texture_;

	public:
		Wall():Field(sf::Color(0x80, 0x80, 0x80), 0) {};
		virtual bool stepOn() { return false; };
};