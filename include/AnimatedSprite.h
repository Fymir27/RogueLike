#pragma once

#include "Common.h"

class AnimatedSprite //only sprite sheets with a single row!
{
    public:
    AnimatedSprite(string const& filename, unsigned frames, unsigned image_width = TILE_SIZE);
    void setPosition(sf::Vector2f pos);
    void move(sf::Vector2f delta);
    void draw(sf::RenderTarget& target);

    private:
    sf::Texture tex_;
    sf::Sprite sprite_;
    vector<sf::IntRect> rects_;
    unsigned image_count_;
    unsigned frames_;
};

