#include "Room.h"
#include "Field.h"
#include "Player.h"
#include "HealingPotion.h"
#include "Item.h"
#include "Types.h"
#include "Common.h"

int main()
{
	cout << "Welcome to RogueLike!" << endl;
	//-- load new room from file --//
	char* filename = "Room1.txt";
	cout << "Instantiating new room..." << endl;
	current_room = new Room(filename);

	//-- create player --//
	Position player_pos{ 1, 3 };
	Stats player_stats = {};
	player_stats.hp_   = 100;
	player_stats.mana_ = 100;
	player_stats.str_  = 10;
	player_stats.int_  = 10;
	player_stats.dex_  = 10;
	Player* player = new Player("Oliver", player_pos, player_stats);
	Item* item = new MediumHealingPotion(23);
	player->addItem(item);

	//-- create window --//
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RogueLike", sf::Style::Default);
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
			else if(event.type == sf::Event::KeyPressed)
				player->processInput(event);
		}

		window.clear(sf::Color::Black);
		
		current_room->draw(window);
		player->draw(window);
		player->getInventory().draw(window);
	
		window.display();
	}
	delete player;
	return 0;
}

