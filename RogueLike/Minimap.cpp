#include "Minimap.h"

vector<vector<RoomStatus>> Minimap::map_;
Position Minimap::active_room_;

void Minimap::init(size_t width, size_t height, Position pos_active)
{
	map_.resize(height);
	for (auto& row : map_)
	{
		row.resize(width);
		for (auto& status : row)
		{
			status = UNEXPLORED;
		}
	}
	active_room_ = pos_active;
	map_.at(active_room_.y_).at(active_room_.x_) = ACTIVE;
}

void Minimap::setActiveRoom(size_t x, size_t y)
{
	map_.at(active_room_.y_).at(active_room_.x_) = EXPLORED;
	active_room_ = Position(x, y);
	map_.at(active_room_.y_).at(active_room_.x_) = ACTIVE;
	//print();
}

void Minimap::setActiveRoom(Position pos)
{
	setActiveRoom(pos.x_, pos.y_);
}

void Minimap::print()
{
	for (auto& row : map_)
	{
		for (auto& room : row)
			cout << room;
		cout << endl;
	}
}

void Minimap::draw(sf::RenderWindow& window)
{
	static Position pos(750, 400);
	const int SIZE = 10;
	sf::Color colors[4] = { sf::Color::Blue, sf::Color::Green, sf::Color::Red, sf::Color::Black };

	for (size_t y = 0; y < map_.size(); y++)
	{
		for (size_t x = 0; x < map_.at(y).size(); x++)
		{
			RoomStatus s = map_.at(y).at(x);

			sf::RectangleShape room(sf::Vector2f(SIZE, SIZE));
			room.setFillColor(colors[s]);
			room.setPosition(sf::Vector2f(pos.x_ + x * SIZE, pos.y_ + y * SIZE));

			window.draw(room);
		}
	}
}