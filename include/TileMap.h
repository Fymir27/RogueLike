#pragma once

#include "Common.h"
#include "Room.h"
#include "Utils.h"

class TileMap : public sf::Drawable, public sf::Transformable
{
private:
	sf::Texture tileset_;
	sf::VertexArray vertices_;


	virtual void draw(sf::RenderTarget& target, 
					  sf::RenderStates states) const;

public:
	TileMap();
	bool load(const char* filename, const Map& map, 
			  size_t tile_size, size_t width, size_t height);
};