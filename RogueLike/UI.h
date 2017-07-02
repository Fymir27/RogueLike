#pragma once

#include "Common.h"
#include "Types.h"

class UI
{
	public:
	UI();
	~UI();

	void draw(sf::RenderWindow & window);

	private:
	Position pos_inv_;
	Position pos_stats_;

	sf::Font font_;
	sf::Text stat_names_;
	sf::Text stat_values_;

	sf::Texture tex_inv_;
};