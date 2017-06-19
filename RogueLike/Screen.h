#pragma once

#include <SFML\Graphics.hpp>
#include <string>
using std::string;

class Room;

typedef struct
{
	int width;
	int height;
	char* name;
}ScreenInfo;


class Screen
{
	private:
		sf::RenderWindow window;
		int width_;
		int height_;
		std::string name_;
		Room* room_;

	public:
		
		Screen(int width, int height, string name);
		void init();
		void setRoom(Room* room);
};