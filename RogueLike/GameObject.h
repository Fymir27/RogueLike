#pragma once

#include "Common.h"

class GameObject
{
public:
    string getName() const { return name_; }
    string getDescription() const { return description_; }

protected:
    string name_;
    string description_;
    GameObject(const string& name, const string& descr);
    GameObject(GameObject* src);
};
