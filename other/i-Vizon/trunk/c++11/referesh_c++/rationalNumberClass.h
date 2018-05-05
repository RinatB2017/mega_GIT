#ifndef RATIONALNUMBERCLASS_H
#define RATIONALNUMBERCLASS_H

/**
 *  To undestand additional features of class
 *  1. Distinction between intialization and assignment
 *  2. Implicit type converions
 *  3. More on operator overloading
 *  4. I/O including friend consept
 *  5. Private global variables (private static variables)
 */
typedef long IntType;
class Rational
{
public:
    /// Constructors
    /**
     *  Befor we start constructors, it is best practise to visualize how
     *  we want our class object to be constructed and then providing those
     *  implentation
     *  For Rational Number:
     *  1. When object is constructed with no values
     *  2. When numerator is supplied and considering default denominator
     *  3. When numerator and denominator are supplied
     *  4. When an another object is passed
     */

    Rational(IntType num = 0)
        : numerator(num), denominator(1)
    {
//        Members are initialized in the order
//        they are declared, not in the order they are
//        encountered in the initialization list.
//        Generally, it is best to avoid writing code
//        that depends on this fact

    }
    //Alternative
//    Rational(IntType num = 0)
//    {
//        numerator = num;
//        denominator = 1;
//        /**
//         * In case IntType is a class, then numerator and denominator are
//         * initialized with zero-parameter default constructor and copy
//         * assignment operator is then called.
//         */
//    }

    Rational(IntType num, IntType den)
        :numerator(num), denominator(den)
    {

    }
    Rational(const Rational& temp)
        :numerator(temp.numerator), denominator(temp.denominator)
    {

    }

    ~Rational()
    {

    }

    //Assignment operators
    const Rational& operator=  (const Rational& rhs);
    const Rational& operator+= (const Rational& rhs);
    const Rational& operator-= (const Rational& rhs);
    const Rational& operator/= (const Rational& rhs);
    const Rational& operator*= (const Rational& rhs);

    //Arithematic Operators
    Rational operator+ (const Rational& rhs) const;
    Rational operator- (const Rational& rhs) const;
    Rational operator/ (const Rational& rhs) const;
    Rational operator* (const Rational& rhs) const;

    //Rational and equality operators
    Rational operator<  (const Rational& rhs) const;
    Rational operator<= (const Rational& rhs) const;
    Rational operator>  (const Rational& rhs) const;
    Rational operator>= (const Rational& rhs) const;
    Rational operator== (const Rational& rhs) const;
    Rational operator!= (const Rational& rhs) const;

    //Unary Operators
    const Rational& operator++ () const; //prefix ++obj



protected:
private:
    IntType numerator;
    IntType denominator;

    void fixSign();
    void reduce();
};

#endif // RATIONALNUMBERCLASS_H
