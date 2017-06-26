#include "Room.h"
#include "Field.h"
//#include "Screen.h"
#include "Player.h"
#include "HealingPotion.h"
#include "Item.h"
#include "Types.h"

#include <iostream>
#include <vector>
#include <thread>

using std::cout;
using std::endl;

int main()
{
	cout << "Welcome to RogueLike!" << endl;
	//-- load new room from file --//
	char* filename = "Room1.txt";
	cout << "Instantiating new room..." << endl;
	Room* room = new Room(filename);

	//-- create player --//
	Position player_pos{ 1, 3 };
	Stats player_stats = {};
	player_stats.hp_   = 100;
	player_stats.mana_ = 100;
	player_stats.str_  = 10;
	player_stats.int_  = 10;
	player_stats.dex_  = 10;
	Player* player = new Player("Oliver", player_pos, player_stats);

	//-- create window --//
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RogueLike");
	window.setFramerateLimit(60);
	sf::Event event;

	//-- main loop --//
	while (window.isOpen())
	{
		//-- get input --//
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);
		
		room->draw(window);
	
		window.display();
	}
	delete player;
	return 0;
}

