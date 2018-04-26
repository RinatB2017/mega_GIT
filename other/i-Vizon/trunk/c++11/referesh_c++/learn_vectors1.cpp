//date : 26/03/2014
#include <iostream>
#include <vector>
#include <stdlib.h> //For rand function

using namespace std;
/// Equivalent of Histogram
/// Algorithm:
/// 1. Generate 0-100 numbers and store in a vector
/// 2. Get a value from user
/// 3. Generate those many random values and increase appropriate value in the vector
/// 4. Display the frequency of generated random numbers in the vectors.
int main1()
{
    const int RANGE = 255;
    int numOfRandomValues;
    vector<int> colorValues (RANGE +1);

    for(int i=0; i<colorValues.size(); i++)
        colorValues[i] = 0;

    cout<<"Enter the number of randomvalues to be generated: ";
    cin>>numOfRandomValues;

    for(int i=0; i<numOfRandomValues; i++)
        colorValues[rand() % RANGE + 1]++;

    cout<<"Histogram of generated values: ";
    for(int i=0; i<colorValues.size(); i++)
        cout<<i<<" occurs "<<colorValues[i]<<" times \n";

    return 0;
}

