//
// Created by oliver on 02/03/18.
//

#pragma once

#include "Singleton.h"

#include "Common.h"

class Enemy;

// Produces entities by their names. Requirements for use:
// - Producible entities (PE) MUST have a cctor in the form of: PE(PE* original)
// - Implement template specialization: Factory<PE>()
template <class Entity>
class Factory : public Singleton<Factory<Entity>>
{
public:
    // creates entity by calling its cctor
    shared_ptr<Entity> createEntity(string const& entity_name);

    // returns names to all PEs
    vector<string> getEntityNames();

    // returns the template entities
    Entity const* getTemplateEntity(string const& entity_name);

    // Implementation (Specialization) of this constructor should:
    // - Initialize templates for all PEs
    // - Store all templates together with their corresponding names in entity_templates_
    // - Store all possible names in entity_names_
    Factory();

private:
    // entity_templates_[entity_name] = std::make_unique<PE>();
    map<string, unique_ptr<Entity>> entity_templates_;

    // stores names of all PEs
    vector<string> entity_names_;
};


template <class Entity>
shared_ptr<Entity> Factory<Entity>::createEntity(string const& entity_name)
{
    //cout << "Factory<" << typeid(Entity).name() << ">::createEntity" << endl;
    return std::make_shared<Entity>(entity_templates_[entity_name].get());
}

template<class Entity>
vector<string> Factory<Entity>::getEntityNames()
{
    return entity_names_;
}

template <class Entity>
Entity const* Factory<Entity>::getTemplateEntity(string const& entity_name)
{
    return entity_templates_[entity_name].get();
}

