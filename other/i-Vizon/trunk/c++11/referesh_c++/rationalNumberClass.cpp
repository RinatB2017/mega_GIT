#include "rationalNumberClass.h"

void Rational::fixSign()
{
    if(denominator < 0)
    {
        denominator = -denominator;
        numerator = -numerator;
    }
}

void Rational::reduce()
{
    IntType divident = 1;

    if(numerator != 0 && denominator != 0)
    {
        //d = gcd(numerator, denominator);
    }
    if(d > 1)
    {
        numerator /= d;
        denominator /= d;
    }
}
