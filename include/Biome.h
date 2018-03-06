//
// Created by oliver on 02/03/18.
//

#pragma once


#include "Common.h"

namespace Biomes
{

    enum EFloorType
    {
        STONE,
        SAND,
        GRASS,
        FOREST,
        SNOW,
        ICE,
    };

    enum EComparison
    {
        SMALLER, EQUAL, BIGGER
    };

    template <typename T>
    class Attribute
    {
        public:
        const T value_;
        bool compare(EComparison comp, Attribute<T> other) const;

        protected:
        explicit Attribute(T value);
    };

    class Temperature : public Attribute<int>
    {
        public:
        Temperature(int value);
    };

    class Humidity : public Attribute<int>
    {
        public:
        Humidity(int value);
    };

    class FloorType : public Attribute<EFloorType>
    {
        public:
        FloorType(EFloorType value);
    };

    template <typename T>
    struct Condition
    {
        public:
        //TODO: Condition(xml_node condition_node);
        const T attribute_;
        const EComparison eq_;
    };

    struct Biome
    {
        //TODO: Biome(xml_node biome_node);

        Temperature temp_;
        Humidity    hum_;
        FloorType   floor_;

        bool satisfies(Condition<Temperature> cond);
        bool satisfies(Condition<Humidity> cond);
        bool satisfies(Condition<FloorType> cond);
    };

}