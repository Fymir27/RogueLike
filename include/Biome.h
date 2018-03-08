//
// Created by oliver on 02/03/18.
//

#pragma once


#include "Common.h"
#include "Factory.h"
#include "Utils.h"

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

    struct Biome;
    struct Condition
    {
        virtual bool isSatisfiedBy(Biome const& biome) = 0;
    };

    template <typename T>
    struct AttributeCondition : Condition
    {
        public:
        const T attribute_;
        const EComparison comp_;
        AttributeCondition(T attribute, EComparison comp) : attribute_(attribute), comp_(comp) {}
        bool isSatisfiedBy(Biome const& biome);
    };

    struct Biome
    {
        string      name_;
        Temperature temp_;
        Humidity    hum_;
        FloorType   floor_;
        string      texture_;

        Biome(string const& name = "Template", int temp = 0, int hum = 0, EFloorType floor = GRASS, string const& texture = "images/tileset.png");
        Biome(Biome* orig);

        bool operator==(Biome const& other) { return name_ == other.name_; }
    };
}

//Facotry constructor
template <>
Factory<Biomes::Biome>::Factory();
