#pragma once
#include "Common.h"
#include "Types.h"

class Item;
class Inventory
{
private:
	vector<Item*> items_;
	vector<size_t> count_;
	size_t limit_;
	size_t rows_;
	sf::Texture background_;
	sf::Sprite sprite_;
	sf::Font font_;

public:
	Inventory();
	~Inventory();

	void click(Position pos_clicked);

	void sort() {};
	bool addItem(Item* item, unsigned int new_count); //returns false when full
	void removeItem(Item* item);
	void useItem(int slot);
	sf::Sprite & getSprite();
	void draw(sf::RenderWindow& window, Position pos = { 0,0 });
};
