#include "GameObject.h"


GameObject::GameObject(const string &name, const string &descr) : name_(name), description_(descr)
{

}

/* = default */
GameObject::GameObject(GameObject* src) : name_(src->name_), description_(src->description_)
{
    cout << "GameObject::cctor" << endl;
}
//*/
