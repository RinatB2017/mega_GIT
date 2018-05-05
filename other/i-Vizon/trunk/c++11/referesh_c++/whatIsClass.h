#ifndef WHATCLASS_H
#define WHATCLASS_H

#include <iostream>
#include <string.h>

/**
 *  All about class data type.
 **/
/**
 *  A story to rember:
 *  Class room is a best analogy to class datatype.
 *  Each student is a member function and their brain
 *  act as a member variable.
 *
 *  Professors are general programmers, who are not allowed to
 *  touch the brain directly, they have to go through the member
 *  functions in-order to process their boring lectures.
 *  Here again the inputs are same but how it is processed is completly
 *  dependent on, how a member function is capable i.e how the student is
 *  able to understand and process the information.
 *
 *  A son of mathematical scientist may be or may not be a good
 *  maths solver right. Its all depends on how his/her gene is inherited from
 *  their parents/grand parents. Which says how much he/she is talented by birth.
 *  Sometimes he/she will  be having parents talents or grand parents talents.
 *
 *  Professor  nature is same, teaching. But they alter their teacings
 *  based on students understanding ability.
 */
/// An object is an entity that has structure and state. Each object
/// defines operations that may access or manipulate that state.

/// Data Hiding : Users are not allowed to access the member variables
///               directly rather only through member functions(eg: get/set). Which says
///               class is a atomic unit, just like a float, cant play with its bits!

/// Encapsulation : Putting member variables and member functions together, making an aggregate.
///                 While hiding the implementation details.
///                 Eg: Template

/// Inheritance : Code reuse

/// Polymorphism :

/// @TIPS   : Object Based Programming : Data Hiding + Encapsulation
/// @TIPS   : Object Oriented Programming : Data Hiding + Encapsulation + Inheritance
/// @TIPS   : A class is the same as a Structure except that, by default, all members are
///           inaccessible.
/// @TIPS   : Functions in a class are methods
/// @TIPS   : C vs  C++ : Objects are in charge
/// @TIPS   : The interface lists the class and its members (data
///           and functions) and describes what can be done to an object.
/// @TIPS   : The implementation represents the internal processes by which the
///           interface specifications are met.


/// @TIPS   : class declaration generally should go into headers
class String //Best practise to start a class with capital letter
{
public:
    /// Is like garden outside your gate
    /// Constructor describes how an object is declared and initialized
    /// You forget to declare, compiler will do it for you
    /// Big three: Destructor, Copy Constructor & operator=,
    /// provided default by compiler
    // Its like you give what your girl friends ask for,or she will
    // be getting it by other means* :).
    /// The moment you define parametrized constructor, it is your
    /// resposibility to declare constructor without parameters

    String()
    {
        value = NULL;
    }

    /// Interface i.e no definition in class
    String(char* string);

    /// Copy Constructor is called when an object is passes as argument or return by value
    /// @TIPS   : Disable by placing declaration in private section
    String(const String& temp)
    {
        //this
    }

    /// Assignment Operator
    const String& operator=(String& rhs)
    {
        if(this != &rhs)
        {
        }// standard alias test
    }

    /// A method that examines but does not change the state of its object is a accessor

    bool isEmpty();
    /// A member function that changes the state of an object is a mutator
    /// (it mutates the state of the object).

    void makeEmpty();

    ~String()
    {
        /// Clears all auto variables
        /// Its is class cerator responsibility to clear "new"ed members with delete
    }

protected:
    ///  Is like garden in your compound wall
    //comes last as it is not been touched again and again. Declare and use it.
private:
    /// Is like garden in your balcony
    // Restricted Access : Data hinding
    /// By default this is accessor
    char* value;

};

/// Initialzer list used when we need to initialize
/// const members.
/// if a data member is const (meaning that it cannot be changed after the object
/// has been constructed), then the data member's value can be initialized only
/// in the initializer list.

/// A class for simulating an integer memory cell.
class IntCell
{
public:
    /// One parameter constructor should be declared with keyword
    /// explicit, to avoid behind scenes conversion
    explicit IntCell( int initialvalue = 0 )
        : storedValue( initialvalue )
    {
    }

    /// Copy Constructors
    explicit IntCell(IntCell& rhs) :
        storedValue(rhs.storedValue)
    {

    }

    /// Assignment Operator
    /// @TIPS: Return by reference for chain assignment ob1 = obj2 = obj3
    const IntCell& operator=(const IntCell& rhs)
    {
        /// Test for alias
        //        Aliasing is a special case that occurs when
        //        the same object appears in more than one role.

        if(this != &rhs)
        {
            //            The pointer this points at the current
            //            object. It is used to return a constant
            //            reference for assignmentoperators
            //            and also to test for aliasing.

            this->storedValue = rhs.storedValue;
        }
        return *this;
    }

    /// A constant member function is a function that does not change
    /// any class data members. Accessor
    /// @TIPS: Constant Memeber functions = Accessor
    int read( ) const;

    void write( int x )
    {
        storedValue = x;
    }
private:
    int storedValue;
};

#endif // WHATCLASS_H
