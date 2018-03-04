//
// Created by oliver on 03/03/18.
//

#include "Biome.h"

namespace Biomes
{
    Attribute<int> const& Temperature::getAttribute(Biome const& b)
    {
        return b.temp_;
    }

    Attribute<int> const& Humidity::getAttribute(Biome const& b)
    {
        return b.hum_;
    }

    Attribute<EFloorType> const& FloorType::getAttribute(Biome const& b)
    {
        return b.floor_;
    }

    template<>
    AttributeCondition<Temperature>::AttributeCondition(Temperature attr, Equality eq) : attr_(attr), eq_(eq)
    {

    }

    template<>
    AttributeCondition<Attribute<int>>::AttributeCondition(Attribute<int> attr, Equality eq) : attr_(attr), eq_(eq)
    {

    }


    template <>
    bool AttributeCondition<Temperature>::isSatisfiedBy(Biome const& b)
    {
        switch (eq_)
        {
            case SMALLER: return attr_.value_ <  attr_.getAttribute(b).value_;
            case EQUAL  : return attr_.value_ == attr_.getAttribute(b).value_;
            case BIGGER : return attr_.value_ >  attr_.getAttribute(b).value_;
        }
    }


    //-----------------------------------------------------------------------

    bool AtLeastOneCondition::isSatisfiedBy(Biome const& b)
    {
        for(auto& cond : conditions_)
        {
            if(cond->isSatisfiedBy(b))
                return true;
        }
        return false;
    }

    AtLeastOneCondition::AtLeastOneCondition(list<ICondition*> conditions) : conditions_(conditions)
    {

    }


}