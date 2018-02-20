#include "Effects.h"
#include "Room.h"
#include "Player.h"

list<Effect*> Effect::effects_;
list<Effect*> Effect::effects_persistent_;

Effect::Effect(string filename)
{
    if(!tex_.loadFromFile(filename))
    {
        cout << "Failed to load " << filename << endl;
    }
    sprite_.setTexture(tex_);
}


Effect::Effect(AnimatedSprite *anim) : anim_(anim)
{

}

void Effect::addEffect(Effect *e, bool persistent)
{
    if(persistent)
        effects_persistent_.push_back(e);
    else
        effects_.push_back(e);
}

void Effect::drawEffects(sf::RenderWindow &window)
{
    //persistent effects:
    list<Effect*> tmp = effects_persistent_;
    for(Effect* e : tmp)
    {
        e->draw(window);
        if(!e->active_)
        {
            effects_.remove(e);
        }
    }

    //other effects:
    tmp = effects_;
    for(Effect* e : tmp)
    {
        e->draw(window);
        if(!e->active_)
        {
            effects_.remove(e);
        }
    }
}

void Effect::removeEffect(Effect *e)
{
    effects_persistent_.remove(e); //shouln't need to remove non persistent effects
}


//----------------------------------------------

MovingEffect::MovingEffect(string filename, float speed) : Effect(filename), speed_(speed)
{

}

MovingEffect::MovingEffect(AnimatedSprite* anim, float speed) : Effect(anim), speed_(speed)
{

}

void MovingEffect::draw(sf::RenderWindow &window)
{
    static size_t dur_left = dur_;

    if(anim_ == nullptr)
    {
        window.draw(sprite_);
        sprite_.move(step_);
    }
    else
    {
        anim_->draw(window);
        anim_->move(step_);
    }
    if (--dur_ <= 0)
    {
        active_ = false;
    }
}



void MovingEffect::aim(sf::Vector2f from, sf::Vector2f to)
{
    cout << "Moving Effect aim from" << from.x << "|" << from.y << " to " << to.x << "|" << to.y << endl;
    if(anim_ != nullptr)
        anim_->setPosition(from);
    else
        sprite_.setPosition(from);

    sf::Vector2f path = (to - from);
    float path_length = getVectorLength(path);
    cout << "Path length: " << path_length << endl;

    if(path_length == 0)
    {
        dur_ = 1;
        step_ = sf::Vector2f(0,0);
        return;
    }

    sf::Vector2f dir = path / path_length;
    step_ = dir * speed_;
    cout << "Step: " << step_.x << "|" << step_.y << endl;
    dur_ = (size_t)(path_length / speed_);
    cout << "Dur: " << dur_ << endl;
    active_ = true;
}

//----------------------------------------------

void ParticleEffect::draw(sf::RenderWindow &window)
{
    const int randomize_every = 8;
    static size_t randomize_in = 0;
    static sf::Vector2f prev_pos;

    if(current_room != room_ && target_ != current_player) //only draw in generated room!
        return;

    if(target_ != nullptr)
    {
        prev_pos = pos_;
        pos_ = worldToScreen(target_->getPosition());
    }

    if((randomize_in-- == 0) || (pos_ != prev_pos)) //every few frames or when character has moved
    {
        randomizeParticles();
        randomize_in = randomize_every;
    }

    window.draw(particles_);
}

ParticleEffect::ParticleEffect(Position tile, sf::Color color, size_t count)
{
    pos_ = worldToScreen(tile);
    generateParticles(color, count);
}

ParticleEffect::ParticleEffect(Character *target, sf::Color color, size_t count)
{
    target_ = target;
    pos_ = worldToScreen(target_->getPosition());
    generateParticles(color, count);
}

void ParticleEffect::generateParticles(sf::Color color, size_t count)
{
    room_ = current_room;
    for(size_t i = 0; i < count; i++) particles_.append(sf::Vertex(pos_, color));
}

void ParticleEffect::randomizeParticles()
{
    for(size_t i = 0; i < particles_.getVertexCount(); i++)
    {
        particles_[i].position = pos_ + sf::Vector2f(rand() % TILE_SIZE, rand() % TILE_SIZE);
    }
}
