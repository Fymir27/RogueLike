#pragma once

#include "Common.h"
#include "Types.h"

class Ressource;
class Textbox;
class Character;
class UI
{
    class Bar
    {
			private:
			sf::Vector2f size_;
			Ressource const& ressource_;
			sf::RectangleShape background_;
			sf::RectangleShape bar_;

			public:
			Bar(sf::Vector2f pos_, sf::Vector2f size_, Ressource const& ress, sf::Color bar_color = sf::Color::Red);
			void draw(sf::RenderWindow& window);
    };

    struct CharacterInfo
    {
			Character* source_;
      sf::Vector2f pos_;
      sf::Text title_;   //Font needs manual setting
      Bar* bar1_ = NULL; //hp
      Bar* bar2_ = NULL; //mana
      Bar* bar3_ = NULL; //exp

			CharacterInfo(sf::Vector2f pos, Character* source);
      ~CharacterInfo();

			void draw(sf::RenderWindow& window);
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