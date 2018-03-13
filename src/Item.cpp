//
// Created by Oliver on 13.03.2018.
//

#include "UI.h"
#include "Item.h"


Item::Item(string const& name, string const& descr, size_t max_stack_size, string const& texture) :
        GameObject(name, descr), max_stack_size_(max_stack_size)
{
    if(!texture_.loadFromFile(texture))
    {
        cout << "[ERROR] Could not load item texture for: " << name << endl;
    }
    sprite_.setTexture(texture_);
}

bool Item::use(Character* who)
{
    UI::displayText("You can't use that!");
    return false;
}

bool Item::throwAt(Character* target)
{
    UI::displayText("You can't throw that!");
    return false;
}

bool Item::drop(Field* where)
{
    UI::displayText("You can't put that down!");
    return false;
}
