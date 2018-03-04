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

    enum Equality { SMALLER, EQUAL, BIGGER };

    struct Biome;

    template <class T>
    class Attribute
    {
    public:
        T value_;
        Attribute(T value) : value_(value) {};
        //returns the attribute of this type from Biome
        virtual Attribute<T> const& getAttribute(Biome const& b);// = 0;
    };

    class Temperature : public Attribute<int>
    {
    public:
        Temperature(int value) : Attribute(value) {};
        Attribute<int> const& getAttribute(Biome const& b) final;
    };

    class Humidity : public Attribute<int>
    {
    public:
        Humidity(int value) : Attribute(value) {};
        Attribute<int> const& getAttribute(Biome const& b) final;
    };

    class FloorType : public Attribute<EFloorType>
    {
    public:
        FloorType(EFloorType value) : Attribute(value) {};
        Attribute<EFloorType> const& getAttribute(Biome const& b) final;
    };

    struct Biome
    {
        Temperature temp_;
        Humidity hum_;
        FloorType floor_;
    };

    class ICondition
    {
    public:
        virtual bool isSatisfiedBy(Biome const& b) = 0;
    };

    template <class T>
    class AttributeCondition : ICondition
    {
    public:
        AttributeCondition(T attr, Equality eq);
        bool isSatisfiedBy(Biome const& b) final;

    private:
        T attr_;
        Equality eq_;
    };

    class AtLeastOneCondition : ICondition
    {
    public:
        AtLeastOneCondition(list<ICondition*> conditions);
        bool isSatisfiedBy(Biome const& b) final;
    private:
        list<ICondition*> conditions_;
    };
}
