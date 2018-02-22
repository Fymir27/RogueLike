#include "Effects.h"
#include "Room.h"
#include "Player.h"

list<shared_ptr<Effect>> Effect::effects_;
list<shared_ptr<Effect>> Effect::effects_persistent_;

void Effect::addEffect(shared_ptr<Effect>e, bool persistent)
{
    if(persistent)
        effects_persistent_.push_back(e);
    else
        effects_.push_back(e);
}

void Effect::drawEffects(sf::RenderWindow &window)
{
    //persistent effects:
    list<shared_ptr<Effect>> tmp = effects_persistent_;
    for(const shared_ptr<Effect>& e : tmp)
    {
        //cout << "Effect::drawEffects: use count: " << e.use_count() << endl;
        if(e.use_count() == 2) //ability effect ran out
            effects_persistent_.remove(e);

        e->draw(window);
    }

    //other effects:
    tmp = effects_;
    for(const shared_ptr<Effect>& e : tmp)
    {
        e->draw(window);
        if(!e->active_)
        {
            effects_.remove(e);
        }
    }
}

void Effect::removeEffect(shared_ptr<Effect>e)
{
    effects_persistent_.remove(e);
}


//----------------------------------------------

MovingSprite::MovingSprite(string filename, float speed) : speed_(speed)
{
    if(!tex_.loadFromFile(filename))
        cout << "Couln't load " << filename << endl;
    sprite_.setTexture(tex_);
}

MovingSprite::MovingSprite(AnimatedSprite* anim, float speed) : speed_(speed)
{
    anim_sprite_ = anim;
    animated_ = true;
}

void MovingSprite::draw(sf::RenderWindow &window)
{
    static size_t dur_left = dur_;

    if(animated_)
    {
        anim_sprite_->draw(window);
        anim_sprite_->move(step_);
    }
    else
    {
        window.draw(sprite_);
        sprite_.move(step_);
    }
    if (--dur_ <= 0)
    {
        active_ = false;
    }
}



void MovingSprite::aim(sf::Vector2f from, sf::Vector2f to)
{
    //cout << "Moving Effect aim from" << from.x << "|" << from.y << " to " << to.x << "|" << to.y << endl;
    if(animated_)
        anim_sprite_->setPosition(from);
    else
        sprite_.setPosition(from);

    sf::Vector2f path = (to - from);
    float path_length = getVectorLength(path);

    if(path_length == 0)
    {
        dur_ = 1;
        step_ = sf::Vector2f(0,0);
        return;
    }

    sf::Vector2f dir = path / path_length;
    step_ = dir * speed_;
    dur_ = (size_t)(path_length / speed_);
    active_ = true;
}

//----------------------------------------------

ParticleEffect::ParticleEffect(sf::Color color, size_t count) : col_(color), count_(count)
{
    //this constructor just creates a template!
}

void ParticleEffect::draw(sf::RenderWindow &window)
{
    const int randomize_every = 8;
    static size_t randomize_in = 0;
    static sf::Vector2f prev_pos;

    if((current_room != room_) && (target_ != current_player)) //only draw in generated room!
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


void ParticleEffect::setPosition(sf::Vector2f pos)
{
    pos_ = pos;
    active_ = true;
}

void ParticleEffect::setTarget(Character *character)
{
    target_ = character;
    active_ = true;
}

Effect* ParticleEffect::createInstance()
{
    return new ParticleEffect(this);
}

ParticleEffect::ParticleEffect(ParticleEffect* orig) : col_(orig->col_), count_(orig->count_)
{
    generateParticles(col_, count_);
}
