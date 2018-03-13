#pragma once
#include "Common.h"
#include "Utils.h"

class ItemOLD;
class Inventory
{
private:
	vector<ItemOLD*> items_;
	vector<size_t> count_;
	size_t limit_;
	size_t rows_;
	sf::Texture background_;
	sf::Sprite sprite_;
	sf::Font font_;

public:
	Inventory();
	~Inventory();

	void sort() {}; //not implemented
	bool addItem(ItemOLD* item); //returns false when full
	void removeItem(ItemOLD* item);
	bool useItem(int slot);
	sf::Sprite & getSprite();
	void draw(sf::RenderWindow& window, Position pos = { 0,0 });
};
