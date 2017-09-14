#pragma once

#include "Common.h"
#include "Types.h"

class UI
{
	public:
	UI();
	~UI();

	void draw(sf::RenderWindow & window);
	void click(const sf::Event& event);

	private:
	Position pos_inv_;
	Position pos_stats_;
	Position pos_bottom_text_;

	sf::Font font_;
	unsigned int font_size_ = 20;

	sf::Text stat_names_;
	sf::Text stat_values_;
	sf::Text bottom_text_;

	sf::Texture tex_inv_;
};