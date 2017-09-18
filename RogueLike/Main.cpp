#include "Room.h"
#include "Dungeon.h"
#include "Field.h"
#include "Player.h"
#include "HealingPotion.h"
#include "Pickup.h"
#include "Item.h"
#include "Types.h"
#include "Common.h"
#include "UI.h"
#include "Enemy.h"

void processInput(const sf::Event& event)
{
	//cout << "Input detected!" << endl;
	auto key = event.key.code;
	Position new_pos = current_player->getPosition();
	int item_slot = 0;

	if     (key == sf::Keyboard::Right) new_pos.x_++;
	else if(key == sf::Keyboard::Left)  new_pos.x_--;
	else if(key == sf::Keyboard::Up)    new_pos.y_--;
	else if(key == sf::Keyboard::Down)  new_pos.y_++;
	else if(key == sf::Keyboard::Num1)  item_slot = 1;
	else if(key == sf::Keyboard::Num2)  item_slot = 2;
	else if(key == sf::Keyboard::Num3)  item_slot = 3;

	if(item_slot > 0)
		current_player->getInventory()->useItem(item_slot);

	if(!(new_pos == current_player->getPosition()) && !current_room->isSolid(new_pos))
	{
		current_player->move(new_pos);
		current_room->stepEnemies();
	}	
}

int main()
{
	cout << endl;
	cout << "#################################" << endl;
	cout << "###-- Welcome to RogueLike! --###" << endl;
	cout << "#################################" << endl;
	cout << endl;

	/*
	//-- load new room from file --//
	char* filename = "Room1.txt";
	cout << "Instantiating new room..." << endl;
	current_room = new Room(filename);
	*/

	//-- load new dungeon from file --//
	current_dungeon = new Dungeon();
	current_dungeon->loadFromFile("Dungeon1.txt");

	//-- create player --//
	Stats player_stats = {};
	player_stats.hp_[CUR] = 100;
	player_stats.hp_[MAX] = 100;
	player_stats.mana_[CUR] = 100;
	player_stats.mana_[MAX] = 100;
	player_stats.str_  = 10;
	player_stats.int_  = 10;
	player_stats.dex_  = 10;

	Player* player = new Player("Oliver", { 1, 3 }, player_stats);
	current_player = player;

	//place Pickup item
	Field* pickup = new Pickup({ 5, 2 }, new SmallHealingPotion(), 17);
	current_room->addField(pickup);

	//spawn Enemy
	Enemy* blob = new Enemy("Blobby", {10,4}, player_stats, "../images/enemy.png");

	//-- create window --//
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RogueLike", sf::Style::Default);
	window.setFramerateLimit(60);

	UI* ui = new UI();
	
	cout << endl;
	cout << "////////////////////////////////////////////////////////" << endl;
	cout << "//    The game is fully loaded and will start now.    //" << endl; 
	cout << "//                  Good Luck!                        //" << endl;
	cout << "////////////////////////////////////////////////////////" << endl;
	cout << endl;

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
				processInput(event);
		}

		//-- Game Logic --//
	    //current_player->step();
		//current_room->stepEnemies();


		window.clear(sf::Color::Black);
		
		current_room->draw(window);
		player->draw(window);
		ui->draw(window);
		window.display();
	}
	delete current_player;
	delete ui;
	delete current_dungeon;
	return 0;
}


