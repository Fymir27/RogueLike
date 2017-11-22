#include "Animation.h"


void Animation::init(sf::Texture* tex, unsigned image_count, unsigned frames)
{
    image_count_ = image_count;
    frames_ = frames;
    image_width_ = tex->getSize().x / image_count;
    rect_.height = tex->getSize().y;
    rect_.width = image_width_;
}

sf::IntRect Animation::update()
{
    static unsigned frames_elapsed = 0;
    static unsigned switch_time = frames_ / image_count_;

    if (frames_elapsed >= switch_time)
    {
        current_image_ = (current_image_ + 1) % image_count_;
        frames_elapsed = 0;
    }

    rect_.left = image_width_ * current_image_;
    frames_elapsed++;
    return rect_;
}
