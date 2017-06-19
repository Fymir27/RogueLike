#pragma once

#include "Field.h"

class Wall : public Field
{
	public:
		Wall():Field('#') {};
};