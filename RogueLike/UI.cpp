#include <sstream>
#include "UI.h"
#include "Types.h"
#include "Common.h"
#include "Room.h"
#include "Player.h"
#include "Textbox.h"
#include "Minimap.h"
#include "Ressource.h"
#include "Abilities.h"
#include "PlayerClasses.h"

Textbox* UI::textbox_ = NULL;
sf::Font UI::Text::default_font_;
unsigned int UI::Text::default_font_size_ = 20;

//-------------------------------------------------------------------------------------------------
UI::Text::Text(sf::Vector2f pos, string content, unsigned int size, sf::Color color)
{
    text_.setPosition(pos);
    text_.setCharacterSize(size);
    text_.setColor(color);
    text_.setString(content);
}

void UI::Text::setFont(sf::Font &font)
{
    text_.setFont(font);
    font_loaded_ = true;
}

void UI::Text::draw(sf::RenderWindow& window)
{
    if(!font_loaded_)
    {
        setFont(default_font_);
    }

    window.draw(text_);
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
UI::Bar::Bar(sf::Vector2f pos, sf::Vector2f size, Ressource const& ress, sf::Color bar_color) :
        text_cur_(pos, "0", 15),
        text_max_(pos + sf::Vector2f(size.x - 30, 0), "", 15),
        size_(size), ressource_(ress), background_(size), bar_(size)
{
    background_.setPosition(pos);
    background_.setFillColor(sf::Color(150, 150, 150));
    bar_.setPosition(pos);
    bar_.setFillColor(bar_color);
}

void UI::Bar::draw(sf::RenderWindow& window)
{
    bar_.setSize(sf::Vector2f(ressource_.relative() * size_.x, size_.y));
    window.draw(background_);
    window.draw(bar_);
    text_cur_.setString(std::to_string(ressource_.cur()));
    text_max_.setString(std::to_string(ressource_.max()));
    text_cur_.draw(window);
    text_max_.draw(window);
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
UI::CharacterInfo::~CharacterInfo()
{
    delete bar1_;
    delete bar2_;
    delete bar3_;
}

void UI::CharacterInfo::draw(sf::RenderWindow & window)
{
    title_.setString(source_->getName() + ", Lv " + std::to_string(source_->getLevel()) + " " + source_->getClassName());
    title_.draw(window);

    if(bar1_ != nullptr)
    {bar1_->draw(window);}
    if (bar2_ != nullptr)
    {bar2_->draw(window);}
    if (bar3_ != nullptr)
    {bar3_->draw(window);}
}

UI::CharacterInfo::CharacterInfo(sf::Vector2f pos, Character * source) : title_(pos, "Character", 16),
                                                                         source_(source), pos_(pos)
{
    bar1_ = new Bar(pos_ + sf::Vector2f(0, 20), { 200, 20 }, source_->getHp());
    bar2_ = new Bar(pos_ + sf::Vector2f(0, 40), { 200, 20 }, source_->getMana(), sf::Color::Blue);
    bar3_ = new Bar(pos_ + sf::Vector2f(0, 60), { 200, 20 }, source_->getExp(), sf::Color(200,0,200));
}
//-------------------------------------------------------------------------------------------------

template <class T>
UI::Menu::List<T>::List(sf::Vector2f pos, map<string, T> items) :
        pos_(pos), items_(items), cursor_dot_(5), offset_(25)
{
    size_t entry_nr = 0;
    for(auto const& el : items_)
    {
        entries_.push_back(Text(pos_ + sf::Vector2f(0, offset_ * entry_nr++), el.first ));
    }
    entry_count_ = entries_.size();
}

template <class T>
void UI::Menu::List<T>::draw(sf::RenderWindow& window)
{
    for(auto& entry : entries_)
    {
        cout << "Drawing list entry " << entry.getString() << endl;
        entry.draw(window);
    }
    cursor_dot_.setPosition(pos_ + sf::Vector2f(-5 - cursor_dot_.getRadius(), cursor_ * offset_ + offset_/3));
    window.draw(cursor_dot_);
}

template <class T>
T UI::Menu::List<T>::select()
{
    return items_[entries_[cursor_].getString()];
}

//-------------------------------------------------------------------------------------------------
UI::UI() : pos_inv_        ({750, 0  }),
           pos_stats_      ({765, 200}),
           stat_names_     ({750, 200}, "Statname"),
           stat_values_    ({900, 200}, "Statvalue"),
           pos_bottom_text_({ 10, 300}),
           player_info_    ({500, 300}, current_player),
           abilities_      ({ 15, 450}, "Abilities:")
{
	//-- set fixed Text --//
	stat_names_.setString("Strength\n"
						  "Endurance\n"
						  "Dexterity\n"
						  "Intelligence\n"
						  "Willpower\n");

	textbox_ = new Textbox(5, 20, Text::getDefaultFont(),
		    pos_bottom_text_, 420, 120,
			5, 5, sf::Color(100, 100, 100), sf::Color(50,50,50));
			
}

UI::~UI()
{
	delete textbox_;
}

void UI::setDefaultFont(string filename)
{
    sf::Font font;
    if (!font.loadFromFile(filename))
        cout << "Failed to load UI font!" << endl;

    Text::setDefaultFont(font);

}

void UI::draw(sf::RenderWindow& window)
{
	//-- draw inventory --//
	current_player->getInventory()->draw(window, {(int)pos_inv_.x, (int)pos_inv_.y});

	//-- draw stats --//
	stat_names_.draw(window);

	Stats stats = current_player->getStats();
	string buffer =
					std::to_string(stats.str_)  + '\n' +
					std::to_string(stats.end_)  + '\n' +
					std::to_string(stats.dex_)  + '\n' +
					std::to_string(stats.int_)  + '\n' +
					std::to_string(stats.will_) + '\n';
	stat_values_.setString(buffer.c_str());
	stat_values_.draw(window);

	textbox_->draw(window);

	player_info_.draw(window);

	Minimap::draw(window);

    //-- Draw Abilites --//
    std::stringstream s;
    s << "Abilities:\n";
    vector<Ability*> player_abilities = current_player->getAbilities();

    size_t ab_nr = 1;
    for(Ability* ability : player_abilities)
    {
        if(ability == nullptr)
            continue;

        s << '[' << ab_nr++ << ']' << " " << ability->getName() << " (" <<  ability->getCost()<< ')';
        //s << " - " << ability->getDescription();
        s << '\n';
    }
    abilities_.setString(s.str());
    abilities_.draw(window);
}

void UI::displayText(string text)
{
	textbox_->displayText(text);
}

Player* UI::startMenu(sf::RenderWindow& window)
{
    Menu::List<PlayerClass> class_list({25,25}, getPlayerClasses());
    
    string player_name = "Oliver";
    sf::Event event;
    while (window.isOpen())
    {
        //-- get input --//
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                PlayerClass player_class;
                sf::Keyboard::Key key = event.key.code;
                if(key == sf::Keyboard::Down)
                    class_list.moveCursorDown();
                else if(key == sf::Keyboard::Up)
                    class_list.moveCursorUp();
                else if(key == sf::Keyboard::Return) //forwards to game
                {
                    player_class = class_list.select();
                    return getPlayer(player_class, "Oliver");
                }
            }
            window.clear(sf::Color::Black);
            class_list.draw(window);
            window.display();
        }
    }
}
//-------------------------------------------------------------------------------------------------
