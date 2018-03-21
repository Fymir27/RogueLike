//
// Created by Oliver on 20.03.2018.
//

#pragma once

#include "Common.h"
#include "Utils.h"

class Room;

class Lightmap
{
    public:
    Lightmap(Room* parent_room);

    /// propagates light of sources across the room
    void update();

    /// draws the lighmap (should be drawn on top of room)
    void draw(sf::RenderTarget& target);

    /// adds a new light source at pos and returns an id to it
    size_t addLightSource(Position pos);

    /// updates the Position of a light source with specific id
    void updateLightSource(size_t id, Position pos);

    /// removes lightsource with specific id
    void removeLightSource(size_t id);

    private:
    Room*  parent_room_;
    size_t width_;  // in fields,
    size_t height_; // not pixels!

    map<size_t, Position> light_sources_;
    vector<size_t>        available_ids_;
    size_t                highest_available_id_ = 0;

    vector<vector<float>> intensity_;
    vector<vector<bool>>  seen_;
    static vector<vector<float>> lighting_mask_;
    static vector<Position> light_shape_;

    sf::RenderTexture    lighting_canvas_;
    static sf::BlendMode subtract_alpha_;
};



