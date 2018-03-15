#pragma once
#include "Common.h"
#include "Utils.h"

namespace Items
{
    class Item;
}

class Character;

class Inventory
{

    private:
	vector<shared_ptr<Items::Item>> items_;
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
	bool addItem(shared_ptr<Items::Item> item, size_t count); //returns false when full
	//void removeItem(Item* item);
	bool useItem(size_t slot);
    bool throwItem(size_t slot, Character* target);
	sf::Sprite & getSprite();
	void draw(sf::RenderWindow& window, Position pos = { 0,0 });
};
