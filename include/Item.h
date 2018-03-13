//
// Created by Oliver on 13.03.2018.
//

#pragma once


#include "GameObject.h"
#include "Factory.h"

class Character;
class Inventory;
class Field;

namespace Items
{

    class IItemEffect
    {
        public:
        virtual bool apply(Character* target) = 0;
    };

    class RestoreHp : public IItemEffect
    {
        public:
        explicit RestoreHp(unsigned amount) : amount_(amount) {}

        inline bool apply(Character* target) final
        {
            target->heal(amount_);
        }

        private:
        unsigned amount_;
    };

    class RestoreMana : public IItemEffect
    {
        public:
        explicit RestoreMana(unsigned amount) : amount_(amount) {}

        inline bool apply(Character* target) final
        {
            target->restoreMana(amount_);
        }

        private:
        unsigned amount_;
    };

    class Damage : public IItemEffect
    {
        public:
        explicit Damage(unsigned amount) : amount_(amount) {}

        inline bool apply(Character* target) final
        {
            target->damage(amount_);
        }

        private:
        unsigned amount_;
    };

    IItemEffect* createEffect(xml_node effect_node);

    class Item : public GameObject
    {
        friend class Inventory;

        public:
        Item(string const& name, string const& descr, size_t max_stack_size, string const& texture);
        Item(Item* orig);
        void addUseEffect(IItemEffect* e);
        void addThrowEffect(IItemEffect* e);
        void setUseEffects(vector<IItemEffect*> effects);
        void setThrowEffects(vector<IItemEffect*> effects);

        virtual bool use(Character* who);
        virtual bool throwAt(Character* target);
        virtual bool drop(Field* where);

        private:
        // draw related
        sf::Texture texture_;
        sf::Sprite  sprite_;

        // general item attributes
        size_t max_stack_size_;

        // effects
        vector<IItemEffect*> use_effects_;
        vector<IItemEffect*> throw_effects_;
    };
}

template <>
Factory<Items::Item>::Factory();



