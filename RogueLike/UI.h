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
        Text(float x, float y, string content, unsigned int size = 15, sf::Color color = sf::Color::White);
        void draw(sf::RenderWindow& target);
        void setFont(sf::Font& font);
        static void setDefaultFont(sf::Font& font) { default_font_ = font; };

        void setString(string content) { text_.setString(content); }

    private:
        sf::Text text_;
        bool font_loaded_ = false;
        static sf::Font default_font_; //plz load me
    };

    struct Bar
    {
        sf::Vector2f size_;
        Ressource const &ressource_;
        sf::RectangleShape background_;
        sf::RectangleShape bar_;
        sf::Text text_cur_; //Font needs manual setting
        sf::Text text_max_;

        Bar(sf::Vector2f pos_, sf::Vector2f size_, Ressource const &ress, sf::Color bar_color = sf::Color::Red);

        void draw(sf::RenderWindow &window);
    };

    struct CharacterInfo
    {
        Character *source_;
        sf::Vector2f pos_;
        sf::Text title_;   //Font needs manual setting
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
    static Textbox *textbox_;

    Text abilities_;

    CharacterInfo player_info_;

};