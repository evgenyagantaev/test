#include "key.hpp"
#include "key_factory.hpp"



/// @brief create anonymous namespace to isolate names in accordance with factory pattern 
namespace
{


    /// @brief artificial syntax construction just to call registration function
    const bool key_creation_callback_registered = 
        Key_factory::get_instance().register_creation_callback(
            KEY_CLASS_ID, 
            []()->Key*
            {return new Key;}
        );

    

} // namespace

bool Key::bind_pin(gpio_num_t pin)
{
    key_pin = pin;
    gpio_set_direction(key_pin, GPIO_MODE_INPUT);
}