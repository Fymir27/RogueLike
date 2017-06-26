#pragma once
#include "Character.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Player : public Character
{
private:
	sf::Texture texture_;
	sf::Sprite sprite_;

public:
	Player(const string name, const Position pos, const Stats stats);
	void processInput(const sf::Event& event);
	void draw(sf::RenderWindow& window);
};

