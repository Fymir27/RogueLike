#pragma once
#include "Common.h"
#include "Types.h"
#include "Character.h"

class Enemy : public Character
{
	private:
		static vector<Enemy*> enemies_;
	public:
		
		//-ctors-//
		//Enemy();
		Enemy(string name, Position pos, Stats stats, string texture);

		static void stepAll();
		static void drawEnemies(sf::RenderWindow& window);
		virtual void step();


};