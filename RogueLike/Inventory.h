#pragma once
#include <iostream>
#include <list>
#include <SFML\Graphics.hpp>

using std::list;
using std::cout;
using std::endl;

class Item;
class Inventory
{
private:
	list<Item*> items_;
	size_t limit_;
	size_t rows_;
	sf::Texture background_;
	sf::Sprite sprite_;

public:
	Inventory();
	~Inventory();

	void sort() {};
	bool addItem(Item* item, bool silent = false); //returns false when full
	friend std::ostream& operator<<(std::ostream& out, const Inventory* inv);
	void removeItem(Item* item);
	void draw(sf::RenderWindow& window);
};
