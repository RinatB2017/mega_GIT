//date : 27/03/2014
#include <iostream>
#include <vector>
#include <string>
using namespace std;

/// Enable different parts of the code to see what happens

/// @TIPS : Never use "delete" on pointer that was not
///         created by "new"
/// @TIPS : Stale Pointer : A stale pointer is a pointer
///         whose alue no longer refers to a valid object
/// @TIPS : Reference variables needs to be initialized while declaring
void test_stale_poniter()
{
    //You are doing this in the northern pole of your code
    string* ptr1 = new string("Mageswaran");
    string* ptr2  = ptr1;

    cout<<"Value of ptr1 :"<<ptr1->c_str()<<endl;
    cout<<"Value of ptr2 :"<<ptr2->c_str()<<endl;

    //You are doing this in INDIA of your code
    delete ptr2;

    //You are doing this in the southern pole of your code
    //cout<<"Value of ptr1->c_str() :"<<ptr1->c_str()<<endl; //Prints though nothing appears
    //cout<<"Value of ptr2->c_str() :"<<ptr2->c_str()<<endl; // Memory Error
    cout<<"Value of ptr1 :"<<*ptr1<<endl; //Nothing prints (Segmentation Fault)
    cout<<"Value of ptr2 :"<<*ptr2<<endl;
}
void test_double_deletion()
{
    //You got a new car
    string* car = new string("BMW");

    cout<<"Name :"<<*car<<endl;

    //You got Amnesia (not your wife!)
    //For you new car is
    string* newCar  = car;

    //You decided to recycle it and you did it
    delete newCar;

    //Now your wife wants to resell it
    delete car; // You are busted
}
string* test_pointer_return()
{
//    string autoVariable = "Dont return this as this is will be cleared as soon as we leave this function";
//    return& autoVariable;
    string* ptrVariable  = new string("Now You See Me");
    return ptrVariable;
}

/// @TIPS : Reference variables are equivalent of pointers
///         but dereferenced implicitly
/// @TIPS : Pointers can be Passed by reference.
///         As a result, a function can change wherea pointer is pointing.

void swapUsingReference(int& a, int& b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

int new_main()
{
    //test_stale_poniter();
    //test_double_deletion();
    //cout<<"Returned value from test_pointer_return function call :"<<*test_pointer_return()<<endl;
    int x = 5, y = 1;
    cout<<" Before swap :- Values of x & y: "<<x<<" "<<y<<endl;
    swapUsingReference(x, y);
    cout<<" After  swap :- Values of x & y: "<<x<<" "<<y<<endl;
    return 0;
}
