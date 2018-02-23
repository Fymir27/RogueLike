#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(string const& filename, unsigned frames, unsigned int image_width)
{
    if(!tex_.loadFromFile(filename))
    {
        cout << "Couldn't load " << filename << endl;
    }
    sprite_.setTexture(tex_);
    image_count_ = tex_.getSize().x / image_width;
    for(unsigned i = 0; i < image_count_; i++)
    {
        rects_.push_back(sf::IntRect(image_width * i, 0, image_width, tex_.getSize().y));
    }
    frames_ = frames;
}


void AnimatedSprite::draw(sf::RenderWindow& target)
{
    static unsigned frames_elapsed = 0;
    static unsigned switch_time = frames_ / image_count_;
    static unsigned cur_image = 0;

    if (frames_elapsed >= switch_time)
    {
        cur_image = (cur_image + 1) % image_count_;
        frames_elapsed = 0;
    }
    sprite_.setTextureRect(rects_[cur_image]);
    target.draw(sprite_);
    frames_elapsed++;
}

void AnimatedSprite::setPosition(sf::Vector2f pos)
{
    sprite_.setPosition(pos);
}

void AnimatedSprite::move(sf::Vector2f delta)
{
    sprite_.move(delta);
}



