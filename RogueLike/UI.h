#pragma once

#include "Common.h"
#include "Types.h"

class Ressource;

class Textbox;

class Character;

class UI
{
    //Wrapper around sf::Text
    class Text
    {
    public:
        Text(sf::Vector2f pos, string content = "TEXT", unsigned int size = default_font_size_, sf::Color color = sf::Color::White);
        void draw(sf::RenderWindow& target);
        void setFont(sf::Font& font);
        static void setDefaultFont(sf::Font& font) { default_font_ = font; };

        void setString(string content) { text_.setString(content); }

    private:
        sf::Text text_;
        bool font_loaded_ = false;
        static sf::Font default_font_;
        static unsigned int default_font_size_;
    };

    struct Bar
    {
        sf::Vector2f size_;
        Ressource const &ressource_;
        sf::RectangleShape background_;
        sf::RectangleShape bar_;
        Text text_cur_;
        Text text_max_;

        Bar(sf::Vector2f pos_, sf::Vector2f size_, Ressource const &ress, sf::Color bar_color = sf::Color::Red);

        void draw(sf::RenderWindow &window);
    };

    struct CharacterInfo
    {
        Character *source_;
        sf::Vector2f pos_;
        Text title_;
        Bar *bar1_ = NULL; //hp
        Bar *bar2_ = NULL; //mana
        Bar *bar3_ = NULL; //exp

        CharacterInfo(sf::Vector2f pos, Character *source);

        ~CharacterInfo();

        void draw(sf::RenderWindow &window);
    };

public:

    UI();

    ~UI();

    void draw(sf::RenderWindow &window);

    static void displayText(string text);


private:

    //--- Inventory ---//
    sf::Vector2f pos_inv_;
    sf::Texture tex_inv_;

    //--- Stats ---//
    sf::Vector2f pos_stats_;
    Text stat_names_;
    Text stat_values_;

    sf::Vector2f pos_bottom_text_;
    static Textbox *textbox_;

    sf::Vector2f pos_ability_bar_;
    Text abilities_;

    CharacterInfo player_info_;

};