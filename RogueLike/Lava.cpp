#include "Field.h"
#include "UI.h"
#include "Character.h"

Position Lava::trigger(Character* who)
{
	who->damage(15);
	UI::displayText("You stepped into Lava. Ouch!");
	return pos_;
}