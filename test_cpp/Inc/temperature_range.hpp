#pragma once

#define TEMPERATURE_LOW_BOUND 33
#define TEMPERATURE_UP_BOUND 40


class temperature_range
{
private:
    double low_bound;
    double up_bound;

public:

    temperature_range();

    bool check(double temperature);
};

