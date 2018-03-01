#include "Minimap.h"
#include "Dungeon.h"

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
			status = NONE;
		}
	}
	active_room_ = pos_active;
    setActiveRoom(active_room_.x_, active_room_.y_); //weird but needed for correct initialization
	map_.at(active_room_.y_).at(active_room_.x_) = ACTIVE;
}

void Minimap::setActiveRoom(size_t x, size_t y)
{
	map_.at(active_room_.y_).at(active_room_.x_) = EXPLORED;
	active_room_ = Position(x, y);
	map_.at(active_room_.y_).at(active_room_.x_) = ACTIVE;
    for (size_t i = 0; i < 4; i++)
    {
        int cur_x = x + DELTA_X[i];
        int cur_y = y + DELTA_Y[i];
        if (current_dungeon->hasNeighbour(Position(x,y),(Direction)i))
        {
            if (map_.at(cur_y).at(cur_x) == NONE)
            {
                map_.at(cur_y).at(cur_x) = UNEXPLORED;
            }
        }
    }
}

void Minimap::setActiveRoom(Position pos)
{
	setActiveRoom(pos.x_, pos.y_);
}

bool Minimap::isExplored(Position pos)
{
	return map_.at(pos.y_).at(pos.x_) == EXPLORED;
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
	static Position pos(750, 350);
	const int SIZE = 10;
	const int BORDER_WIDTH = 10;

	sf::RectangleShape border(sf::Vector2f(map_.at(0).size() * SIZE + BORDER_WIDTH*2, map_.size() * SIZE + BORDER_WIDTH*2));
	border.setFillColor(sf::Color(50, 50, 50));
	border.setPosition(pos.x_, pos.y_);
	window.draw(border);

	sf::Color colors[4] = { sf::Color::Blue, sf::Color(100, 100, 100), sf::Color(30,30,50), sf::Color::Black };

	for (size_t y = 0; y < map_.size(); y++)
	{
		for (size_t x = 0; x < map_.at(y).size(); x++)
		{
			RoomStatus s = map_.at(y).at(x);

			sf::RectangleShape room(sf::Vector2f(SIZE, SIZE));
			room.setFillColor(colors[s]);
			room.setPosition(sf::Vector2f(pos.x_ + BORDER_WIDTH + x * SIZE, pos.y_ + BORDER_WIDTH + y * SIZE));

			window.draw(room);
		}
	}
}
