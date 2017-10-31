#include "UI.h"
#include "Types.h"
#include "Room.h"
#include "Player.h"
#include "Textbox.h"
#include "Minimap.h"
#include "Ressource.h"

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
	pos_inv_   =       { 23 * TILE_SIZE + 10, 0                  };
	pos_stats_ =       { 23 * TILE_SIZE + 14, 200                };
	pos_bottom_text_ = { 10                 , TILE_SIZE * 9 + 10 };

	stat_names_.setPosition(pos_stats_.x_, pos_stats_.y_);
	stat_values_.setPosition(pos_stats_.x_ + 150, pos_stats_.y_);

	//-- set fixed Text --//
	stat_names_.setString("Level\n"
						  "Exp:\n"
						  "Strength\n"
						  "Endurance\n"
						  "Dexterity\n"
						  "Intelligence\n"
						  "Willpower\n");

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
	string buffer = std::to_string(current_player->getLevel()) + '\n' +
					/*Experience bar*/            '\n' +
					std::to_string(stats.str_)  + '\n' +
					std::to_string(stats.end_)  + '\n' +
					std::to_string(stats.dex_)  + '\n' +
					std::to_string(stats.int_)  + '\n' +
					std::to_string(stats.will_) + '\n';
	stat_values_.setString(buffer.c_str());
	window.draw(stat_values_);
	textbox_->draw(window);
	Minimap::draw(window);
}

void UI::displayText(string text)
{
	textbox_->displayText(text);
}


UI::Bar::Bar(sf::Vector2f pos, sf::Vector2f size, size_t border_size, sf::Color bar_color) :
		pos_(pos), size_(size), border_size_(border_size), bar_color_(bar_color),
		border_(size_),
		background_(size_ - sf::Vector2f(border_size_ * 2, border_size_ * 2)),
		bar_(size_ - sf::Vector2f(border_size_ * 2, border_size_ * 2)) 
{
    
	border_.setFillColor(sf::Color::Black);  
	background_.setFillColor(sf::Color(150, 150, 150)); 
    bar_.setFillColor(bar_color_);
}

void UI::Bar::update(float value_, float max_)
{
	cur_ = size_.x * value_ / max_;
}

void UI::Bar::draw(sf::RenderWindow& window)
{
	border_.    setPosition (pos_);
	background_.setPosition (pos_ + sf::Vector2f(border_size_, border_size_));
	bar_.       setPosition (pos_ + sf::Vector2f(border_size_, border_size_));

	bar_.setSize(sf::Vector2f(cur_, size_.y));
	bar_.setFillColor(bar_color_);

	window.draw(border_);
	window.draw(background_);
	window.draw(bar_);
}

UI::CharacterInfo::~CharacterInfo()
{
	if(ressource1_ != NULL)
		delete ressource1_;
	if(ressource2_ != NULL)
		delete ressource2_;
	if(ressource3_ != NULL)
		delete ressource3_;
}