#include "Room.h"
#include "Dungeon.h"
#include "Field.h"
#include "Player.h"
#include "HealingPotion.h"
#include "Item.h"
#include "Types.h"
#include "Common.h"
#include "UI.h"
#include "Enemy.h"
#include "Ressource.h"
#include "PlayerClasses.h"
#include "Effects.h"
#include <ctime>


enum Turn
{
    PLAYER,
    PLAYER_EFFECTS,
    ENEMY,
};

Turn turn = PLAYER;

enum InputType
{
	INVALID,
	REST,
	MOVE,
	ITEM,
	SPELL,
    ESC,
    CONFIRM
};

struct Command
{
	InputType type_ = INVALID;
	Direction dir_;
	int nr_;
};

Command getCommand(const sf::Keyboard::Key& key)
{
	Command c;

	switch (key)
	{
        case sf::Keyboard::Escape:    c.type_ = ESC; break;

		case sf::Keyboard::Space:	  c.type_ = REST; break;

        case sf::Keyboard::Return:    c.type_ = CONFIRM; break;

		case sf::Keyboard::Right:   c.type_ = MOVE; c.dir_ = RIGHT; break;
		case sf::Keyboard::Left:    c.type_ = MOVE; c.dir_ = LEFT; break;
		case sf::Keyboard::Up:      c.type_ = MOVE; c.dir_ = UP; break;
		case sf::Keyboard::Down:    c.type_ = MOVE; c.dir_ = DOWN; break;

		case sf::Keyboard::Numpad1: c.type_ = ITEM; c.nr_ = 1; break;
		case sf::Keyboard::Numpad2: c.type_ = ITEM; c.nr_ = 2; break;
		case sf::Keyboard::Numpad3: c.type_ = ITEM; c.nr_ = 3; break;
		case sf::Keyboard::Numpad4: c.type_ = ITEM; c.nr_ = 4; break;
		case sf::Keyboard::Numpad5: c.type_ = ITEM; c.nr_ = 5; break;
		case sf::Keyboard::Numpad6: c.type_ = ITEM; c.nr_ = 6; break;
		case sf::Keyboard::Numpad7: c.type_ = ITEM; c.nr_ = 7; break;
		case sf::Keyboard::Numpad8: c.type_ = ITEM; c.nr_ = 8; break;
		case sf::Keyboard::Numpad9: c.type_ = ITEM; c.nr_ = 9; break;

		case sf::Keyboard::Num1:    c.type_ = SPELL; c.nr_ = 1; break;
		case sf::Keyboard::Num2:    c.type_ = SPELL; c.nr_ = 2; break;
		case sf::Keyboard::Num3:    c.type_ = SPELL; c.nr_ = 3; break;
		case sf::Keyboard::Num4:    c.type_ = SPELL; c.nr_ = 4; break;
		case sf::Keyboard::Num5:    c.type_ = SPELL; c.nr_ = 5; break;
		case sf::Keyboard::Num6:    c.type_ = SPELL; c.nr_ = 6; break;
		case sf::Keyboard::Num7:    c.type_ = SPELL; c.nr_ = 7; break;
		case sf::Keyboard::Num8:    c.type_ = SPELL; c.nr_ = 8; break;
		case sf::Keyboard::Num9:    c.type_ = SPELL; c.nr_ = 9; break;

		default:
			c.type_ = INVALID;
			cout << "UNKNOWN COMMAND" << endl;
			break;
	}

	return c;
}

void processInput(const sf::Event& event, sf::RenderWindow& window)
{
    static Command prev;
	//cout << "Input detected!" << endl;
	auto key = event.key.code;
	Command com = getCommand(key);
	bool valid = false; //valid action?

	switch (com.type_)
	{
		case REST:
            if (prev.type_ == SPELL)
                valid = current_player->castSpell(prev.nr_, current_player);
            else
            {
                valid = true;
                current_player->rest();
            }
			break;

		case MOVE:
            if (prev.type_ == SPELL)
                valid = current_player->castSpell(prev.nr_, com.dir_);
            else
			    valid = current_player->move(current_player->getPosition() + DELTA_POS[com.dir_]);
			break;

		case ITEM:
			valid = current_player->getInventory()->useItem(com.nr_);
			break;

		case SPELL:
           UI::displayText("Which direction?");
           valid = false; //wait for next input to be move
           break;

        case ESC:
            //interrupts spell casting automatically bec. prev is now ESC
            break;

        case INVALID:
            //just do nothing
            break;

        case CONFIRM:
            //nope
            break;
	}
    prev = com;
	if (valid)
	{
        turn = PLAYER_EFFECTS;
		current_player->advanceEffects();
	}
}

bool turnReady()
{
    return (Effect::getEffectCount() == 0);
}

int main()
{
	cout << endl;
	cout << "#################################" << endl;
	cout << "###-- Welcome to RogueLike! --###" << endl;
	cout << "#################################" << endl;
	cout << endl;

	srand((unsigned int)std::time(NULL));
	current_dungeon = new Dungeon();	
	current_dungeon->readRoomPartsFromFile();
	cout << "Dungeon Layout:" << endl;
	current_dungeon->generate(15, 10);

	Character::init_exp_needed();

    //-- create window --//
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RogueLike", sf::Style::Default);
    window.setFramerateLimit(60);
    sf::Event event;


    //-- Main menu --//
	UI::setDefaultFont("../fonts/8bitOperatorPlus-Regular.ttf");
    current_player = UI::startMenu(window);

	//-- create player --//
	//current_player = new Mage("Oliver", current_room->getFreePosition());
    //current_player = new Warrior("Oliver", current_room->getFreePosition());
    //current_player = new Thief("Oliver", current_room->getFreePosition());

	UI* ui = new UI();
	
	cout << endl;
	cout << "////////////////////////////////////////////////////////" << endl;
	cout << "//    The game is fully loaded and will start now.    //" << endl; 
	cout << "//                  Good Luck!                        //" << endl;
	cout << "////////////////////////////////////////////////////////" << endl;
	cout << endl;

	//------- Test Area ----------//
	//current_player->grantExp(33);
	current_room->placeItem(current_room->getFreePosition(), new SmallHealingPotion(5));
	//----------------------------//

	//-- main loop --//
	while (window.isOpen())
	{
		//-- get input --//
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
            {
                window.close();
            }
			else if (event.type == sf::Event::KeyPressed && turn == PLAYER)
            {
                processInput(event, window);
            }
		}

		//-- Game Logic --//

        if(Effect::getEffectCount() == 0) //advances the phase when animations have finished playing
        {
            switch(turn)
            {
                case PLAYER_EFFECTS:
                    turn = ENEMY;
                    current_room->stepEnemies();
                    current_room->deleteDeadEnemies();
                    break;

                case ENEMY:
                    turn = PLAYER;
                    break;

                case PLAYER:
                    break;
            }
        }

		if (current_player->dead())
		{
			UI::displayText("");
			UI::displayText("Game Over");
			UI::displayText("");
		}


        //-- Draw Stuff --//
		window.clear(sf::Color::Black);
		
		current_room->draw(window);   //draws Fields and enemies
		current_player->draw(window);
		Effect::drawEffects(window);
		ui->draw(window);
		window.display();

		if (current_player->dead())
		{
			delete current_player;
			current_player = NULL;
			break;
		}
		
	}

	while (window.isOpen())
	{
		//-- Game over; waiting for the User to quit --//
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	delete ui;
	delete current_dungeon;
	return 0;
}


