#include "thermo_pixel_vector.hpp"

using namespace std;

//#define DEBUG_CONSOLE_OUTPUT

thermo_pixel_vector::thermo_pixel_vector()
{
    current_length =0;

}

void thermo_pixel_vector::add_pixel(thermo_pixel value)
{
    int insert_index = current_length;

    bool dojob = true;

    if(current_length == 0)
    {
        vector[0] = value;
    }
    else
    {
        while(   (value.get_temperature() > vector[insert_index - 1].get_temperature()) && (insert_index > 0)   )
        {
            insert_index--;
        }
        shift(insert_index);
        vector[insert_index] = value;

    }

    current_length++;

    #ifdef DEBUG_CONSOLE_OUTPUT
    //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
    int i;
    for(i=0; i<current_length; i++)
    {
        cout << " >> " << vector[i].get_temperature();
    }
    cout << endl;
    //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
    #endif
}

void thermo_pixel_vector::cut(int value)
{
    current_length -= value;
    if(current_length < 0)
        current_length = 0;
}

void thermo_pixel_vector::shift(int index)
{
    int up_index = current_length;

    while(up_index > index)
    {
        vector[up_index] = vector[up_index - 1];
        up_index--;
    }
}

int thermo_pixel_vector::get_current_length()
{
    return current_length;
}

thermo_pixel thermo_pixel_vector::get_pixel(int index)
{
    return vector[index];
}


void thermo_pixel_vector::clean(void)
{
    current_length = 0;
}