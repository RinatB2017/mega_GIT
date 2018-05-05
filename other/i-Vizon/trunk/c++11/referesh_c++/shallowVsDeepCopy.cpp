// 05/04/2014
/**
 * In this tutorial we will explore difference between
 * shallow copy and deep copy.
 * Indigenous Data : Completely contained by the structure
 * Exogeneous Data : Not part of structure but acccessed through pointers
 * A shallow copy is a copy of pointers rather than data being pointed at.
 * A deep copy is a copy of the data being pointed at rather than the pointers.
 * And also explore some tricks with structure datatype
 */


#include <iostream>
#include <string>

typedef struct employee
{
    char*       firstName;  //Exogeneous Data
    char*       lastName;
    std::string designation;
    int         empNum;  //Indigenous Data
} EMPLOYEE;

void displayRecord(EMPLOYEE *emp)
{
    /**
     * @TIPS:
     *      When member variables are accessed by *ptrObj.memberName
     *      postfix operator "." is given high precedence than
     *      prefix *. So always use ().
     *      Feeling lazy, use -> operator     *
     */
    std::cout<<"Employee Details"<<std::endl;
    std::cout<<"Name        : "<<(*emp).firstName<<(*emp).lastName<<std::endl;
    //std::cout<<"ID          : "<<*emp.empNum<<std::endl; //Error : See the tips
    std::cout<<"ID          : "<<(*emp).empNum<<std::endl;
    std::cout<<"Desgination : "<<emp->designation<<std::endl;
}

void learnFromError_0()
{
    /// @TIPS: Donts assign uninitialized pointer
    int x =10, *ptr;
    *ptr = x;
    std::cout<<"ptr : You are luck if you see my value "<<*ptr<<std::endl;
    //See what happens by removing std::endl
}
void learnFromError_1()
{
    int x = 12, *ptr = &x; //Declaration
    std::cout<<"Not a big deal you seeing the value : "<<*ptr<<std::endl;

    int y = 15, *ptr1;
    /// Enable to see the error
    //*ptr1 = &y; //The * in the declaration is not a dereferencing
                //operator; instead, it is part of the type.
     //std::cout<<"ptr1 : You are luck if you see my value "<<*ptr1<<std::endl;

    /// @TIPS: Because of precedence rules, * ptr++ increments ptr, not *ptr .

}

int main()
{
    learnFromError_1();
    //Assign memory from heap
    char* firstName = new char[10];
    /// !TODO: Need to see how to copy string to memory
    firstName = "Mageswaran";
    /// !TODO: In which section the lastName is stored.( data / string / static / heap)
    char* lastName = "D";
    std::string roll = "Senior Software Engineer";

    EMPLOYEE employee1 = {firstName,
                          lastName,
                          designation : roll,
                          empNum : 300057};
    EMPLOYEE employee2;

    //Lets check what we have put in structure
    displayRecord(&employee1);

    //Lets check our shallow copy
    employee2 = employee1;
    displayRecord(&employee2);

    //Let us test of shallow copy by enabling below delete
    //delete employee2.lastName;
    //displayRecord(&employee1);
    //Seems we are busted
}

