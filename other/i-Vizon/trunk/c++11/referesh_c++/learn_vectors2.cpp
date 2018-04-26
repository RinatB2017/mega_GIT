//date : 27/03/2014
#include <iostream>
#include <vector>

using namespace std;

#define OPTIMIZED_RESIZE 1
#define USING_PUSH_BACK 0

void getIntsInput(vector<int>& array)
{
    unsigned int iteamReadIndex = 0;
    int value;
    array.resize(0);
    cout<<"Entered Unlimitted Inputs (terminate with an charater):\n";
    while(cin >> value)
    {
#if OPTIMIZED_RESIZE
        if(array.size() == iteamReadIndex)
        array.resize(array.size() * 2 + 1); // Resize double the size of current
                                            // size to reduce memory ops
        array[iteamReadIndex++] = value;
#elif USING_PUSH_BACK
        array.push_back(value);
#else
        array.resize(iteamReadIndex + 1); //Expensive memory ops
        array[iteamReadIndex++] = value;
#endif
    }
#ifdef OPTIMIZED_RESIZE
    array.resize(iteamReadIndex);
#endif
}

int main2()
{
    vector<int> userInput;
    getIntsInput(userInput);
    cout<<"Entered "<<userInput.size()<<" values :\n";
    for(unsigned int i=0; i < userInput.size(); i++)
        cout<<userInput[i]<<"\n";
    return 0;
}

