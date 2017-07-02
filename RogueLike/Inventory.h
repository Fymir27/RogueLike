#pragma once
#include "Common.h"
#include "Types.h"

class Item;
class Inventory
{
private:
	list<Item*> items_;
	size_t limit_;
	size_t rows_;
	sf::Texture background_;
	sf::Sprite sprite_;
	sf::Font font_;

public:
	Inventory();
	~Inventory();

	void click(sf::Event event);

	void sort() {};
	bool addItem(Item* item, bool silent = false); //returns false when full
	friend std::ostream& operator<<(std::ostream& out, const Inventory* inv);
	void removeItem(Item* item);
	sf::Sprite & getSprite();
	void draw(sf::RenderWindow& window, Position pos = { 0,0 });
};
