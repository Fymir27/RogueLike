#pragma once

#include "Common.h"
#include "Room.h"
#include "Types.h"

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
			  int tile_size, int width, int height);
};