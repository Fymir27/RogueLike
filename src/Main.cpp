#include "Room.h"
#include "Dungeon.h"
#include "Field.h"
#include "Player.h"
#include "HealingPotion.h"
#include "Item.h"
#include "Utils.h"
#include "Common.h"
#include "UI.h"
#include "Enemy.h"
#include "Ressource.h"
#include "PlayerClasses.h"
#include "Effects.h"
#include "Biome.h"
#include <ctime>

using Biomes::Biome;

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
                valid = current_player->castSpell(prev.nr_, UP /*doesnt matter bec self cast*/, true);
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
		current_player->coolDownAbilities();
	}
}

int main()
{
	cout << endl;
	cout << "#################################" << endl;
	cout << "###-- Welcome to RogueLike! --###" << endl;
	cout << "#################################" << endl;
	cout << endl;

    // init singletons
	Factory<Enemy>::get();
    Factory<Biome>::get();

	srand((unsigned int)std::time(NULL));
	current_dungeon = new Dungeon();	
	current_dungeon->readRoomPartsFromFile();
	cout << "Dungeon Layout:" << endl;
	current_dungeon->generate(15, 10);

    current_dungeon->generateBiomes(current_room);
    current_dungeon->printBiomes();

	Character::init_exp_needed();
	UI::setDefaultFont("fonts/8bitOperatorPlus-Regular.ttf");

	cout << endl;
	cout << "////////////////////////////////////////////////////////" << endl;
	cout << "//    The game is fully loaded and will start now.    //" << endl;
	cout << "//                  Good Luck!                        //" << endl;
	cout << "////////////////////////////////////////////////////////" << endl;
	cout << endl;

    //-- Start-menu --//
    current_player = UI::startMenu();
    //current_player = new Mage("Oli", current_room->getFreePosition());
    if(current_player == nullptr)     //player closed the start Menu
        return 0;

	//-- create main window --//
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RogueLike", sf::Style::Default);
	window.setFramerateLimit(60);

	auto ui = new UI();

	//------- Test Area ----------//
	current_room->placeItem(current_room->getFreePosition(), new SmallHealingPotion(5));
    Biome desert = { "Desert", 40, 50, Biomes::SAND };
    Biomes::Condition<Biomes::Temperature> likes_it_warm = { 30, Biomes::BIGGER };
    Biomes::Condition<Biomes::Temperature> likes_it_cold = { 20, Biomes::SMALLER };
    Biomes::Condition<Biomes::FloorType> likes_sand = { Biomes::SAND, Biomes::EQUAL };
    //auto likes_it_warm = Biomes::Condition<Biomes::Temperature, int>(new Biomes::Temperature(30), Biomes::BIGGER);
    cout << "likes_it_warm: " << (desert.satisfies(likes_it_warm) ? "yes" : "no") << endl;
    cout << "likes_it_cold: " << (desert.satisfies(likes_it_warm) ? "yes" : "no") << endl;
    cout << "likes_sand: " << (desert.satisfies(likes_it_warm) ? "yes" : "no") << endl;
	//----------------------------//

	sf::Event event;
    //sf::RenderTexture lighting;
    //lighting.create(WINDOW_WIDTH, WINDOW_HEIGHT);

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

        if(current_room->getEffectCount() == 0) //advances the phase when animations have finished playing
        {
            switch(turn)
            {
                case PLAYER_EFFECTS:
                    turn = ENEMY;
                    current_room->stepEnemies();
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

        //-- Lighting test ---------------------------------------------------------------------------------------------
        /*
        lighting.clear();

        //latern/character light
        auto subtractAlpha = sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::DstColor, sf::BlendMode::Equation::Add,
                                         sf::BlendMode::Zero, sf::BlendMode::Factor::OneMinusSrcAlpha, sf::BlendMode::Equation::Add);
        sf::RectangleShape light(sf::Vector2f(TILE_SIZE, TILE_SIZE));


        light.setFillColor(sf::Color(0,0,0,200));
        light.setPosition(worldToScreen(current_player->getPosition()));
        lighting.draw(light, subtractAlpha);

        light.setFillColor(sf::Color(0,0,0,140));
        for(int i = 0; i < 4; i++) //direct neighbours
        {
            light.setPosition(worldToScreen(current_player->getPosition() + DELTA_POS[i]));
            lighting.draw(light, subtractAlpha);
        }

        //diagonal neighbours
        light.setFillColor(sf::Color(0,0,0,100));
        light.setPosition(worldToScreen(current_player->getPosition() + Position(-1,-1))); lighting.draw(light, subtractAlpha);
        light.setPosition(worldToScreen(current_player->getPosition() + Position(1,-1)));  lighting.draw(light, subtractAlpha);
        light.setPosition(worldToScreen(current_player->getPosition() + Position(-1,1)));  lighting.draw(light, subtractAlpha);
        light.setPosition(worldToScreen(current_player->getPosition() + Position(1,1)));   lighting.draw(light, subtractAlpha);

        //2-away neighbours
        light.setFillColor(sf::Color(0,0,0,40));
        light.setPosition(worldToScreen(current_player->getPosition() + Position(0,-2)));  lighting.draw(light, subtractAlpha);
        light.setPosition(worldToScreen(current_player->getPosition() + Position(2,0)));   lighting.draw(light, subtractAlpha);
        light.setPosition(worldToScreen(current_player->getPosition() + Position(0,2)));   lighting.draw(light, subtractAlpha);
        light.setPosition(worldToScreen(current_player->getPosition() + Position(-2,0)));  lighting.draw(light, subtractAlpha);


        lighting.display();
        sf::Sprite finished_lighting(lighting.getTexture());
        int darkness = 100;
        finished_lighting.setColor(sf::Color(0,0,0, darkness));
        window.draw(finished_lighting);
        */


        ui->draw(window);
		window.display();

		if (current_player->dead())
		{
			//delete current_player;
			//current_player = nullptr;
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
			if (event.type == sf::Event::KeyPressed)
				if(event.key.code == sf::Keyboard::Space)
				{
					current_player = UI::startMenu();
					if(current_player == nullptr)
						return 0;
					//goto MAIN_LOOP;
				}
		}
	}

    // destroy singletons
    Factory<Enemy>::destroy();
    Factory<Biome>::destroy();

	delete ui;
	delete current_dungeon;
	return 0;
}


