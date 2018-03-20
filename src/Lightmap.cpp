//
// Created by Oliver on 20.03.2018.
//

#include "Lightmap.h"
#include "Room.h"

vector<vector<float>> Lightmap::lighting_mask_ = {
        {0.00, 0.15, 0.25, 0.15, 0.00},
        {0.15, 0.42, 0.70, 0.42, 0.15},
        {0.25, 0.70, 1.00, 0.70, 0.25},
        {0.15, 0.42, 0.70, 0.42, 0.15},
        {0.00, 0.15, 0.25, 0.15, 0.00}};

sf::BlendMode Lightmap::subtract_alpha_ = sf::BlendMode(
        sf::BlendMode::Zero, sf::BlendMode::DstColor, sf::BlendMode::Equation::Add,
        sf::BlendMode::Zero, sf::BlendMode::Factor::OneMinusSrcAlpha, sf::BlendMode::Equation::Add);

Lightmap::Lightmap(Room* parent_room) : parent_room_(parent_room)
{
    width_  = parent_room->getWidth();
    height_ = parent_room->getHeight();

    if(!lighting_canvas_.create((unsigned)width_  * TILE_SIZE, (unsigned)height_ * TILE_SIZE))
    {
        cout << "Error creating lighting_canvas!" << endl;
    }

    intensity_.resize(height_);
    for(auto& row : intensity_)
    {
        row.resize(width_);
    }
}

void Lightmap::update()
{
    //cout << "Lightmap::update" << endl;

    for(auto& row : intensity_)
        for(auto& intensity : row)
            intensity = 0.f;

    static size_t mask_size      = lighting_mask_.size();
    static size_t mask_size_half = mask_size / 2;

    for(auto& light : light_sources_)
    {
        for (size_t mask_y = 0; mask_y < mask_size; ++mask_y)
        {
            for (size_t mask_x = 0; mask_x < mask_size; ++mask_x)
            {
                size_t x = light.second.x_ - mask_size_half + mask_x;
                size_t y = light.second.y_ - mask_size_half + mask_y;
                if(x >= width_ || y >= height_)
                    continue;

                auto& intensity = intensity_.at(y).at(x);
                intensity += lighting_mask_.at(mask_y).at(mask_x);
                if(intensity > 1.0)
                   intensity = 1.0;
            }
        }
    }
}

void Lightmap::draw(sf::RenderTarget& target)
{
    lighting_canvas_.clear(sf::Color::Black);

    // create "prefab" light
    sf::RectangleShape light(sf::Vector2f(TILE_SIZE, TILE_SIZE));

    for (size_t y = 0; y < height_; ++y)
    {
        for (size_t x = 0; x < width_; ++x)
        {
            if(intensity_[y][x] == 0.0)
                continue;

            light.setPosition(x * TILE_SIZE, y * TILE_SIZE);
            // set light color according to intesity on that field
            light.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(255 * intensity_[y][x])));
            // draw the light on the canvas using custom blend mode
            lighting_canvas_.draw(light, subtract_alpha_);
        }
    }
    // finally draw the finished lightmap on the window by taking its texture
    lighting_canvas_.display();
    target.draw(sf::Sprite(lighting_canvas_.getTexture()));
}

size_t Lightmap::addLightSource(Position pos)
{
    size_t id;
    if(available_ids_.empty())
    {
        id = highest_available_id_++;
    }
    else
    {
        id = available_ids_.back();
        available_ids_.pop_back();
    }
    light_sources_[id] = pos;
    //cout << "Lightmap::addLightSource: " << id << " -> " << pos << endl;
    return id;
}

void Lightmap::updateLightSource(size_t id, Position pos)
{
    if(light_sources_.find(id) == light_sources_.end())
    {
        cout << "Lightmap::updateLightSource: source doesnt exist!" << endl;
        return;
    }
    //cout << "Lightmap::updateLightSource: " << id << " -> " << light_sources_[id] << "=>";
    light_sources_[id] = pos;
    //cout << light_sources_[id] << endl;
}

void Lightmap::removeLightSource(size_t id)
{
    light_sources_.erase(id);
    available_ids_.push_back(id);
    //cout << "Lightmap::removeLightSource: " << id << endl;
}
