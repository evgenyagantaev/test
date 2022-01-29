/*!
 @file
 @startuml
    
    note top of Key_factory 
        factory creates keys
        for device keyboard
    end note

    class Key_factory 
    {
        -typedef std::map<size_t, creation_callback_t> callback_map_t
        -callback_map_t callback_map
        -size_t items_created
        .....
        -Key_factory()
        -Key_factory(const Key_factory&)
        -Key_factory& operator=(const Key_factory&)
        -~Key_factory()
        -----
        +{static} Key_factory& get_instance()
        .....
        +typedef Key* (*creation_callback_t)() 
        +bool register_creation_callback(int ID, callback_pointer)
        +bool unregister_creation_callback(int ID)
        +Key* create(ID)
        .....
        +size_t get_map_size()
        +size_t get_items_created()
    }

    note left of Key_factory::get_instance()
        singleton
    end note

    class Key
    Interface I_Key

    I_Key <|-up- Key : implements

    Key_factory -> Key : creates

    
 @enduml
*/

#pragma once
#include "main.hpp"
#include <map>
#include "key.hpp"


/*!
 @brief Factory of objects of type Key
 @details
    designed as singleton;
    implements factory pattern;
*/
class Key_factory
{
    public:

    typedef Key* (*creation_callback_t)();

    private:

    typedef std::map<int, creation_callback_t> callback_map_t;
    callback_map_t callback_map;
    size_t items_created = 0;
    
    //*****************

    Key_factory(){};
    Key_factory(const Key_factory&);
    Key_factory& operator=(const Key_factory&);
    ~Key_factory(){};

    public:

    static Key_factory& get_instance()
    {
        static Key_factory obj;
        return obj;
    }
    //****************

    /// @brief registers callback which creates object of certain type
    /// @details saves pair (id, callback_pointer) in std::map callback_map
    /// @param id integer callback identificator; 
    /// @param callback_pointer pointer to callback function which creates concrete object
    /// @returns true if success, false in another case
    bool register_creation_callback(int id, creation_callback_t callback_pointer);

    /// @brief unregisters callback
    /// @details removes pair (id, callback_pointer) from std::map callback_map
    /// @param id integer callback identificator;
    /// @returns true if success, false in another case 
    bool unregister_creation_callback(int id);

    /// @brief calls callback which creates desired object
    /// @details finds callback pointer by id in std::map callback_map; calls callback by pointer;
    /// @param id integer callback identificator; 
    /// @returns pointer to newly created object of Key type
    Key* create(int id);

    /// @brief returns current length of callback map; just for unit testing
    /// @returns current length of callback map
    size_t get_map_size()
    {
        return callback_map.size();
    }

    size_t get_items_created()
    {
        return items_created;
    }

};