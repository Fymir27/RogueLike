#include "Wall.h"

bool Wall::texture_loaded_ = false;
sf::Texture Wall::texture_ = sf::Texture();

sf::Texture & Wall::getTexture()
{
	if (!texture_loaded_)
		if (!texture_.loadFromFile("Wall.png", sf::IntRect(0, 0, 64, 64)))
			cout << "Error loading Wall texture" << endl;
		else
			texture_loaded_ = true;

	return texture_;
}