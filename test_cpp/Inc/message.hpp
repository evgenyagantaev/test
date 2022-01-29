/*!
 @file
 @startuml

    class message_t 
    {
        + uint8_t len
        + uint8_t data[255]
    }

 @enduml
*/


#pragma once

#include <stdint>
#include <string>

const size_t MESSAGE_LENGTH = 255;

typedef struct 
{

    uint8_t len;
    uint8_t data[255]

} message_t;