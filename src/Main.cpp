#include "Room.h"
#include "Dungeon.h"
#include "Field.h"
#include "Player.h"
#include "Potions.h"
#include "ItemOLD.h"
#include "Item.h"
#include "Utils.h"
#include "Common.h"
#include "UI.h"
#include "Enemy.h"
#include "Ressource.h"
#include "PlayerClasses.h"
#include "Effects.h"
#include "Biome.h"
#include "Command.h"
#include <ctime>

using Biomes::Biome;
using Items::Item;

enum Turn
{
    PLAYER,
    PLAYER_EFFECTS,
    ENEMY,
};

Turn turn = PLAYER;

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
	Factory<Item>::get();

	srand((unsigned int)std::time(0));
    random_engine.seed((unsigned)std::time(0));

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
	//current_room->placeItem(current_room->getFreePosition(), new SmallHealingPotion(5));
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
				Command* command = new Command(event.key.code);
				if (command->isValid())
				{
					if(command->execute())
					{
						turn = PLAYER_EFFECTS;
						current_player->advanceEffects();
						current_player->coolDownAbilities();
					}
					else
					{
						// command didn't do anything
					}
				}
				else
				{
                    delete command;
					cout << "Unknown Command!" << endl;
				}
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
//			if (event.type == sf::Event::KeyPressed)
//				if(event.key.code == sf::Keyboard::Space)
//				{
//					current_player = UI::startMenu();
//					if(current_player == nullptr)
//						return 0;
//					//goto MAIN_LOOP;
//				}
		}
	}

    // destroy singletons
    Factory<Enemy>::destroy();
    Factory<Biome>::destroy();
	Factory<Item>::destroy();

	delete ui;
	delete current_dungeon;
	return 0;
}


