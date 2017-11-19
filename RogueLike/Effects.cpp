#include "Effects.h"

list<Effect*> Effect::effects_;

Effect::Effect(string filename)
{
    if(!tex_.loadFromFile(filename))
    {
        cout << "Failed to load " << filename << endl;
    }
    sprite_.setTexture(tex_);
}

void Effect::addEffect(Effect *e)
{
    effects_.push_back(e);
}

void Effect::drawEffects(sf::RenderWindow &window)
{
    list<Effect*> tmp = effects_;
    for(Effect* e : tmp)
    {
        e->draw(window);
        if(!e->active_)
        {
            delete e;
            effects_.remove(e);
        }
    }
}

//----------------------------------------------

MovingEffect::MovingEffect(string filename, sf::Vector2f from, sf::Vector2f to, float speed) : Effect(filename)
{
    sprite_.setPosition(from);
    dur_ = (30 / speed);
    step_ = (to - from) / (float)dur_;
}

void MovingEffect::draw(sf::RenderWindow &window)
{
    window.draw(sprite_);
    sprite_.move(step_);
    if(dur_-- == 0)
        active_ = false;
}
