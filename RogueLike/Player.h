#pragma once
#include "Common.h"
#include "Character.h"

class Player : public Character
{
private:
	sf::Texture texture_;
	sf::Sprite sprite_;
	sf::Font font_;
	sf::Text text_;

public:
	Player(const string name, const Position pos, const Stats stats);
	void processInput(const sf::Event& event);
	void draw(sf::RenderWindow& window);
	Inventory & getInventory() const;
	Inventory & printInventory() const;
};

extern Player* current_player;