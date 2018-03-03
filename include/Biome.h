//
// Created by oliver on 02/03/18.
//

#pragma once

#include "Common.h"

namespace Biome
{
    template <typename T>
    using Attribute = T;

    typedef Attribute<int> Temperature;
    typedef Attribute<int> Humidity;
    typedef Attribute<
            enum
            {
                STONE,
                SAND,
                GRASS,
                FOREST,
                SNOW,
                ICE,
            }>            FloorType;

    enum Equality
    {
        SMALLER,
        EQUAL,
        BIGGER
    };

    typedef list<Attribute> Data;

}
