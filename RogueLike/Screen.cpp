#include "Screen.h"
#include "Room.h"

#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>



Screen::Screen(int width, int height, string name):  width_(width), height_(height), name_(name), room_(nullptr)
{
}

void Screen::init()
{
	window.create(sf::VideoMode(width_, height_), name_.c_str());
	window.setFramerateLimit(60); // call it once, after creating the window
	sf::Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();			
		}
		if (room_)
		{
			int rows = room_->getRowCount();
			int cols = room_->getColCount();
			float tile_size = window.getSize().x / (float)cols;
			std::cout << window.getSize().x << " | " << cols <<  "="  << tile_size << std::endl;
			sf::Color tile_color;
			for (int y = 0; y < rows; y++)
			{
				for (int x = 0; x < cols; x++)
				{
					if (room_->getField(x, y)->symbol_ == '#')
					{
						tile_color.r = 0x80;
						tile_color.b = 0x80;
						tile_color.g = 0x80;
					}
					else
					{
						tile_color = sf::Color::White;
					}

					sf::RectangleShape tile(sf::Vector2f(tile_size, tile_size));
					tile.setFillColor(tile_color);
					tile.setPosition(sf::Vector2f(x * tile_size, y * tile_size));
					window.draw(tile);
				}
			}
		}
		window.display();
	}
}

void Screen::setRoom(Room* room)
{
	room_ = room;
}
