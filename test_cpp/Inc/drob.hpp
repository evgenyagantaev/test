#include <cassert>
#include <iostream>
 
template <typename T>
const T& max_template(const T& a, const T& b)
{
    if(a>b)
        return a;
    else
        return b;
}