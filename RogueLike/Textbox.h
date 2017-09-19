#pragma once

#include "Common.h"
#include "Types.h"

class Textbox
{
public:
	Textbox(size_t lines, size_t font_size, const sf::Font& font, 
		    Position pos, size_t width, size_t height,
		    size_t margin, size_t border, 
		    sf::Color fill_color, sf::Color border_color);

	void displayText(string text);
	void draw(sf::RenderWindow& window);

private:
	vector<sf::Text> lines_;
	Position pos_;
	size_t font_size_;

	size_t margin_;
	size_t border_;

	sf::Font font_;
	sf::RectangleShape inner_box_;
	sf::RectangleShape outer_box_;
	sf::Color fill_color_;
	sf::Color border_color_;
};