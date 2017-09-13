#pragma once
#include "Common.h"
#include "Types.h"
#include "Character.h"

class Enemy : public Character
{
	public:
		
		//-ctors-//
		Enemy(string name, Position pos, Stats stats, string texture);

		virtual void step();


};