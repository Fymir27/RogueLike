#pragma once

#include "Common.h"
#include "Types.h"

class Textbox;
class UI
{
	public:
	UI();
	~UI();

	void draw(sf::RenderWindow & window);
	//void click(const sf::Event& event);
	//
	static void displayText(string text);

	private:
	Position pos_inv_;
	Position pos_stats_;
	Position pos_bottom_text_;

	sf::Font font_;
	unsigned int font_size_ = 20;

	sf::Text stat_names_;
	sf::Text stat_values_;

	static Textbox* textbox_;

	sf::Texture tex_inv_;
};