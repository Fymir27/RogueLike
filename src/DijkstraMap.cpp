#include "DijkstraMap.h"
#include "Room.h"
#include "Field.h"


void DijkstraMap2D::updateNeighbours(Position pos, size_t cur)
{
	for (size_t i = 0; i < 4; i++)
	{
		int new_x = pos.x_ + DELTA_X[i];
		int new_y = pos.y_ + DELTA_Y[i];

		try
		{
			size_t& tmp = map_.at(new_y).at(new_x);

			if (current_room->getField({new_x, new_y})->getFieldStatus() == SOLID)
			{
				continue;
			}

			if (tmp > (cur + 1))
			{
				tmp = cur + 1;
				updateNeighbours(Position(new_x, new_y), tmp);
			}
		}
		catch (...)
		{
			continue;
		}
	}
}

DijkstraMap2D::DijkstraMap2D(size_t width, size_t height, Position source) : width_(width), height_(height), source_(source)
{
	map_.resize(height_);
	for (auto& row : map_)
	{
		row.resize(width_);
		for (size_t& value : row)
		{
			value = std::numeric_limits<size_t>::max();
		}
	}

	map_.at(source_.y_).at(source_.x_) = 0;
	updateNeighbours(source_, 0);

	/*
	cout << endl;
	for (auto& row : map_)
	{
	for (auto val : row)
	{
	if (val > 99)
	cout << " # ";
	else
	printf_s("%3zd", val);
	}
	cout << endl;
	}
	*/
}

Position DijkstraMap2D::getNextPosition(Position cur_pos)
{
    //cout << "DMap: Getting next step from " << cur_pos << " to " << source_ << endl;
	int dx = absolute(source_.x_ - cur_pos.x_);
	int dy = absolute(source_.y_ - cur_pos.y_);

    //current_room->printToConsole();

	if(dx == 0 && dy == 0)
		return  cur_pos;

	Direction vertical_dir = (source_.y_ > cur_pos.y_) ? DOWN : UP;
	Direction horizontal_dir = (source_.x_ > cur_pos.x_) ? RIGHT : LEFT;

    Position new_pos;

	if(dx > dy)
	{
        new_pos = cur_pos + DELTA_POS[horizontal_dir];
        FIELD_STATUS status = current_room->getField(new_pos)->getFieldStatus();
        if((status == SOLID))
        {

            new_pos = cur_pos + DELTA_POS[vertical_dir];
        }
	}
	else if(dy > dx)
	{
        new_pos = cur_pos + DELTA_POS[vertical_dir];
        FIELD_STATUS status = current_room->getField(new_pos)->getFieldStatus();
        if((status == SOLID))
        {

            new_pos = cur_pos + DELTA_POS[horizontal_dir];
        }
	}
	else
	{
        if(roll(1,2))
        {
            new_pos = cur_pos + DELTA_POS[horizontal_dir];
            FIELD_STATUS status = current_room->getField(new_pos)->getFieldStatus();
            if(status == SOLID)
            {
                new_pos = cur_pos + DELTA_POS[vertical_dir];
            }
        }
        else
        {
            new_pos = cur_pos + DELTA_POS[vertical_dir];
            FIELD_STATUS status = current_room->getField(new_pos)->getFieldStatus();
            if(status == SOLID)
            {
                new_pos = cur_pos + DELTA_POS[horizontal_dir];
            }
        }
	}
    cout << "Final new_pos " << new_pos << endl;
    return new_pos;
}

void DijkstraMap2D::updateSource(Position pos)
{
	source_ = pos;

	for (auto& row : map_)
		for (size_t& value : row)
			value = std::numeric_limits<size_t>::max();
	map_.at(source_.y_).at(source_.x_) = 0;
	updateNeighbours(source_, 0);
}
