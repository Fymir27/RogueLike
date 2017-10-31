#include "Textbox.h"

Textbox::Textbox(size_t lines, size_t font_size,  const sf::Font& font, 
		    	Position pos, size_t width, size_t height,
		    	size_t margin, size_t border, 
		    	sf::Color fill_color, sf::Color border_color) :
				pos_(pos), font_size_(font_size), margin_(margin), border_(border),
				fill_color_(fill_color), border_color_(border_color)
{
	lines_.resize(lines);
	font_ = sf::Font(font);
	for(size_t i = 0; i < lines; i++)
	{
		sf::Text tmp;
		tmp.setFont(font_);
		tmp.setCharacterSize(font_size);
		lines_[i] = tmp;
	}
	outer_box_.setSize(sf::Vector2f(width, height));
	inner_box_.setSize(sf::Vector2f(width - border*2, height- border*2));
}

void Textbox::draw(sf::RenderWindow& window)
{
	outer_box_.setPosition(pos_.x_, pos_.y_);
	outer_box_.setFillColor(border_color_);
	window.draw(outer_box_);
	inner_box_.setPosition(pos_.x_ + border_, pos_.y_ + border_);
	inner_box_.setFillColor(fill_color_);
	window.draw(inner_box_);
	for(size_t i = 0; i < lines_.size(); i++)
	{
		lines_[i].setPosition(pos_.x_ + border_ + margin_, pos_.y_ + border_ + margin_ + i * font_size_ );
		window.draw(lines_[i]);
	}
}

void Textbox::displayText(string text)
{
	for(size_t i = 0; i < lines_.size() - 1; i++)
	{
		lines_[i].setString(lines_[i+1].getString());
	}
	lines_[lines_.size() - 1].setString(text);
}