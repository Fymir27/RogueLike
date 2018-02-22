#pragma once

#include "Common.h"
#include "Utils.h"
#include "Player.h"
#include "PlayerClasses.h"

class Ressource;

class Textbox;

class Character;

class UI
{
    //Wrapper around sf::Text
    class Text
    {
    public:
        Text(sf::Vector2f pos, string content = "TEXT", unsigned int size = default_font_size_,
             sf::Color color = sf::Color::White);

        void draw(sf::RenderWindow& target);

        void setFont(sf::Font& font);

        static void setDefaultFont(sf::Font& font)
        { default_font_ = font; };

        static sf::Font& getDefaultFont()
        { return default_font_; };

        void setString(string content)
        { text_.setString(content); }

        string getString() const { return text_.getString(); }

    private:
        sf::Text text_;
        bool font_loaded_ = false;
        static sf::Font default_font_;
        static unsigned int default_font_size_;
    };

    struct Bar
    {
        sf::Vector2f size_;
        Ressource const& ressource_;
        sf::RectangleShape background_;
        sf::RectangleShape bar_;
        Text text_cur_;
        Text text_max_;

        Bar(sf::Vector2f pos_, sf::Vector2f size_, Ressource const& ress, sf::Color bar_color = sf::Color::Red);

        void draw(sf::RenderWindow& window);
    };

    struct CharacterInfo
    {
        Character* source_;
        sf::Vector2f pos_;
        Text title_;
        Bar* bar1_ = NULL; //hp
        Bar* bar2_ = NULL; //mana
        Bar* bar3_ = NULL; //exp

        CharacterInfo(sf::Vector2f pos, Character* source);

        ~CharacterInfo();

        void draw(sf::RenderWindow& window);
    };

public:
    class Menu
    {
    public:

        template<class T>
        class List
        {
        public:
            List(sf::Vector2f pos, map<string, T> items);
            void draw(sf::RenderWindow& window);
            void moveCursorDown()   { cursor_ = (cursor_ + 1) % entry_count_; }
            void moveCursorUp()     { cursor_ = (cursor_ - 1) % entry_count_; }
            T select();
        private:
            sf::Vector2f pos_;
            float offset_;
            vector<Text> entries_; //visual representation
            map<string, T> items_; //actual data
            unsigned entry_count_ = 0;
            unsigned cursor_ = 0;
            sf::CircleShape cursor_dot_;
        };
    };



public:

    UI();

    ~UI();

    static void setDefaultFont(string filename);
    void draw(sf::RenderWindow& window);
    static void displayText(string text);

    static Player* startMenu();

private:

    //--- Inventory ---//
    sf::Vector2f pos_inv_;
    sf::Texture tex_inv_;

    //--- Stats ---//
    sf::Vector2f pos_stats_;
    Text stat_names_;
    Text stat_values_;

    sf::Vector2f pos_bottom_text_;
    static Textbox* textbox_;

    sf::Vector2f pos_ability_bar_;
    Text abilities_;

    CharacterInfo player_info_;

};

