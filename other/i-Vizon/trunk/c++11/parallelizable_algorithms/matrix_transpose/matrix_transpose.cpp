#include <iostream>

using namespace std;
/// @TIPS:  The gist of it is that arrays decay into pointers once,
///         it doesn't happen recursively. An array of arrays decays
///         into a pointer to an array, not into a pointer to a pointer.
void print2DMatrixIncorrectWay(int **mat, int width, int height)
{
    int x,y;
    for(y=0; y<height; y++)
        for(x=0; x<width; x++)
            std::cout<<" "<<mat[y * width + x];
}

void print2DMatrix(int width, int height, int mat[][3])
//void print2DMatrix(int width, int height, int (*mat)[3])
{
    int x,y;
    for(y=0; y<height; y++)
        for(x=0; x<width; x++)
            std::cout<<" "<<mat[y][x];
}
int main()
{
    cout<<"Hello World \n";
    int a[4][3] = { 1,2,3,
                    4,5,6,
                    7,8,9,
                    0,0,0};
    int b[3][4];

    print2DMatrix(3, 4, a);
}
