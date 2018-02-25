#pragma once

#include "Common.h"
#include "Utils.h"
#include "AnimatedSprite.h"
#include "Character.h"

class Room;

class Effect : public sf::Drawable
{
public:
    static void drawEffects(sf::RenderWindow& window);
    static void addEffect(shared_ptr<Effect> e, bool persistent = false);
    static size_t getEffectCount() { return effects_.size(); } //doesn't count persistent effects
    virtual Effect* createInstance() = 0;
    void setPosition(sf::Vector2f pos) { pos_ = pos; };
    virtual void update() = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

protected:
    static list<shared_ptr<Effect>> effects_; //game pauses while playing these
    static list<shared_ptr<Effect>> effects_persistent_; //removed manually

    bool active_ = true;
    sf::Vector2f pos_;
};

/*
class MovingSprite : public Effect
{
public:
    explicit MovingSprite(string filename, float speed = 1); //static sprite
    explicit MovingSprite(AnimatedSprite* anim, float speed = 1);
    void aim(sf::Vector2f from, sf::Vector2f to);
    virtual Effect* createInstance() { cout << "Not implemented yet!" << endl; return nullptr; }
private:
    void draw(sf::RenderWindow& window);

    sf::Texture tex_;
    sf::Sprite sprite_;
    AnimatedSprite* anim_sprite_;
    bool animated_ = false;
    float speed_;
    sf::Vector2f step_;
    size_t dur_;
};

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
    Room* room_ = nullptr; //TODO: Maybe let every room keep track of its effects?

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

