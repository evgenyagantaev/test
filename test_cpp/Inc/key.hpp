/*!
 @file
 @startuml
    interface I_Key
    {
        +get_state()
        +set_state(bool state)
        +get_count()
        +increment_count()
        +reset_count()
        +bind_pin(gpio_num_t  pin)
        +get_pin()
    }

    class Key 
    {
        -pressed
        -count_ms
        -key_pin
    }

    class Key_factory

    Key_factory -> Key : creates

    I_Key <|-up- Key : implements
 @enduml
*/

#pragma once

#include "main.hpp"
#include <cstring>

/// @brief integer class identificator for object production factory class
const int KEY_CLASS_ID = 1;

/*!
 @brief 
*/

/*!
 @brief Public interface of one device key 
*/
class I_Key
{
    public:

    virtual bool get_state() const = 0;
    virtual void set_state(bool state) = 0;
    virtual std::size_t get_count() const = 0;
    virtual std::size_t increment_count() = 0;
    virtual std::size_t reset_count() = 0;

    virtual bool bind_pin(gpio_num_t pin) = 0;
    virtual gpio_num_t get_pin() const = 0;
};

/*!
 @brief Class that represents one key of device keyboard
 @details 
    


*/
class Key: public I_Key
{
    private:

    bool pressed = false;
    size_t count_ms = 0;

    gpio_num_t key_pin;

    public:

    /*!
    @defgroup interface_implementation This module implements interface I_Key
    @{ 
    */

    virtual bool get_state() const
    {
        return pressed;
    };

    virtual void set_state(bool state)
    {
        pressed = state;
    };

    virtual std::size_t get_count() const
    {
        return count_ms;
    };

    virtual std::size_t increment_count()
    {
        count_ms++;
        return count_ms;
    };

    virtual std::size_t reset_count()
    {
        count_ms = 0;
        return count_ms;
    };

    virtual bool bind_pin(gpio_num_t pin);

    virtual gpio_num_t get_pin() const
    {
        return key_pin;
    };

    /*! @} */

};