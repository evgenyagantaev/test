#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "opencv_headers.hpp"



enum class SPOT_TYPE
{
    EYE,
    FOREHEAD
};

class rect_thermal_spot
{
private:

    SPOT_TYPE spot_type;     // 0 for eyes; 1 for forehead

    cv::Point base_point;
    int width;
    int height;

    cv::RNG random_generator;
    uint64_t rng_seed;
    
public:

    rect_thermal_spot();

    void set_base(int x, int y);
    void set_width(int value);
    void set_height(int value);

    cv::Point get_base(void);
    int get_width(void);
    int get_height(void);

    cv::Point generate();

    void set_type(SPOT_TYPE type);
    SPOT_TYPE get_type();

};

