//
// Created by oliver on 03/03/18.
//

#include "Biome.h"

namespace Biomes
{

    template <typename T>
    Attribute<T>::Attribute(T value) : value_(value)
    {

    }

    template <typename T>
    bool Attribute<T>::compare(EComparison comp, Attribute<T> other) const
    {
        switch(comp)
        {
            case SMALLER: return value_ <  other.value_;
            case EQUAL:   return value_ == other.value_;
            case BIGGER:  return value_ >  other.value_;
        }
    }

    //------------------------------------------------------------

    Temperature::Temperature(int value) : Attribute(value)
    {

    }

    Humidity::Humidity(int value) : Attribute(value)
    {

    }

    FloorType::FloorType(EFloorType value) : Attribute(value)
    {

    }

    //------------------------------------------------------------

    bool Biome::satisfies(Condition<Temperature> cond)
    {
        return temp_.compare(cond.eq_, cond.attribute_);
    }

    bool Biome::satisfies(Condition<Humidity> cond)
    {
        return hum_.compare(cond.eq_, cond.attribute_);
    }

    bool Biome::satisfies(Condition<FloorType> cond)
    {
        return  floor_.compare(cond.eq_, cond.attribute_);
    }
}