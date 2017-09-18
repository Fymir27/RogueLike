#include "TileMap.h"
#include "Common.h"

TileMap::TileMap()
{
	cout << "Tilemap ctor" << endl;
}

bool TileMap::load(const char* filename, const Map& map, 
				   int tile_size, int width, int height)
{
	cout << "Loading tileset from file:" << filename << endl;
	if(!tileset_.loadFromFile(filename))
	{
		cout << "FAILED" << endl;
	}

	//cout << width << 'x' << height << ',' << tile_size << endl;

	vertices_.setPrimitiveType(sf::Quads);
	vertices_.resize(width * height * 4);

	for(size_t y = 0; y < height; y++)
	{
		for(size_t x = 0; x < width; x++)
		{
			int tile_nr = map.at(y).at(x)->getTileNr();
			int index = width * y + x; //linear index

			int tex_x = (tile_nr * 64) % tileset_.getSize().x;
			int tex_y = (tile_nr * 64) / tileset_.getSize().x;


			sf::Vertex* quad = &vertices_[index * 4];

			//clockwise!
			quad[0].position = sf::Vector2f(x * tile_size, y * tile_size);
			quad[1].position = sf::Vector2f((x+1) * tile_size, y * tile_size);
			quad[2].position = sf::Vector2f((x+1) * tile_size, (y+1) * tile_size);
			quad[3].position = sf::Vector2f(x * tile_size, (y+1) * tile_size);

			/*
			for(int i = 0; i < 4; i++)
			{
				cout << quad[i].position.x << '|' << quad[i].position.y << ", ";
			}
			cout << endl;
			*/

			quad[0].texCoords = sf::Vector2f(tex_x, tex_y);
			quad[1].texCoords = sf::Vector2f(tex_x + 64, tex_y);
			quad[2].texCoords = sf::Vector2f(tex_x + 64, tex_y + 64);
			quad[3].texCoords = sf::Vector2f(tex_x, tex_y + 64);
		}
	}
	return true;
}

void TileMap::draw(sf::RenderTarget& target, 
					  sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &tileset_;
	target.draw(vertices_, states);
}