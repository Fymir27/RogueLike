#include "Floor.h"

bool Floor::texture_loaded_ = false;
sf::Texture Floor::texture_ = sf::Texture();

sf::Texture & Floor::getTexture()
{
	if (!texture_loaded_)
		if (!texture_.loadFromFile("Floor.png", sf::IntRect(0, 0, 64, 64)))
			cout << "Error loading Floor texture" << endl;
		else
			texture_loaded_ = true;

	return texture_;
}

