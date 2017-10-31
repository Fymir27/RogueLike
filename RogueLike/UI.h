#pragma once

#include "Common.h"
#include "Types.h"

class Textbox;
class UI
{
    class Bar
    {
    private:
        size_t cur_; //current width of bar
        sf::Vector2f pos_;
        sf::Vector2f size_;
        size_t border_size_;
        sf::Color bar_color_;

        sf::RectangleShape border_;
        sf::RectangleShape background_;
        sf::RectangleShape bar_;

    public:
        Bar(sf::Vector2f pos_, sf::Vector2f size_, size_t border_size = 0, sf::Color bar_color = sf::Color::Red);
        void draw(sf::RenderWindow& window);
        void setPosition(sf::Vector2f pos) { pos_ = pos; }
        void setBarColor(sf::Color col) { bar_color_ = col; }
        void update(float value_, float max_);
    };

    struct CharacterInfo
    {
        sf::Vector2f pos_;
        sf::Text description_;
        Bar* ressource1_ = NULL; //hp
        Bar* ressource2_ = NULL; //mana
        Bar* ressource3_ = NULL; //exp
        ~CharacterInfo();
    };

	public:
	UI();
	~UI();

	void draw(sf::RenderWindow & window);
	static void displayText(string text);


    private:

    sf::Font font_;
    unsigned int font_size_ = 20;

    //--- Inventory ---//
    Position pos_inv_;
    sf::Texture tex_inv_;

    //--- Stats ---//
	Position pos_stats_;
	sf::Text stat_names_;
	sf::Text stat_values_;

    Position pos_bottom_text_;
    static Textbox* textbox_;
    
	CharacterInfo player_info_;

};