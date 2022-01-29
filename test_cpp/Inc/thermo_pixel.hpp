#pragma once

#include "opencv_headers.hpp"

#define UNREAL_BRIGHTNESS -1
#define UNREAL_TEMPERATURE -273


class thermo_pixel
{
private:

    cv::Point position;
    int brightness;
    double temperature;
    
public:

    thermo_pixel();

    void set_position(int x, int y);
    void set_brightness(int value);
    void set_temperature(double value);

    cv::Point get_position(void);
    int get_brightness(void);
    double get_temperature(void);
    
};

