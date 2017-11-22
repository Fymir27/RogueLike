#pragma once

#include "Common.h"

class Animation //only sprite sheets with a single row!
{
    public:
    void init(sf::Texture* tex, unsigned image_count, unsigned frames);
    sf::IntRect update();

    private:
    unsigned current_image_ = 0;
    unsigned image_count_;
    unsigned frames_;
    unsigned image_width_;
    sf::IntRect rect_;
};

