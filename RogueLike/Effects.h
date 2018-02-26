#pragma once

#include "Common.h"
#include "Utils.h"
#include "AnimatedSprite.h"
#include "Character.h"

class Room;

class Effect : public sf::Drawable
{
public:
    virtual Effect* createInstance() = 0;
    void setPosition(sf::Vector2f pos) { pos_ = pos; };
    bool isActive() { return active_; }
    virtual void update() = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

protected:
    bool active_ = true;
    sf::Vector2f pos_;
};


class MovingSprite : public Effect
{
public:
    explicit MovingSprite(AnimatedSprite* anim, float speed = 1);
    void aim(sf::Vector2f from, sf::Vector2f to);
    virtual Effect* createInstance();
    virtual void update();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    ~MovingSprite();
private:
    explicit MovingSprite(MovingSprite* orig);

    AnimatedSprite* anim_sprite_;
    float speed_;
    sf::Vector2f step_;
    size_t dur_;
};
/*
class ParticleEffect : public Effect
{
public:
    ParticleEffect(sf::Color color, size_t count); //just creates a template
    virtual Effect* createInstance();              //use this if you want a concrete effect!
    virtual void setPosition(sf::Vector2f pos);
    virtual void setTarget(Character* character);
    void draw(sf::RenderWindow& window);
private:
    sf::Color col_;
    size_t count_;
    sf::Vector2f pos_;
    Character* target_ = nullptr;

    sf::VertexArray particles_;
    Room* room_ = nullptr;

    explicit ParticleEffect(ParticleEffect* orig);
    void generateParticles(sf::Color, size_t count);
    void randomizeParticles();
};
 */

class BigParticleEffect : public Effect
{
public:
    struct Particle
    {
        sf::RectangleShape shape_;
        bool growing_ = true;
    };
    BigParticleEffect(sf::Color col_, size_t count, unsigned max_size = 7, unsigned min_size = 2);
    virtual void setPosition(sf::Vector2f pos) { pos_ = pos; };
    virtual Effect* createInstance();
    void update();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Color col_;
    size_t count_;
    unsigned min_size_;
    unsigned max_size_;
    vector<Particle> particles_;

    explicit BigParticleEffect(BigParticleEffect* orig);
    void generateParticles(sf::Color col, size_t count);
    void randomize_Particles();
};

