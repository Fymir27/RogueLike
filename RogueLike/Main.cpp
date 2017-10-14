#include "Room.h"
#include "Dungeon.h"
#include "Field.h"
#include "Player.h"
#include "HealingPotion.h"
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
	else if(key == sf::Keyboard::Num4)  item_slot = 4;
	else if(key == sf::Keyboard::Num5)  item_slot = 5;
	else if(key == sf::Keyboard::Num6)  item_slot = 6;
	else if(key == sf::Keyboard::Num7)  item_slot = 7;
	else if(key == sf::Keyboard::Num8)  item_slot = 8;
	else if(key == sf::Keyboard::Num9)  item_slot = 9;
	else if(key == sf::Keyboard::Space) current_room->stepEnemies(); //wait a turn

	if(item_slot > 0)
		current_player->getInventory()->useItem(item_slot);

	if(!(new_pos == current_player->getPosition())) 
	{
		if(current_player->move(new_pos)) //valid move?
		{
			current_room->stepEnemies();
		}
	}
}

int main()
{
	cout << endl;
	cout << "#################################" << endl;
	cout << "###-- Welcome to RogueLike! --###" << endl;
	cout << "#################################" << endl;
	cout << endl;

	//-- load new dungeon from file --//
	current_dungeon = new Dungeon();
	srand(42);
	cout << "Dungeon Layout:" << endl;
	current_dungeon->readRoomPartsFromFile();
	current_dungeon->generateLayout(20, 13);
	//current_dungeon->loadFromFile("Dungeon1.txt");

	//-- create player --//
	Stats player_stats = {};
	player_stats.hp_[CUR] = 100;
	player_stats.hp_[MAX] = 100;
	player_stats.mana_[CUR] = 100;
	player_stats.mana_[MAX] = 100;
	player_stats.str_  = 10;
	player_stats.int_  = 10;
	player_stats.dex_  = 10;

	Player* player = new Player("Oliver", { 1, 1 }, player_stats);
	current_player = player;

	//place Pickup item
	//current_room->placeItem({5,2}, new SmallHealingPotion(33));
	//Field* pickup = new Pickup({ 5, 3 }, new SmallHealingPotion(), 17);
	//current_room->addField(pickup);

	//spawn Enemy
	//Enemy* blob = new Enemy("Blobby", {10,4}, player_stats, "../images/enemy.png");
	//Enemy* blob2 = new Enemy("Blobby2", {9,4}, player_stats, "../images/enemy.png");

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
		if (current_player->getStats().hp_[CUR] <= 0)
		{
			UI::displayText("");
			UI::displayText("Game Over");
			UI::displayText("");
		}

		window.clear(sf::Color::Black);
		
		current_room->draw(window);
		player->draw(window);
		ui->draw(window);
		window.display();

		if (current_player->getStats().hp_[CUR] <= 0)
		{
			delete current_player;
			current_player = NULL;
			break;
		}
	}

	while (window.isOpen())
	{
		//-- Game over; waiting for the User to quit --//
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	delete ui;
	delete current_dungeon;
	return 0;
}


