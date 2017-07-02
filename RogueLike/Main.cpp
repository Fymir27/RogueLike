#include "Room.h"
#include "Field.h"
#include "Player.h"
#include "HealingPotion.h"
#include "Pickup.h"
#include "Item.h"
#include "Types.h"
#include "Common.h"
#include "UI.h"

int main()
{
	cout << "Welcome to RogueLike!" << endl;
	//-- load new room from file --//
	char* filename = "Room1.txt";
	cout << "Instantiating new room..." << endl;
	current_room = new Room(filename);

	//-- create player --//
	Stats player_stats = {};
	player_stats.hp_   = 100;
	player_stats.mana_ = 100;
	player_stats.str_  = 10;
	player_stats.int_  = 10;
	player_stats.dex_  = 10;

	Player* player = new Player("Oliver", { 1, 3 }, player_stats);
	current_player = player;

	Item* item = new MediumHealingPotion(57);

	Field* pickup = new Pickup({ 2, 1 }, item);
	current_room->addField(pickup);

	//-- create window --//
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RogueLike", sf::Style::Default);
	window.setFramerateLimit(60);

	UI* ui = new UI();
	
	//-- main loop --//
	sf::Event event;
	while (window.isOpen())
	{
		//-- get input --//
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
				player->processInput(event);
			else if (event.type == sf::Event::MouseButtonPressed)
				player->click(event);
		}

		window.clear(sf::Color::Black);
		
		current_room->draw(window);
		player->draw(window);
		//player->getInventory()->draw(window);
		ui->draw(window);
		window.display();
	}
	delete player;
	return 0;
}

