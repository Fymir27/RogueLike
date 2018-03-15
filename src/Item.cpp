//
// Created by Oliver on 13.03.2018.
//

#include "UI.h"
#include "Item.h"
#include "Character.h"

namespace Items
{

    Item::Item(string const& name, string const& descr, size_t max_stack_size, string const& texture) :
            GameObject(name, descr), max_stack_size_(max_stack_size)
    {
        if (!texture_.loadFromFile(texture))
        {
            cout << "[ERROR] Could not load item texture for: " << name << endl;
        }
        sprite_.setTexture(texture_);
    }

    Item::Item(Item* orig) : GameObject(orig), max_stack_size_(orig->max_stack_size_),
                             texture_(orig->texture_), use_effects_(orig->use_effects_),
                             throw_effects_(orig->throw_effects_)
    {
        sprite_.setTexture(texture_);
    }

    bool Item::use(Character* who)
    {
        if (use_effects_.empty())
        {
            UI::displayText("You can't use that!");
            return false;
        }
        UI::displayText("You use an item! (" + name_ + ")");
        for (auto& e : use_effects_)
        {
            e->apply(who);
        }
        return true;
    }

    bool Item::throwAt(Character* target)
    {
        if (throw_effects_.empty())
        {
            UI::displayText("You can't throw that!");
            return false;
        }
        UI::displayText("You throw " + name_ + " at " + target->getName());
        for (auto& e : throw_effects_)
        {
            e->apply(target);
        }
        return true;
    }

    bool Item::drop(Field* where)
    {
        UI::displayText("You can't put that down!");
        return false;
    }

    void Item::addUseEffect(IItemEffect* e)
    {
        use_effects_.push_back(e);
    }

    void Item::addThrowEffect(IItemEffect* e)
    {
        throw_effects_.push_back(e);
    }

    void Item::setUseEffects(vector<IItemEffect*> effects)
    {
        use_effects_ = effects;
    }

    void Item::setThrowEffects(vector<IItemEffect*> effects)
    {
        throw_effects_ = effects;
    }

    IItemEffect* createEffect(xml_node effect)
    {
        IItemEffect* i_effect = nullptr;
        string type = effect.name();
        if(type == "restoreHp")
        {
            i_effect = new RestoreHp(std::stoul(effect.child_value()));
        }
        else if(type == "restoreMana")
        {
            i_effect = new RestoreMana(std::stoul(effect.child_value()));
        }
        else if(type == "damage")
        {
            i_effect = new Damage(std::stoul(effect.child_value()));
        }
        cout << type << " " << effect.child_value() << endl;
        return  i_effect;
    }

    inline bool RestoreHp::apply(Character* target)
    {
        UI::displayText(target->getName() + " gets healed! (" + std::to_string(amount_) + ")");
        target->heal(amount_);
        return false;
    }

    inline bool RestoreMana::apply(Character* target)
    {
        UI::displayText(target->getName() + " restores some mana! ( " + std::to_string(amount_) + ")");
        target->restoreMana(amount_);
        return false;
    }

    inline bool Damage::apply(Character* target)
    {
        UI::displayText(target->getName() + " loses some health! (" + std::to_string(amount_) + ")");
        target->damage(amount_);
        return false;
    }
}

template <>
Factory<Items::Item>::Factory()
{
    const char* filename = "data/items.xml";
    std::ifstream file(filename);
    if(!file.is_open())
    {
        cout << "Factory<Items::Item> failed to open file " << filename << endl;
        return;
    }
    //-----------------------------------------------------------------------------------
    cout << "Loading in items..." << endl;
    xml_document doc;
    xml_parse_result res = doc.load(file);

    if(!res)
    {
        cout << "Parsing of " << filename << " failed:" << endl;
        cout << res << endl;
    }
    //-----------------------------------------------------------------------------------
    auto items = doc.child("items");

    for(auto item : items.children())
    {
        string name = item.attribute("name").value();

        if (name == "Template")
            continue;

        entity_names_.push_back(name);

        cout << separator << endl;
        cout << "Item: " << name << endl;
        string    texture         = item.attribute("texture").value();
        //-----------------------------------------------------------------------------------
        string    description     = item.child("description").child_value();
        cout << description << endl;
        size_t    max_stack_size  = std::stoul(item.child("maxStackSize").child_value());
        //-----------------------------------------------------------------------------------
        cout << "Use effects:" << endl;
        auto      usable_node = item.child("usable");
        vector<Items::IItemEffect*> use_effects;
        for (auto effect : usable_node)
        {
            use_effects.push_back(Items::createEffect(effect));
        }

        cout << "Throw effects: " << endl;
        auto throwable_node = item.child("throwable");
        vector<Items::IItemEffect*> throwable_effects;
        for (auto effect : throwable_node)
        {
            throwable_effects.push_back(Items::createEffect(effect));
        }

        entity_templates_[name] = std::make_unique<Items::Item>(name, description, max_stack_size , texture);
        entity_templates_[name]->setUseEffects(use_effects);
        entity_templates_[name]->setThrowEffects(throwable_effects);
        cout << separator << endl << endl;
    }

}