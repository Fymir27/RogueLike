//
// Created by Oliver on 13.03.2018.
//

#include "Command.h"
#include "Player.h"
#include "UI.h"

vector<std::unique_ptr<Command>> Command::history_;

Command::Command(sf::Keyboard::Key const& key)
{
    switch (key)
    {
        case sf::Keyboard::Escape:  type_ = ESC;break;
        case sf::Keyboard::Space:	type_ = SPACE; break;
        case sf::Keyboard::Return:  type_ = CONFIRM; break;

        case sf::Keyboard::Right:   type_ = DIRECTION; dir_ = RIGHT; break;
        case sf::Keyboard::Left:    type_ = DIRECTION; dir_ = LEFT;  break;
        case sf::Keyboard::Up:      type_ = DIRECTION; dir_ = UP;    break;
        case sf::Keyboard::Down:    type_ = DIRECTION; dir_ = DOWN;  break;

        case sf::Keyboard::Numpad1: type_ = NUMBER; nr_ = 1; break;
        case sf::Keyboard::Numpad2: type_ = NUMBER; nr_ = 2; break;
        case sf::Keyboard::Numpad3: type_ = NUMBER; nr_ = 3; break;
        case sf::Keyboard::Numpad4: type_ = NUMBER; nr_ = 4; break;
        case sf::Keyboard::Numpad5: type_ = NUMBER; nr_ = 5; break;
        case sf::Keyboard::Numpad6: type_ = NUMBER; nr_ = 6; break;
        case sf::Keyboard::Numpad7: type_ = NUMBER; nr_ = 7; break;
        case sf::Keyboard::Numpad8: type_ = NUMBER; nr_ = 8; break;
        case sf::Keyboard::Numpad9: type_ = NUMBER; nr_ = 9; break;
        case sf::Keyboard::Num1:    type_ = NUMBER; nr_ = 1; break;
        case sf::Keyboard::Num2:    type_ = NUMBER; nr_ = 2; break;
        case sf::Keyboard::Num3:    type_ = NUMBER; nr_ = 3; break;
        case sf::Keyboard::Num4:    type_ = NUMBER; nr_ = 4; break;
        case sf::Keyboard::Num5:    type_ = NUMBER; nr_ = 5; break;
        case sf::Keyboard::Num6:    type_ = NUMBER; nr_ = 6; break;
        case sf::Keyboard::Num7:    type_ = NUMBER; nr_ = 7; break;
        case sf::Keyboard::Num8:    type_ = NUMBER; nr_ = 8; break;
        case sf::Keyboard::Num9:    type_ = NUMBER; nr_ = 9; break;

        // Items
        case sf::Keyboard::U: type_ = USEITEM;   break;
        case sf::Keyboard::T: type_ = THROWITEM; break;
        case sf::Keyboard::D: type_ = DROPITEM;  break;

        // Spells
        case sf::Keyboard::C: type_ = CASTSPELL; break;

        default: valid_ = false;
    }
}

bool Command::execute()
{
    static ECommandType current_activity = ESC;

    // history_ must never be empty because this function always checks history_
    if(history_.empty())
    {
        history_.push_back(std::make_unique<Command>(sf::Keyboard::Escape));
        history_.push_back(std::make_unique<Command>(sf::Keyboard::Escape));
        history_.push_back(std::make_unique<Command>(sf::Keyboard::Escape));
    }

    auto prev = history_.back().get();
    history_.push_back(unique_ptr<Command>(this));

    switch (type_)
    {
        case SPACE:
            switch (current_activity)
            {
                case CASTSPELL:
                    current_activity = SPACE;
                    return current_player->castSpell(prev->nr_, UP /*doesnt matter bec self cast*/, true);

                case THROWITEM:
                    current_activity = SPACE;
                    return false;

                default:
                    current_activity = SPACE;
                    current_player->rest();
                    return true;
            }

        case DIRECTION:
            switch (current_activity)
            {
                case CASTSPELL:
                    current_activity = DIRECTION;
                    return current_player->castSpell(prev->nr_, dir_);

                case THROWITEM:
                    current_activity = DIRECTION;
                    return false;

                default:
                    return current_player->move(current_player->getPosition() + DELTA_POS[dir_]);
            }


        case USEITEM:
        case THROWITEM:
        case DROPITEM:
        case CASTSPELL:
            UI::displayText("Which one?");
            current_activity = type_;
            return false; //wait for next input

        case NUMBER:
            switch (current_activity)
            {
                case USEITEM:
                    current_player->getInventory()->useItem(nr_);
                    break;

                case CASTSPELL:
                case THROWITEM:
                    UI::displayText("Which direction?");
                    break;

                default: break;
            }
            return false; //wait for next input

        case ESC:
            current_activity = ESC;
            break;

        case CONFIRM:
            current_activity = ESC;
            break;
    }
    return false;
}
