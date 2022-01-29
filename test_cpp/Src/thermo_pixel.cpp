#include "thermo_pixel.hpp"


using namespace cv;


thermo_pixel::thermo_pixel()
{
    brightness = UNREAL_BRIGHTNESS;
    temperature = UNREAL_TEMPERATURE;
}

void thermo_pixel::set_position(int x, int y)
{
    position.x = x;
    position.y = y;
}


void thermo_pixel::set_brightness(int value)
{
    brightness = value;
}
void thermo_pixel::set_temperature(double value)
{
    temperature = value;
}

Point thermo_pixel::get_position(void)
{
    return position;
}
int thermo_pixel::get_brightness(void)
{
    return brightness;
}
double thermo_pixel::get_temperature(void)
{
    return temperature;
}