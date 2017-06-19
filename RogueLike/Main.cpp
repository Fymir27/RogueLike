#include "Room.h"
#include "Field.h"
#include "Screen.h"
#include "Player.h"
#include "HealingPotion.h"
#include "Item.h"

#include <iostream>
#include <vector>
#include <thread>

using std::cout;
using std::endl;


int main()
{
	char* filename = "Room1.txt";
	Room* room = new Room(filename);

	Player* player = new Player("Oliver", { 1,3 }, { 100,100,10,10,10 });

	Item* item = new LargeHealingPotion(3);
	player->addItem(item);

	player->printInventory();

	item = new LargeHealingPotion(5);
	player->addItem(item);

	player->printInventory();

	item = new LargeHealingPotion(7);
	player->addItem(item);

	player->printInventory();

	Screen* screen = new Screen(600, 500, filename);
	std::thread graphics(&Screen::init, screen);
	screen->setRoom(room);
	graphics.join();
	return 0;
}