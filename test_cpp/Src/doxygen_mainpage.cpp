/*! 
 @mainpage Шаблонный проект; язык C++; система сборки cmake;

Проект является работающим приложением. Реализует функциональность OpenCV. Использует
gTest в качестве фреймворка для тестирования. Использует doxygen для формирования шаблонного 
примера самодокументации. Использует plantuml для создания графических диаграм.

- @subpage manual_page "Инструкция по эксплуатации"

- @subpage diagram_page "Диаграммы структуры программного обеспечения"

*/ 

/*! 
 @page manual_page Инструкция по эксплуатации

Пример оформления многостраничной документации:
Проект копируется или клонируется с гитхаба и настраивается под свои нужды.

*/

/*! 
 @page diagram_page Диаграммы структуры программного обеспечения
 
Пример использования страниц документации, содержащих диаграммы.

 @startuml
    interface I_Key
    {
        +get_state()
        +set_state(bool state)
        +get_count()
        +increment_count()
        +reset_count()
        +get_pin()
    }

    note top of Key 
    singleton
    end note

    class Key <gpio_num_t Key_pin>
    {
        -pressed
        -count_ms
        -key_pin
        -Key()
        -Key(const Key&)
        -Key& operator=(const Key&)
        -~Key()
        -----
        +{static} Key& get_instance()
        .....
        +get_state()
        +set_state(bool state)
        +get_count()        
        +increment_count()
        +reset_count()
        +get_pin()
    }

    I_Key <|-up- Key : implements
 @enduml


 */

