
/*!
 @file
 @startuml
    
    Interface I_message_dispatcher
    {
        +message_t* new_message()
        +void delete_message(message_t* msg)
        +int send(uint8_t destination_id, message_t* msg)
        +int recv(uint8_t receiver_id, message_t* msg)
    }

    class Message_dispatcher 
    {
        -typedef std::map<size_t, creation_callback_t> callback_map_t
        -callback_map_t callback_map
        -size_t items_created
        .....
        -Message_dispatcher()
        -Message_dispatcher(const Message_dispatcher&)
        -Message_dispatcher& operator=(const Message_dispatcher&)
        -~Message_dispatcher()
        -----
         +{static} Message_dispatcher& get_instance()
        .....

    }

    note left of Message_dispatcher::get_instance()
        singleton
    end note

    I_message_dispatcher <|-up- Message_dispatcher : implements

    
 @enduml
*/


class I_message_dispatcher
{

};



class Message_dispatcher : public I_message_dispatcher
{
    private:

    //*****************

    Message_dispatcher(){};
    Message_dispatcher(const Message_dispatcher&);
    Message_dispatcher& operator=(const Message_dispatcher&);
    ~Message_dispatcher(){};

    public:

    static Message_dispatcher& get_instance()
    {
        static Message_dispatcher obj;
        return obj;
    }
    //****************

};