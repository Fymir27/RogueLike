#include "UI.h"
#include "Types.h"
#include "Room.h"
#include "Player.h"

UI::UI()
{
	if (!font_.loadFromFile("../fonts/Arcade.ttf"))
		cout << "Failed to load UI font!" << endl;
	
	pos_inv_ = { static_cast<unsigned int>(current_room->getColCount() * TILE_SIZE), 0 };

	stat_names_.setFont(font_);
	Position offset = { 4, TILE_SIZE * 3 };
	pos_stats_ = pos_inv_ + offset;
	stat_names_.setPosition(pos_stats_.x_, pos_stats_.y_);
	stat_names_.setString("Hitpoints\n"
												"Mana\n"
												"Str\n"
												"Int\n"
												"Dex\n");

	stat_values_.setFont(font_);
	stat_values_.setPosition(pos_stats_.x_ + TILE_SIZE *2, pos_stats_.y_);


}

UI::~UI()
{
}

void UI::draw(sf::RenderWindow& window)
{
	current_player->getInventory()->draw(window, pos_inv_);
	window.draw(stat_names_);

	Stats stats = current_player->getStats();
	char buffer[1024];
	sprintf_s(buffer, 1024,
		"%d\n"
		"%d\n"
		"%d\n"
		"%d\n"
		"%d\n",
		stats.hp_, stats.mana_, stats.str_, stats.int_, stats.dex_);
	stat_values_.setString(buffer);
	window.draw(stat_values_);
}