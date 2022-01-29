#include "temperature_range.hpp"

temperature_range::temperature_range()
{
    low_bound = TEMPERATURE_LOW_BOUND;
    up_bound = TEMPERATURE_UP_BOUND;
}

bool temperature_range::check(double temperature)
{
    if((temperature >= low_bound) && (temperature <= up_bound))
        return true;
    else
        return false;
}

