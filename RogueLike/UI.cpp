#include "UI.h"
#include "Types.h"
#include "Room.h"
#include "Player.h"
#include "Textbox.h"

Textbox* UI::textbox_ = NULL;

UI::UI()
{
	//-- load font --//
	if (!font_.loadFromFile("../fonts/8bitOperatorPlus-Regular.ttf"))
		cout << "Failed to load UI font!" << endl;

	//-- set font --//
	stat_names_.setFont(font_);
	stat_values_.setFont(font_);

	//-- set size --//
	stat_names_.setCharacterSize(font_size_);
	stat_values_.setCharacterSize(font_size_);
	
	//-- set Positions --//
	pos_inv_   =       { 15 * TILE_SIZE + 10, 0                  };
	pos_stats_ =       { 15 * TILE_SIZE + 14, 200                };
	pos_bottom_text_ = { 10                 , TILE_SIZE * 7 + 10 };

	stat_names_.setPosition(pos_stats_.x_, pos_stats_.y_);
	stat_values_.setPosition(pos_stats_.x_ + 130, pos_stats_.y_);

	//-- set fixed Text --//
	stat_names_.setString("Hitpoints\n"
						  "Mana\n"
						  "Str\n"
						  "Int\n"
						  "Dex\n");

	textbox_ = new Textbox(3, 20, font_, 
		    pos_bottom_text_, 420, 80,
		    5, 5, sf::Color(100, 100, 100), sf::Color(50,50,50));

}

UI::~UI()
{
}

/*
void UI::click(const sf::Event& event)
{
	Position pos_clicked = { event.mouseButton.x, event.mouseButton.y };
	current_player->getInventory()->click(pos_clicked);
}
*/

void UI::draw(sf::RenderWindow& window)
{
	//-- draw inventory --//
	current_player->getInventory()->draw(window, pos_inv_);

	//-- draw stats --//
	window.draw(stat_names_);

	Stats stats = current_player->getStats();
	string buffer = std::to_string(stats.hp_[CUR])   + "/" + std::to_string(stats.hp_[MAX]) + '\n' +
					std::to_string(stats.mana_[CUR]) + "/" + std::to_string(stats.mana_[MAX]) + '\n' +
					std::to_string(stats.str_)  + '\n' +
					std::to_string(stats.int_)  + '\n' +
					std::to_string(stats.dex_);
	stat_values_.setString(buffer.c_str());
	window.draw(stat_values_);
	textbox_->draw(window);
}

void UI::displayText(string text)
{
	textbox_->displayText(text);
}