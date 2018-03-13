//
// Created by Oliver on 13.03.2018.
//

#pragma once


#include "GameObject.h"

class Character;
class Inventory;
class Field;

class Item : public GameObject
{
    friend class Inventory;

    public:
    Item(string const& name, string const& descr, size_t max_stack_size, string const& texture);
    virtual bool use(Character* who);
    virtual bool throwAt(Character* target);
    virtual bool drop(Field* where);

    private:
    // draw related
    sf::Texture texture_;
    sf::Sprite sprite_;

    // general item attributes
    size_t max_stack_size_;
};



