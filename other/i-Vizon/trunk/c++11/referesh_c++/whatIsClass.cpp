//05/4/2014
#include "whatIsClass.h"
/// Implementation
String::String(char* string)
{
    int i, length = 0;
    for(i =0; string[i]!='\0'; i++)
        ++length;
    memcpy(value, string, length); //returns a ptr to value i.e destination

}

int IntCell::read( ) const
{
    return storedValue;
    //storedValue = 0; //causes compiler error
    //error: assignment of member 'IntCell::storedValue' in read-only object
}

void testExplicit()
{
    std::cout<<"Testing explicit keyword :"<<std::endl;
    IntCell obj;
    /// Enable below lines, to see compiler error
    /// no known conversion for argument 1 from 'int' to 'const IntCell&'
    //obj = 5
    //IntCell temp = 25;
    //obj = temp;
    //std::cout<<"Read storedValue :"<<obj.read()<<std::endl;
}

/// @TIPS: Default Copy constructors and assignment operators, works
///         well with primitive data types as long as there are no
///         pointers.
void testCopyConstructor()
{
//    The copy constructor is called when an
//    object is passed or returned by value

//    By default, the copy constructor is a
//    member-by-member application of copy constructors.


    String obj1;
    String obj2 = obj1;
    String obj3(obj1);
    String obj4;
    //obj4 = obj1; This is assignment operator
}

void testEqualOperator()
{
//    The copy assignment operator operator=
//    is used to copy objects.

}
