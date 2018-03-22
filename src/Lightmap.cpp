//
// Created by Oliver on 20.03.2018.
//

#include "Lightmap.h"
#include "Room.h"

vector<vector<float>> Lightmap::lighting_mask_ = {
        {0.00, 0.00, 0.10, 0.15, 0.10, 0.00, 0.00},
        {0.00, 0.17, 0.20, 0.40, 0.20, 0.17, 0.00},
        {0.10, 0.20, 0.50, 0.70, 0.50, 0.20, 0.10},
        {0.15, 0.40, 0.70, 1.00, 0.70, 0.40, 0.15},
        {0.10, 0.20, 0.50, 0.70, 0.50, 0.20, 0.10},
        {0.00, 0.17, 0.20, 0.40, 0.20, 0.17, 0.00},
        {0.00, 0.00, 0.10, 0.15, 0.10, 0.00, 0.00}
};

vector<Position> Lightmap::light_shape_ = {
        {-1, -3}, { 0, -3}, { 1, -3}, { 2, -2}, // top
        { 3, -1}, { 3,  0}, { 3,  1}, { 2,  2}, // right
        { 1,  3}, { 0,  3}, {-1,  3}, {-2,  2}, // bottom
        {-3,  1}, {-3,  0}, {-3, -1}, {-2, -2}  // left
};

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
    seen_.resize(height_);
    for(auto& row : seen_)
    {
        row.resize(width_);
        for (size_t i = 0; i < width_; ++i)
        {
            row[i] = false;
        }
    }
    illuminated_.resize(height_);
    for(auto& row : illuminated_)
    {
        row.resize(width_);
        for (size_t i = 0; i < width_; ++i)
        {
            row[i] = false;
        }
    }
}

void Lightmap::update()
{
    //cout << "Lightmap::update" << endl;

    for(auto& row : illuminated_)
    {
        for (size_t i = 0; i < width_; ++i)
        {
            row[i] = false;
        }
    }

    //light up already seen areas
    for (size_t x = 0; x < width_; ++x)
    {
        for (size_t y = 0; y < height_; ++y)
        {
            if(seen_[y][x])
            {
                intensity_[y][x] = 0.2;
            }
            else
                intensity_[y][x] = 0.0;
        }
    }

    for(auto const& light : light_sources_)
    {
        intensity_[light.second.y_][light.second.x_] = 1.f;
        seen_[light.second.y_][light.second.x_] = 1.f;
        for (size_t i = 0; i < 4; ++i)
        {
            lightUp(light.second, light.second + DELTA_POS[i], static_cast<Direction>(i), 3);
        }
    }

    /*
    // use Bresenham's algorithm to cast rays to each vertex of the light shape
    vector<Position> fields;
    float light_power;
    for(auto const& light : light_sources_)
    {
        for (auto const& vertex : light_shape_)
        {
            light_power = 1.f;
            fields.clear();
            fields = bresenham(light.second, light.second + vertex, false);
            for(auto const& field : fields)
            {
                if(!current_room->isInside(field))
                    continue;

                intensity_[field.y_][field.x_] += light_power;
                if( intensity_[field.y_][field.x_] > 1.f)
                    intensity_[field.y_][field.x_] = 1.f;
                seen_[field.y_][field.x_] = true;
                light_power /= 1.5f;
            }
        }
    }
    */

    /*
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
                if(intensity_[y][x] != 0.0)
                    seen_[y][x] = true;
            }
        }
    }
     */
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

void Lightmap::lightUp(Position origin, Position pos, Direction dir, int power)
{
    static float distance_factor = 0.8;

    if(power == 0)
        return;

    if(!current_room->isInside(pos))
        return;

    intensity_[pos.y_][pos.x_] = 1.f - (0.2f * realDistance(pos, origin));
    seen_[pos.y_][pos.x_] = true;
    illuminated_[pos.y_][pos.x_] = true;

    if(current_room->getField(pos)->getFieldStatus() == SOLID)
        return;

    Position delta[2];
    Position cur;
    float intesity;

    if(dir == UP || dir == DOWN)
    {
        delta[0] = DELTA_POS[RIGHT];
        delta[1] = DELTA_POS[LEFT];
    }
    else
    {
        delta[0] = DELTA_POS[UP];
        delta[1] = DELTA_POS[DOWN];
    }

    for (size_t j = 0; j < 2; ++j)
    {
        cur = pos;
        for (size_t i = 0; i < power; ++i)
        {
            cur = cur + delta[j];
            if(!current_room->isInside(cur))
                break;

            intesity = 1.f - (0.2f * realDistance(cur, origin));
            if(illuminated_[cur.y_][cur.x_])
            {
                intensity_[cur.y_][cur.x_] += intesity/2;
            }
            else //if(intesity > intensity_[cur.y_][cur.x_]); // only update if brighter than FOV
            {
                intensity_[cur.y_][cur.x_]   = intesity/2;
            }
            illuminated_[cur.y_][cur.x_] = true;
            seen_[cur.y_][cur.x_] = true;
            if(current_room->getField(cur)->getFieldStatus() == SOLID)
                break;
        }
    }

    lightUp(origin, pos + DELTA_POS[dir], dir, power - 1);
}
