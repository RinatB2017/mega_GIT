

///This is to prove that the size of array can't be
///calculated directly as the address of array decays to
///a pointer ans the resultant value will be sizeof pointer
///which is equal to NO Of ADDRESS LINES(32bi/64bit)
#include <iostream>
using namespace std;

template <typename T, size_t N>
size_t foo(const T (&buffer)[N])
{
    cout << "size: " << N << endl;
    return N;
}

template <typename T>
size_t foo1(T& N)
{
    cout << "size: " << sizeof(N) << endl;
}


template <typename T>
size_t fooOrdinary(T N)
{
    cout << "size: " << sizeof(N) << endl;
}

int main()
{
    cout << "Hello World!" << endl;

    char a[42];
    int b[100];
    short c[77];

    foo(a);
    foo(b);
    foo(c);

    fooOrdinary(a);
    fooOrdinary(b);
    fooOrdinary(c);

    return 0;
}

