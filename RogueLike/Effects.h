#pragma once

#include "Common.h"
#include "Types.h"
#include "Animation.h"
#include "Character.h"

class Room;

class Effect
{
public:
    static void drawEffects(sf::RenderWindow& window);
    static void addEffect(Effect* e, bool persistent = false);
    static size_t getEffectCount() { return effects_.size(); } //doesn't count persistent effects
    static void removeEffect(Effect* e);
protected:
    static list<Effect*> effects_;
    static list<Effect*> effects_persistent_;

    Effect(string filename); //effect with animation
    Effect();
    virtual void draw(sf::RenderWindow& window) = 0;
    sf::Texture tex_;
    sf::Sprite sprite_;
    bool active_ = true;
    Animation anim_;
};

class MovingEffect : public Effect
{
public:
    MovingEffect(string filename, sf::Vector2f from, sf::Vector2f to, float speed = 1);
private:
    void draw(sf::RenderWindow& window);
    sf::Vector2f step_;
    size_t dur_;
};

class ParticleEffect : public Effect
{
public:
    ParticleEffect(Position tile, sf::Color color, size_t count);
    ParticleEffect(Character* target, sf::Color color, size_t count); //intensity in %
    void draw(sf::RenderWindow& window);
private:
    Character* target_ = nullptr; //can also be static -> no target
    sf::Vector2f pos_;
    sf::Color color_;
    sf::VertexArray particles_;
    Room* room_ = nullptr; //TODO: Maybe let every room keep track of its effects??

    void generateParticles(sf::Color, size_t count);
    void randomizeParticles();
};


