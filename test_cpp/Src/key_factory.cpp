#include "key_factory.hpp"

    /// @brief registers callback which creates object of certain type
    /// @details saves pair (id, callback_pointer) in std::map callback_map
    /// @param id integer callback identificator; 
    /// @param callback_pointer pointer to callback function which creates concrete object
    /// @returns true if success, false in another case
    bool Key_factory::register_creation_callback(int id, creation_callback_t callback_pointer)
    {
        bool return_value = false;

        std::pair<int, creation_callback_t> pair_to_register = std::make_pair(id, callback_pointer);

        bool this_key_yet_registered = false;

        /// @brief check, if pair with the same key yet exists
        auto iterator = callback_map.find(id);        

        if(iterator != callback_map.end())
        {
            /// replace the callback with new one
            iterator->second = pair_to_register.second;
        }
        else
        {
            /// register new pair
            return_value = (callback_map.insert(pair_to_register)).second;
        }

        return return_value;
         
    }// end bool Key_factory::register_creation_callback(int id, creation_callback_t callback_pointer)

    /// @brief unregisters callback
    /// @details removes pair (id, callback_pointer) from std::map callback_map
    /// @param id integer callback identificator;
    /// @returns true if success, false in another case 
    bool Key_factory::unregister_creation_callback(int id)
    {
        return callback_map.erase(id) == 1;
    }

    /// @brief calls callback which creates desired object
    /// @details finds callback pointer by id in std::map callback_map; calls callback by pointer;
    /// @param id integer callback identificator; 
    /// @returns pointer to newly created object of Key type
    Key* Key_factory::create(int id)
    {
        auto iterator = callback_map.find(id);        

        if(iterator != callback_map.end())
        {
            items_created++;
            /// call creation callback
            return (iterator->second)();
        }
        else
        {
            return nullptr;
        }
    }

    