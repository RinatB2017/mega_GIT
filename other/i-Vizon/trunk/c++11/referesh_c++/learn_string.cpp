//date : 27/03/2014
#include<iostream>
#include <string>

using namespace std;
/// Different ways of accessing "string"
int main3()
{
    string A = "Mages";
    string B = "waran";
    string C;   //should be ""

    A += " ";
    A += B;
    C = A + B;

    ///Mehod 1
    cout<<"String is :"<<C<<endl;
    ///Method 2
    cout<<"String is :"<<C.c_str()<<endl;
    ///Method 3
    cout<<"String is :";
    for(int i=0; i< C.size(); i++)
        cout<<C[i];
    cout<<endl;
    return 0;
}
